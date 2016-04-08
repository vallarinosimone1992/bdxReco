// $Id$
//
//    File: JEventProcessor_Calorimeter_rate.cc
// Created: Fri Mar 25 18:38:03 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include "JEventProcessor_Calorimeter_rate.h"
#include <system/BDXEventProcessor.h>

#include <DAQ/eventData.h>
#include <DAQ/fa250Mode1CalibPedSubHit.h>
#include <TT/TranslationTable.h>


#include <Calorimeter/CalorimeterHit.h>
#include <IntVeto/IntVetoSiPMHit.h>
#include <IntVeto/IntVetoHit.h>
#include <IntVeto/IntVetoDigiHit.h>
#include <IntVeto/IntVetoSummary.h>
#include <ExtVeto/ExtVetoPMTHit.h>
#include <ExtVeto/ExtVetoDigiHit.h>
#include <ExtVeto/ExtVetoHit.h>
#include <ExtVeto/ExtVetoSummary.h>
#include <DAQ/fa250Mode1CalibHit.h>

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
	app->AddProcessor(new JEventProcessor_Calorimeter_rate());
}
} // "C"


//------------------
// JEventProcessor_Calorimeter_rate (Constructor)
//------------------
JEventProcessor_Calorimeter_rate::JEventProcessor_Calorimeter_rate()
{

}

//------------------
// ~JEventProcessor_Calorimeter_rate (Destructor)
//------------------
JEventProcessor_Calorimeter_rate::~JEventProcessor_Calorimeter_rate()
{

}

//------------------
// init
//------------------
jerror_t JEventProcessor_Calorimeter_rate::init(void)
{
	// This is called once at program startup. If you are creating
	// and filling historgrams in this plugin, you should lock the
	// ROOT mutex like this:
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();
	//
	japp->RootWriteLock();
	t=new TTree("CaloRate","CaloRate");
	t->Branch("eventN",&eventNumber);
	t->Branch("Ec",&Ec);
	t->Branch("Ec1",&Ec1);
	t->Branch("Ec2",&Ec2);
	t->Branch("nHitsIntVeto",&nHitsIntVeto);
	t->Branch("nHitsExtVeto",&nHitsExtVeto);

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
jerror_t JEventProcessor_Calorimeter_rate::brun(JEventLoop *eventLoop, int32_t runnumber)
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
jerror_t JEventProcessor_Calorimeter_rate::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	vector<const CalorimeterHit*> chits;
	vector<const CalorimeterHit*>::iterator chits_it;

	vector<const IntVetoDigiHit*> ivhits;
	vector<const IntVetoDigiHit*>::iterator ivhits_it;

	vector<const ExtVetoDigiHit*> evhits;
	vector<const ExtVetoDigiHit*>::iterator evhits_it;


	vector <const IntVetoSummary*> IntVetoSum;
	vector <const IntVetoSummary*>::iterator IntVetoSum_it;

	vector <const ExtVetoSummary*> ExtVetoSum;
	vector <const ExtVetoSummary*>::iterator ExtVetoSum_it;

	vector <const fa250Mode1CalibPedSubHit*> cwaves;
	vector <const fa250Mode1CalibPedSubHit*>::iterator cwaves_it;

	vector <const fa250Mode1CalibPedSubHit*> ivwaves;
	vector <const fa250Mode1CalibPedSubHit*>::iterator ivwaves_it;

	vector <const fa250Mode1CalibHit*> evwaves;
	vector <const fa250Mode1CalibHit*>::iterator evwaves_it;


	const eventData* evdata;

	const IntVetoSummary* IntVetoSum0;
	const ExtVetoSummary* ExtVetoSum0;

	int sector,layer,component;
	int crate,slot,channel,channelTmp;
	int N;
	bool flag;

	double Q,Qtot;
	loop->Get(chits);
	loop->Get(ivhits);
	loop->Get(evhits);

	loop->Get(IntVetoSum);
	loop->Get(ExtVetoSum);

	try{
		loop->GetSingle(evdata);
	}
	catch(unsigned long e){
		jout<<"No trig bank this event"<<endl;
		return NOERROR;
	}

	int tWord=evdata->triggerWords[0];
	if ((tWord&0x1)==0) return NOERROR;
	if (chits.size()!=1) return NOERROR;


	japp->RootWriteLock();


	flag=false;
	nHitsIntVeto=0;
	nHitsExtVeto=0;

	eventNumber=eventnumber;

	if (IntVetoSum.size()==1){
		IntVetoSum0=IntVetoSum[0];
		nHitsIntVeto=IntVetoSum0->nHits;
	}
	else{
		nHitsIntVeto=0;
	}
	if (ExtVetoSum.size()==1){
		ExtVetoSum0=ExtVetoSum[0];
		nHitsExtVeto=ExtVetoSum0->nHits;
	}
	else{
		nHitsExtVeto=0;
	}
	Ec=chits[0]->E;
	for (int ihit=0;ihit<chits[0]->m_data.size();ihit++){
		switch (chits[0]->m_data[ihit].readout){
		case (1):
												Ec1=chits[0]->m_data[ihit].E;
		break;
		case (2):
												Ec2=chits[0]->m_data[ihit].E;
		break;
		}
	}


	if ((Ec1>10)&&(nHitsExtVeto==0)&&(nHitsIntVeto==0)) flag=true;

	if (flag){
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
				hwave->SetName(Form("h_%i_%i__%i__%f",cwaves[iwave]->m_channel.slot,cwaves[iwave]->m_channel.channel,eventnumber,Ec1));
				hwave->SetTitle(Form("h_%i_%i__%i__%f",cwaves[iwave]->m_channel.slot,cwaves[iwave]->m_channel.channel,eventnumber,Ec1));

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
				hwave->SetName(Form("hIntVeto_%i_%i__%i__%f_%f",slot,channel,eventnumber,Qtot,Q));
				hwave->SetTitle(Form("hIntVeto_%i_%i__%i__%f_%f",slot,channel,eventnumber,Qtot,Q));

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
				hwave->SetName(Form("hExtVeto_%i_%i__%i__%f",(*evwaves_it)->m_channel.slot,(*evwaves_it)->m_channel.channel,eventnumber,(*evhits_it)->Q));
				hwave->SetTitle(Form("hExtVeto_%i_%i__%i__%f",(*evwaves_it)->m_channel.slot,(*evwaves_it)->m_channel.channel,eventnumber,(*evhits_it)->Q));

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
jerror_t JEventProcessor_Calorimeter_rate::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t JEventProcessor_Calorimeter_rate::fini(void)
{
	// Called before program exit after event processing is finished.
	return NOERROR;
}






