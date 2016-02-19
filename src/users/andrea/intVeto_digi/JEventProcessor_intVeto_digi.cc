// $Id$
//
//    File: JEventProcessor_intVeto_digi.cc
// Created: Tue Feb  9 16:51:03 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include "JEventProcessor_intVeto_digi.h"
using namespace jana;

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>



#include "JEventProcessor_test.h"
#include "system/BDXEventProcessor.h"

#include <DAQ/fa250Mode1Hit.h>

#include <TT/TranslationTable.h>

#include <DAQ/triggerData.h>

#include <IntVeto/IntVetoDigiHit.h>
#include <IntVeto/IntVetoSiPMHit.h>

#include <ExtVeto/ExtVetoPMTHit.h>
#include <ExtVeto/ExtVetoDigiHit.h>


#include <system/JROOTOutput.h>

#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"





extern "C"{
void InitPlugin(JApplication *app){
	InitJANAPlugin(app);
	app->AddProcessor(new JEventProcessor_intVeto_digi());
}
} // "C"


//------------------
// JEventProcessor_intVeto_digi (Constructor)
//------------------
JEventProcessor_intVeto_digi::JEventProcessor_intVeto_digi()
{

}

//------------------
// ~JEventProcessor_intVeto_digi (Destructor)
//------------------
JEventProcessor_intVeto_digi::~JEventProcessor_intVeto_digi()
{

}

//------------------
// init
//------------------
jerror_t JEventProcessor_intVeto_digi::init(void)
{

	//
		japp->RootWriteLock();
		t=new TTree("tout","tout");

		t->Branch("sector",&m_sector);
		t->Branch("layer",&m_layer);
		t->Branch("component",&m_component);
		t->Branch("readout",&m_readout);
		t->Branch("Q",&Q);
		t->Branch("Q1",&Q1);
		t->Branch("Q2",&Q2);
		t->Branch("Q3",&Q3);
		t->Branch("Q4",&Q4);

		japp->RootUnLock();


	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t JEventProcessor_intVeto_digi::brun(JEventLoop *eventLoop, int32_t runnumber)
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
jerror_t JEventProcessor_intVeto_digi::evnt(JEventLoop *loop, uint64_t eventnumber)
{
	vector<const IntVetoDigiHit*> data;
	vector<const IntVetoSiPMHit*> associated_data;
	vector<const IntVetoDigiHit*>::const_iterator data_it;
	loop->Get(data);
	japp->RootWriteLock();

	for (data_it=data.begin();data_it<data.end();data_it++){
		m_sector=(*data_it)->m_channel.sector;
		m_layer=(*data_it)->m_channel.layer;
		m_component=(*data_it)->m_channel.component;
		m_readout=(*data_it)->m_channel.readout;

		Q=(*data_it)->Q;
		(*data_it)->Get(associated_data,"",1);
		if (m_component<=3){
			Q1=associated_data.at(0)->Qphe;
			Q2=associated_data.at(1)->Qphe;
			Q3=associated_data.at(2)->Qphe;
			Q4=associated_data.at(3)->Qphe;
		}
		t->Fill();
	}


	japp->RootUnLock();

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t JEventProcessor_intVeto_digi::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t JEventProcessor_intVeto_digi::fini(void)
{
	// Called before program exit after event processing is finished.
	return NOERROR;
}

