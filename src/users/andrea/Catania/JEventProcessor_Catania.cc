// $Id$
//
//    File: JEventProcessor_Calorimeter_rate.cc
// Created: Fri Mar 25 18:38:03 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include "JEventProcessor_Catania.h"

#include <system/BDXEventProcessor.h>

#include <DAQ/eventData.h>
#include <DAQ/fa250Mode1CalibHit.h>
#include <DAQ/fa250Mode1CalibPedSubHit.h>
#include <TT/TranslationTable.h>

#include <EventBuilder/CataniaEvent.h>
#include <Calorimeter/CalorimeterHit.h>
#include <Calorimeter/CalorimeterSiPMHit.h>
#include <IntVeto/IntVetoSiPMHit.h>
#include <IntVeto/IntVetoHit.h>
#include <IntVeto/IntVetoDigiHit.h>
#include <IntVeto/IntVetoSummary.h>
#include <ExtVeto/ExtVetoPMTHit.h>
#include <ExtVeto/ExtVetoDigiHit.h>
#include <ExtVeto/ExtVetoHit.h>
#include <ExtVeto/ExtVetoSummary.h>
#include <Paddles/PaddlesHit.h>

#include <MC/CalorimeterMCHit.h>
#include <system/JROOTOutput.h>

#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TTree.h"


using namespace jana;

using namespace std;

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>
extern "C"{
void InitPlugin(JApplication *app){
	InitJANAPlugin(app);
	app->AddProcessor(new JEventProcessor_Catania());
}
} // "C"


//------------------
// JEventProcessor_Calorimeter_rate (Constructor)
//------------------
JEventProcessor_Catania::JEventProcessor_Catania()
{
	caloHit=0;
	m_isMC=0;
	m_ROOTOutput=0;
	m_isFirstCallToBrun=1;
	event=0;
}

//------------------
// ~JEventProcessor_Calorimeter_rate (Destructor)
//------------------
JEventProcessor_Catania::~JEventProcessor_Catania()
{

}

