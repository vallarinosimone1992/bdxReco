// $Id$
//
//    File: JEventProcessor_Calorimeter_SipmStudies.cc
// Created: Mon Apr  4 15:27:17 CEST 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include "JEventProcessor_Calorimeter_SipmStudies.h"
using namespace jana;




#include "JEventProcessor_test.h"
#include <system/BDXEventProcessor.h>

#include <DAQ/fa250Mode1Hit.h>
#include <DAQ/fa250Mode1CalibPedSubHit.h>
#include <DAQ/fa250Mode1CalibHit.h>

#include <DAQ/eventData.h>

#include <TT/TranslationTable.h>




#include <Calorimeter/CalorimeterSiPMHit.h>
#include <Calorimeter/CalorimeterHit.h>



#include <system/JROOTOutput.h>
#include "TTree.h"



// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>
extern "C"{
void InitPlugin(JApplication *app){
	InitJANAPlugin(app);
	app->AddProcessor(new JEventProcessor_Calorimeter_SipmStudies());
}
} // "C"


//------------------
// JEventProcessor_Calorimeter_SipmStudies (Constructor)
//------------------
JEventProcessor_Calorimeter_SipmStudies::JEventProcessor_Calorimeter_SipmStudies():
		m_ROOTOutput(0)
{
	hit1=0;
	hit2=0;
	m_isFirstCallToBrun=1;
}

//------------------
// ~JEventProcessor_Calorimeter_SipmStudies (Destructor)
//------------------
JEventProcessor_Calorimeter_SipmStudies::~JEventProcessor_Calorimeter_SipmStudies()
{

}

//------------------
// init
//------------------
jerror_t JEventProcessor_Calorimeter_SipmStudies::init(void)
{
	japp->RootWriteLock();
	t=new TTree("CaloSipm","CaloSipm");

	t->Branch("evnt",&eventNumber);
	//t->SetMakeClass(0);
	t->Branch("hit1",&hit1);
	t->Branch("hit2",&hit2);

	t->Branch("Ec1",&Ec1);
	t->Branch("Ec2",&Ec2);
	t->Branch("Ec",&Ec);

/*	t->Branch("type1",&m_type1);
	t->Branch("type2",&m_type2);



	t->Branch("Q1",&m_Q1);
	t->Branch("Q2",&m_Q2);
	t->Branch("Q1s",&m_Q1s);
	t->Branch("Q2s",&m_Q2s);
	t->Branch("T1",&m_T1);
	t->Branch("T2",&m_T2);
	t->Branch("A1",&m_A1);
	t->Branch("A2",&m_A2);

	t->Branch("ped",&m_ped1);
	t->Branch("pedSigma",&m_pedSigma1);
	t->Branch("ped",&m_ped2);
	t->Branch("pedSigma",&m_pedSigma2);

	t->Branch("goodRMS1",&m_goodRMS1);
	t->Branch("goodRMS2",&m_goodRMS2);



	japp->RootUnLock();



	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t JEventProcessor_Calorimeter_SipmStudies::brun(JEventLoop *eventLoop, int32_t runnumber)
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

			m_ROOTOutput->AddObject(t);
		}
	}

	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t JEventProcessor_Calorimeter_SipmStudies::evnt(JEventLoop *loop, uint64_t eventnumber)
{
	vector <const CalorimeterSiPMHit*> cdata;
	vector <const CalorimeterSiPMHit*>::const_iterator cdata_it;

	vector <const CalorimeterHit*> cdata2;
	vector <const CalorimeterHit*>::const_iterator cdata2_it;
	loop->Get(cdata);
	loop->Get(cdata2);


	japp->RootWriteLock();
	eventNumber=eventnumber;



	for (cdata2_it=cdata2.begin();cdata2_it!=cdata2.end();cdata2_it++){
		const CalorimeterHit *hit= *cdata2_it;
		Ec=hit->E;
		for (int idata=0;idata<hit->m_data.size();idata++){
			switch (hit->m_data[idata].readout){
			case 1:
				Ec1=hit->m_data[idata].E;
				break;
			case 2:
				Ec2=hit->m_data[idata].E;
				break;
			}
		}
	}

	for (cdata_it=cdata.begin();cdata_it!=cdata.end();cdata_it++){
		const CalorimeterSiPMHit *sipmhit= *cdata_it;
		switch (sipmhit->m_channel.int_veto->readout){
		case (1):
			hit1=sipmhit;
		break;
		case (2):
			hit2=sipmhit;
		break;
		default:
			break;

		}
	}

t->Fill();
japp->RootUnLock();


return NOERROR;
}

//------------------
// erun
//------------------
jerror_t JEventProcessor_Calorimeter_SipmStudies::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t JEventProcessor_Calorimeter_SipmStudies::fini(void)
{
	// Called before program exit after event processing is finished.
	return NOERROR;
}

