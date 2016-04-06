/*
 /*
 * ExtVeto_PheDisplay.cc
 *
 *  Created on: 18 feb 2016
 *      Author: Mariangela
 */
#include <iostream>
#include <iomanip>
using namespace std;

#include "ExtVeto_summary.h"
#include "system/BDXEventProcessor.h"
#include <DAQ/eventData.h>
#include <DAQ/fa250Mode1CalibHit.h>

#include <TT/TranslationTable.h>



#include <ExtVeto/ExtVetoPMTHit.h>
#include <ExtVeto/ExtVetoDigiHit.h>
#include <ExtVeto/ExtVetoHit.h>
#include <ExtVeto/ExtVetoSummary.h>


#include <system/JROOTOutput.h>

#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>

using namespace jana;



extern "C"{
void InitPlugin(JApplication *app){
	InitJANAPlugin(app);
	app->AddProcessor(new ExtVeto_summary());
}
} // "C"


//------------------
// ExtVeto_SigDisplay (Constructor)
//------------------
ExtVeto_summary::ExtVeto_summary():m_isFirstCallToBrun(1)
{

}

//------------------
// ~ExtVeto_SigDisplay (Destructor)
//------------------
ExtVeto_summary::~ExtVeto_summary()
{

}
//------------------
// init
//------------------
jerror_t ExtVeto_summary::init(void)
{

	app->RootWriteLock();

	jout<<"test::init is called"<<std::endl;

	t=new TTree("tout","tout");

//	t->Branch("component",&component);
//	t->Branch("E",&E);
//	t->Branch("T",&T);
 	t->Branch("sum",&sum);
	t->Branch("eventN",&eventN);
	t->Branch("Tw",&Tw);
    t->Branch("sum_chan",sum_chan,"sum_chan[12]/I");

//    t->Branch("A",A, "A[100]/D");
//   t->Branch("time",time,"time[100]/I");



	app->RootUnLock();
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t ExtVeto_summary::brun(JEventLoop *loop, int32_t runnumber)
{


	// This is called whenever the run number changes
	/* To get access to the ROOT output, use exactly the following code */
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
//			m_ROOTOutput->AddObject(h);
			m_ROOTOutput->AddObject(t);
		}

	}

	return NOERROR;


}

//------------------
// evnt
//------------------
jerror_t ExtVeto_summary::evnt(JEventLoop *loop,uint64_t eventnumber)
{

	vector<const ExtVetoSummary*> data;


	vector<const ExtVetoSummary*>::const_iterator data_it;

	vector <const fa250Mode1CalibHit*> faV;


	loop->Get(data);

	const eventData* tData;
	//has to be in a try-catch block, since if no trigger data is there (prestart - start - end events) trows it!
	try{
		loop->GetSingle(tData);
	}
	catch(unsigned long e){
		jout<<"No trig bank this event"<<endl;
		return 	OBJECT_NOT_AVAILABLE;
	}

	int tWord=tData->triggerWords.at(0);

	japp->RootWriteLock();


//	jout << "start_event"<<std::endl;

	for (data_it=data.begin();data_it<data.end();data_it++){

		const ExtVetoSummary *evhit = *data_it;

		eventN=eventnumber;

			Tw=tData->triggerWords.at(0);
            sum = (*data_it)->nHits;

   //         jout << sum << endl;

     //      jout << "inizio chan"<<endl;

            for (int i=0;i<12;i++){

            	if(i<(*data_it)->hits.size()){
            	sum_chan[i] = (*data_it)->hits.at(i).component;
            	}else{
            	sum_chan[i] = 999;
            	}
      //      	jout << sum_chan[i]<<endl;

            }
      //     jout <<"nHits= "<< sum <<std::endl;

       /*

							evhit->Get(faV);
							const fa250Mode1CalibHit *fa=faV[0];

									for (int ii=0;ii<fa->samples.size();ii++){
															A[ii]=fa->samples.at(ii);
															time[ii]=ii*4;		// in nsec
														    	}
*/


			t->Fill();




}

	japp->RootUnLock();

//	downthres:


	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t ExtVeto_summary::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t ExtVeto_summary::fini(void)
{

	return NOERROR;
}





