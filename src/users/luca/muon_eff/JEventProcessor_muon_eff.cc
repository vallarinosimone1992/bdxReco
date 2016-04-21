// $Id$
//
//    File: JEventProcessor_muon_eff.cc
// Created: Wed Apr  6 09:45:37 CEST 2016
// Creator: lmarsicano (on Linux apcx14 2.6.32-573.el6.x86_64 x86_64)
//

#include "JEventProcessor_muon_eff.h"
#include "system/BDXEventProcessor.h"
using namespace jana;

#include <Calorimeter/CalorimeterSiPMHit.h>
#include <Calorimeter/CalorimeterDigiHit.h>
#include <Calorimeter/CalorimeterHit.h>

#include <ExtVeto/ExtVetoHit.h>
#include <IntVeto/IntVetoDigiHit.h>
#include <Paddles/PaddlesHit.h>

#include <MC/CalorimeterMCHit.h>

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>

#include <system/JROOTOutput.h>

#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TF1.h"

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>
extern "C"{
void InitPlugin(JApplication *app){
	InitJANAPlugin(app);
	app->AddProcessor(new JEventProcessor_muon_eff());
}
} // "C"


//------------------
// JEventProcessor_muon_eff (Constructor)
//------------------
JEventProcessor_muon_eff::JEventProcessor_muon_eff():m_ROOTOutput(0)
{
	isMC=0;
	IntVetoTopDigiHit=0;
	IntVetoBotDigiHit=0;
	ExtVetoTopHit6=0;
	ExtVetoTopHit7=0;
	ExtVetoBotHit8=0;
	ExtVetoBotHit9=0;


}

//------------------
// ~JEventProcessor_muon_eff (Destructor)
//------------------
JEventProcessor_muon_eff::~JEventProcessor_muon_eff()
{

}

