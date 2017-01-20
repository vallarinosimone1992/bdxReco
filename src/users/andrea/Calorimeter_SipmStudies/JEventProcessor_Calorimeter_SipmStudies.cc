// $Id$
//
//    File: JEventProcessor_Calorimeter_SipmStudies.cc
// Created: Mon Apr  4 15:27:17 CEST 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include "JEventProcessor_Calorimeter_SipmStudies.h"
using namespace jana;




#include <system/BDXEventProcessor.h>

#include <DAQ/fa250Mode1Hit.h>
#include <DAQ/fa250Mode1CalibPedSubHit.h>


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
	vhit=new vector<const CalorimeterSiPMHit*>;

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
	t->Branch("CalorimeterSipmHits",&vhit);


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


	loop->Get(cdata);



	japp->RootWriteLock();
	eventNumber=eventnumber;




	vhit->clear();
	for (cdata_it=cdata.begin();cdata_it!=cdata.end();cdata_it++){
		const CalorimeterSiPMHit *sipmhit= *cdata_it;
		vhit->push_back(sipmhit);
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