//------------------
// init
//------------------
jerror_t JEventProcessor_Catania::init(void)
{
	// This is called once at program startup. If you are creating
	// and filling historgrams in this plugin, you should lock the
	// ROOT mutex like this:
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();
	//
	gPARMS->GetParameter("MC", m_isMC);


	japp->RootWriteLock();
	t=new TTree("CaloRate","CaloRate");
	t->Branch("eventN",&eventNumber);
	/*Calorimeter hit; Ec1 and Ec2; sipm1 and sipm2*/
	t->Branch("CataniaEvent",&event);
	t->Branch("EcMC",&EcMC);



	for (int iwave=0;iwave<50;iwave++){
		hwavesIntVeto.push_back(NULL);
		hwavesExtVeto.push_back(NULL);
		hwavesCalo.push_back(NULL);
	}

	japp->RootUnLock();

	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t JEventProcessor_Catania::brun(JEventLoop *eventLoop, int32_t runnumber)
{
	// This is called whenever the run number changes

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
jerror_t JEventProcessor_Catania::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	vector<const CalorimeterSiPMHit*> mppchits;
	vector<const CalorimeterSiPMHit*>::iterator mppchits_it;

	vector<const CalorimeterHit*> chits;
	vector<const CalorimeterHit*>::iterator chits_it;

	vector<const IntVetoDigiHit*> ivhits;
	vector<const IntVetoDigiHit*>::iterator ivhits_it;

	vector<const ExtVetoDigiHit*> evhits;
	vector<const ExtVetoDigiHit*>::iterator evhits_it;


	vector<const PaddlesHit*> phits;
	vector<const PaddlesHit*>::iterator phits_it;



	vector <const fa250Mode1CalibPedSubHit*> cwaves;
	vector <const fa250Mode1CalibPedSubHit*>::iterator cwaves_it;

	vector <const fa250Mode1CalibPedSubHit*> ivwaves;
	vector <const fa250Mode1CalibPedSubHit*>::iterator ivwaves_it;

	vector <const fa250Mode1CalibHit*> evwaves;
	vector <const fa250Mode1CalibHit*>::iterator evwaves_it;


	vector<const CalorimeterMCHit*> mc_data;
	vector<const CataniaEvent*> events;

	const eventData* evdata=0;


	int sector,layer,component;
	int crate,slot,channel,channelTmp;
	int N;
	bool flag;

	double Q,Qtot,T;
	loop->Get(events);
	loop->Get(chits);
	loop->Get(phits);
	if (m_isMC==0){
		loop->Get(ivhits);
		loop->Get(evhits);
	}
	else{
		loop->Get(ivhits,"MC");
		loop->Get(evhits,"MC");
	}


	if (m_isMC==0){
		loop->Get(mppchits);
	}

	if (events.size()!=1){
		jout<<"Not a single event in this event "<<events.size()<<endl;
		return RESOURCE_UNAVAILABLE;
	}



	if (m_isMC==0){
		try{
			loop->GetSingle(evdata);
		}
		catch(unsigned long e){
			jout<<"No trig bank this event"<<endl;
			return NOERROR;
		}

		int tWord=evdata->triggerWords[0];
		if ((tWord&0x1)==0) return NOERROR;
	}

	if (chits.size()!=1){
		return NOERROR;
	}

	japp->RootWriteLock();

	event=events[0];
	flag=false;
	EcMC=0;
	if (m_isMC){
		caloHit->Get(mc_data); //use a vector since it is re-iterating!
		for (int imc=0;imc<mc_data.size();imc++){
			EcMC+=mc_data[imc]->totEdep;
		}
	}
	else{
		EcMC=-1;
	}


	flag=false;
	//if (1){
	//	if ((Ec1>10)&&(nHitsExtVeto==0)&&(nHitsIntVeto==0)&&(caloHit->m_data[0].good_ped_RMS==true)&&(caloHit->m_data[1].good_ped_RMS==true)) flag=true;
	//if ((Ec1>20)&&(Ec1<40)&&(nHitsIntVetoCoincidence==0)&&(nHitsIntVeto>0)&&(event->flag_RMS==true)) flag=true;
	if ((flag)&&(m_isMC==false)){
		//	jout<<"QUI "<<eventnumber<<endl;
		//	cin.get();
		int iwave=0;
		int N;
		for (chits_it=chits.begin();chits_it!=chits.end();chits_it++){
			cwaves.clear();
			(*chits_it)->Get(cwaves);

			for (cwaves_it=cwaves.begin();cwaves_it!=cwaves.end();cwaves_it++){
				hwave=hwavesCalo[iwave];
				N=(*cwaves_it)->samples.size();

				if (hwave==0){
					hwavesCalo[iwave]=new TH1D(Form("hCalo_%i",iwave),Form("hCalo_%i",iwave),N,-0.5,N-0.5);
					m_ROOTOutput->AddObject(hwavesCalo[iwave]);
				}
				hwave=hwavesCalo[iwave];
				hwave->Reset();
				hwave->SetName(Form("h_%i_%i__%f_%f_%f_%f",cwaves[iwave]->m_channel.channel,event->eventN,event->Ec1,event->Ec2,event->E,event->T));
				hwave->SetTitle(Form("h_%i_%i__%f_%f_%f_%f",cwaves[iwave]->m_channel.channel,event->eventN,event->Ec1,event->Ec2,event->E,event->T));

				for (int isample=0;isample<N;isample++){
					hwave->Fill(isample,(*cwaves_it)->samples[isample]);
				}
				hwave->Write();
				iwave++;
			}
		}

		iwave=0;
		for (ivhits_it=ivhits.begin();ivhits_it!=ivhits.end();ivhits_it++){
			ivwaves.clear();
			(*ivhits_it)->Get(ivwaves);
			for (ivwaves_it=ivwaves.begin();ivwaves_it!=ivwaves.end();ivwaves_it++){
				hwave=hwavesIntVeto[iwave];
				N=(*ivwaves_it)->samples.size();

				if (hwave==0){
					hwavesIntVeto[iwave]=new TH1D(Form("hIntVeto_%i",iwave),Form("hIntVeto_%i",iwave),N,-0.5,N-0.5);
					m_ROOTOutput->AddObject(hwavesIntVeto[iwave]);
				}
				T=(*ivhits_it)->T;
				Qtot=(*ivhits_it)->Qtot;
				slot=(*ivwaves_it)->m_channel.slot;
				channel=(*ivwaves_it)->m_channel.channel;
				Q=0;

				if (slot==7) Q=Qtot;
				if (slot==6){
					channelTmp=channel%4+1;
					for (int itmp=0;itmp<(*ivhits_it)->m_data.size();itmp++){
						if ((*ivhits_it)->m_data[itmp].readout==channelTmp){
							Q=(*ivhits_it)->m_data[itmp].Q;
							break;
						}
					}
				}
				hwave=hwavesIntVeto[iwave];
				hwave->Reset();
				hwave->SetName(Form("hIntVeto_%i_%i__%i__%f_%f_%f",slot,channel,event->eventN,Q,Qtot,T));
				hwave->SetTitle(Form("hIntVeto_%i_%i__%i__%f_%f_%f",slot,channel,event->eventN,Q,Qtot,T));

				for (int isample=0;isample<N;isample++){
					hwave->Fill(isample,(*ivwaves_it)->samples[isample]);
				}
				hwave->Write();
				iwave++;
			}
		}

		iwave=0;
		for (evhits_it=evhits.begin();evhits_it!=evhits.end();evhits_it++){
			evwaves.clear();
			(*evhits_it)->Get(evwaves);
			for (evwaves_it=evwaves.begin();evwaves_it!=evwaves.end();evwaves_it++){
				hwave=hwavesExtVeto[iwave];
				N=(*evwaves_it)->samples.size();

				if (hwave==0){
					hwavesExtVeto[iwave]=new TH1D(Form("hExtVeto_%i",iwave),Form("hExtVeto_%i",iwave),N,-0.5,N-0.5);
					m_ROOTOutput->AddObject(hwavesExtVeto[iwave]);
				}
				hwave=hwavesExtVeto[iwave];
				hwave->Reset();
				hwave->SetName(Form("hExtVeto_%i_%i__%i__%f_%f",(*evwaves_it)->m_channel.slot,(*evwaves_it)->m_channel.channel,event->eventN,(*evhits_it)->Q,(*evhits_it)->T));
				hwave->SetTitle(Form("hExtVeto_%i_%i__%i__%f_%f",(*evwaves_it)->m_channel.slot,(*evwaves_it)->m_channel.channel,event->eventN,(*evhits_it)->Q,(*evhits_it)->T));

				for (int isample=0;isample<N;isample++){
					hwave->Fill(isample,(*evwaves_it)->samples[isample]);
				}
				hwave->Write();
				iwave++;
			}
		}


	}/*end if flag*/


	t->Fill();
	japp->RootUnLock();


	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t JEventProcessor_Catania::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t JEventProcessor_Catania::fini(void)
{
	// Called before program exit after event processing is finished.
	return NOERROR;
}