//------------------
// init
//------------------
jerror_t JEventProcessor_muon_eff::init(void)
{
	// This is called once at program startup. If you are creating
	// and filling historgrams in this plugin, you should lock the
	// ROOT mutex like this:
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();
	//
	gPARMS->GetParameter("MC", isMC);

	app->RootWriteLock();

	jout<<"muon_eff::init is called"<<std::endl;


	t=new TTree("muon_eff","muon_eff");
	t->Branch("eventN",&eventN);
	t->Branch("Qc1",&Qc1);
	t->Branch("Qc2",&Qc2);
	t->Branch("Qctot",&Qctot);
	t->Branch("Tc1",&Tc1);
	t->Branch("Tc2",&Tc2);
	t->Branch("Ep1",&Ep1);
	t->Branch("Ep2",&Ep2);
	t->Branch("Tp1",&Tp1);
	t->Branch("Tp2",&Tp2);
	t->Branch("Tpdiff",&Tpdiff);
	t->Branch("IntVetoTopHitQ",&IntVetoTopDigiHitQ);
	t->Branch("good_ped_RMS",&good_ped_RMS);

	t->Branch("IntVetoTopHit",&IntVetoTopDigiHit);
	t->Branch("IntVetoBotHit",&IntVetoBotDigiHit);
	t->Branch("ExtVetoTopHit6",&ExtVetoTopHit6);
	t->Branch("ExtVetoTopHit7",&ExtVetoTopHit7);
	t->Branch("ExtVetoBotHit8",&ExtVetoBotHit8);
	t->Branch("ExtVetoBotHit9",&ExtVetoBotHit9);




	//Create always MC branch
	t->Branch("Ec_MC",&Ec_MC);

	app->RootUnLock();
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t JEventProcessor_muon_eff::brun(JEventLoop *eventLoop, int32_t runnumber)
{
	if (m_isFirstCallToBrun){
		string class_name,this_class_name;
		string joutput_name;
		BDXEventProcessor *m_BDXEventProcessor;
		vector<JEventProcessor*> m_processors=app->GetProcessors();
		vector<JEventProcessor*>::iterator m_processors_it;

		m_isFirstCallToBrun=0;
		class_name="BDXEventProcessor";
		joutput_name="JROOTOutput";
		//Now I need to determine which processor is the one holding the output. Discussing with David, he suggested just to check the class name, since
		//a dynamic cast may not work with plugins
		for (m_processors_it=m_processors.begin();m_processors_it!=m_processors.end();m_processors_it++){
			if ((*m_processors_it)!=0){
				this_class_name=string((*m_processors_it)->className());
				if (this_class_name==class_name){
					m_BDXEventProcessor=(BDXEventProcessor*)(*m_processors_it);
					if (m_BDXEventProcessor->getOutput()==0){
						jerr<<"BDXEventProcessor JOutput is null!"<<endl;
						break;
					}
					if (string(m_BDXEventProcessor->getOutput()->className())==joutput_name){
						m_ROOTOutput=(JROOTOutput*)(m_BDXEventProcessor->getOutput());
						jout<<"Got JROOTOutput!"<<endl;
					}
					else{
						jerr<<"BDXEventProcessor JOutput is not null BUT class is: "<<m_BDXEventProcessor->getOutput()->className()<<endl;
					}
				}
			}
		}
		/*For ALL objects you want to add to ROOT file, use the following:*/
		if (m_ROOTOutput){
			/*	m_ROOTOutput->AddObject(hp2);
			m_ROOTOutput->AddObject(hp1);
			m_ROOTOutput->AddObject(hp1_2);
			m_ROOTOutput->AddObject(hc1);
			m_ROOTOutput->AddObject(hc2);
			m_ROOTOutput->AddObject(hc1_2);
			m_ROOTOutput->AddObject(hctot); */
			m_ROOTOutput->AddObject(t);
		}

	}
	return NOERROR;




}

//------------------
// evnt
//------------------
jerror_t JEventProcessor_muon_eff::evnt(JEventLoop *loop, uint64_t eventnumber)
{
	// This is called for every event. Use of common resources like writing
	// to a file or filling a histogram should be mutex protected. Using
	// loop->Get(...) to get reconstructed objects (and thereby activating the
	// reconstruction algorithm) should be done outside of any mutex lock
	// since multiple threads may call this method at the same time.
	// Here's an example:
	//
	// vector<const MyDataClass*> mydataclasses;
	// loop->Get(mydataclasses);
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();

	int sector,layer,component;
	double E,T;

	vector <const ExtVetoHit*> evdata;
	vector <const ExtVetoHit*>::const_iterator evdata_it;
	loop->Get(evdata);

	vector <const IntVetoDigiHit*> ivdata;
	vector <const IntVetoDigiHit*>::const_iterator ivdata_it;
	if (isMC==1){
		loop->Get(ivdata,"MC");
	}
	else{
		loop->Get(ivdata);
	}

	vector<const PaddlesHit*> data;
	vector<const PaddlesHit*>::const_iterator data_it;
	loop->Get(data);


	vector<const CalorimeterHit*> cdata;
	vector<const CalorimeterHit*>::const_iterator cdata_it;
	loop->Get(cdata);

	vector<const CalorimeterMCHit*> mc_data;

	japp->RootWriteLock();

	IntVetoTopDigiHit=0;
	IntVetoBotDigiHit=0;
	ExtVetoTopHit6=0;
	ExtVetoTopHit7=0;
	ExtVetoBotHit8=0;
	ExtVetoBotHit9=0;

	good_ped_RMS=1;


	for (evdata_it=evdata.begin();evdata_it!=evdata.end();evdata_it++){
		sector=(*evdata_it)->m_channel.sector;
		layer=(*evdata_it)->m_channel.layer;
		component=(*evdata_it)->m_channel.component;
		if (component==6){
			ExtVetoTopHit6= (*evdata_it);
		}
		if (component==7){
			ExtVetoTopHit7= (*evdata_it);
		}
		if (component==8){
			ExtVetoBotHit8= (*evdata_it);
		}
		if (component==9){
			ExtVetoBotHit9= (*evdata_it);
		}

	}





	for (ivdata_it=ivdata.begin();ivdata_it!=ivdata.end();ivdata_it++){

		sector=(*ivdata_it)->m_channel.sector;
		layer=(*ivdata_it)->m_channel.layer;
		component=(*ivdata_it)->m_channel.component;

		if (component==0){  //Top inner veto
			IntVetoTopDigiHit=(*ivdata_it);
			IntVetoTopDigiHitQ=(*ivdata_it)->Qtot;
		}
		if (component==3){  //Bottom inner veto
			IntVetoBotDigiHit=(*ivdata_it);
		}

	}
	Ep1=Ep2=-1;
	for (data_it=data.begin();data_it<data.end();data_it++){

		const PaddlesHit *evhit = *data_it;
		//jout << evhit->m_channel.id<<endl;
		E = evhit->E;
		T = evhit->T;
		if(evhit->m_channel.id==0){
			Ep1=E;
			Tp1=T;
		}
		if(evhit->m_channel.id==1){
			Ep2=E;
			Tp2=T;
		}
	}

	for (cdata_it=cdata.begin();cdata_it<cdata.end();cdata_it++){
		const CalorimeterHit *evchit= *cdata_it;
		Qc1=0;
		Qc2=0;
		for (int idata=0;idata<evchit->m_data.size();idata++){
			if (evchit->m_data[idata].good_ped_RMS==false) good_ped_RMS=0;
			switch (evchit->m_data[idata].readout){
			case (1):
											Qc1 = evchit->m_data[idata].Q;
			Tc1 = evchit->m_data[idata].T;
			break;
			case (2):
											Qc2 = evchit->m_data[idata].Q;
			Tc2 = evchit->m_data[idata].T;
			break;
			default:
				break;

			}
		}

		if (isMC){
			evchit->Get(mc_data); //use a vector since it is re-iterating!
			if (mc_data.size()!=1){
				cout<<"luca_muon_eff error, no associated CalorimeterMCHit : got "<<mc_data.size()<<endl;
			}
			else{
				Ec_MC=mc_data[0]->totEdep;
			}
		}
		else{
			Ec_MC=-1;
		}

	}

	eventN=eventnumber;
	if((Ep1 > 0.5) && (Ep2 > 0.5)){
		t->Fill();
	}


	japp->RootUnLock();

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t JEventProcessor_muon_eff::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t JEventProcessor_muon_eff::fini(void)
{
	// Called before program exit after event processing is finished.
	return NOERROR;
}

