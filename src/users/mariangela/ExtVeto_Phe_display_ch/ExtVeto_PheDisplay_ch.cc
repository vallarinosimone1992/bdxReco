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

#include "ExtVeto_PheDisplay_ch.h"
#include "system/BDXEventProcessor.h"

#include <DAQ/fa250Mode1CalibHit.h>

#include <TT/TranslationTable.h>

#include <DAQ/eventData.h>

#include <ExtVeto/ExtVetoPMTHit.h>
#include <ExtVeto/ExtVetoDigiHit.h>

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
	app->AddProcessor(new ExtVeto_PheDisplay_ch());
}
} // "C"


//------------------
// ExtVeto_SigDisplay (Constructor)
//------------------
ExtVeto_PheDisplay_ch::ExtVeto_PheDisplay_ch():m_isFirstCallToBrun(1),m_ROOTOutput(0)
{

}

//------------------
// ~ExtVeto_SigDisplay (Destructor)
//------------------
ExtVeto_PheDisplay_ch::~ExtVeto_PheDisplay_ch()
{

}
//------------------
// init
//------------------
jerror_t ExtVeto_PheDisplay_ch::init(void)
{

	app->RootWriteLock();

	jout<<"test::init is called"<<std::endl;

	t=new TTree("tout","tout");

	t->Branch("component",&component);
	t->Branch("Q",&Q);
	t->Branch("Q0",&Q0);
    t->Branch("Q1",&Q1);
    t->Branch("Q2",&Q2);
    t->Branch("Q3",&Q3);
    t->Branch("Q4",&Q4);
    t->Branch("Q5",&Q5);
    t->Branch("Q6",&Q6);
    t->Branch("Q7",&Q7);
    t->Branch("Q8",&Q8);
    t->Branch("Q9",&Q9);
    t->Branch("Q10",&Q10);
    t->Branch("Q11",&Q11);

 //   t->Branch("phe",&phe);
 //   t->Branch("pedestal",&pedestal);
    t->Branch("T0",&T0);
    t->Branch("T1",&T1);
 	t->Branch("T2",&T2);
 	t->Branch("T3",&T3);
 	t->Branch("T4",&T4);
 	t->Branch("T5",&T5);
 	t->Branch("T6",&T6);
 	t->Branch("T7",&T7);
 	t->Branch("T8",&T8);
 	t->Branch("T9",&T9);
 	t->Branch("T10",&T10);
 	t->Branch("T11",&T11);










	t->Branch("eventN",&eventN);
	t->Branch("Tw",&Tw);
    t->Branch("A",A, "A[100]/D");
 //   t->Branch("B",B, "B[100]/D");
    t->Branch("time",time,"time[100]/I");


    //  t->Branch("h_wave",&h_wave);

	app->RootUnLock();
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t ExtVeto_PheDisplay_ch::brun(JEventLoop *loop, int32_t runnumber)
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
jerror_t ExtVeto_PheDisplay_ch::evnt(JEventLoop *loop,uint64_t eventnumber)
{

	vector<const ExtVetoPMTHit*> data;
	vector<const ExtVetoPMTHit*>::const_iterator data_it;
	const fa250Mode1CalibHit *fa;
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


//	jout<<"****************************************************************"<<endl;
//	jout<<"Evt number="<< eventnumber<<endl;

	japp->RootWriteLock();

	for (data_it=data.begin();data_it<data.end();data_it++){



		const ExtVetoPMTHit *evhit = *data_it;


			// Get associated fa250Mode1CalibHit object
			fa = NULL;
			evhit->GetSingle(fa);


			if(!fa) continue; // need fa250Mode1CalibHit to continue


			eventN=eventnumber;
			component=(*data_it)->m_channel.ext_veto->component;

			if (component==0){
							Q0=(*data_it)->Q*2;
							T0 = (*data_it)->T;
						}

			if (component==1){
				Q1=(*data_it)->Q*2;
				T1 = (*data_it)->T;
		                 	}
			if (component==2){
				Q2=(*data_it)->Q*2;
				T2 = (*data_it)->T;
		                 	}
			if (component==3){
				Q3=(*data_it)->Q*2;
				T3 = (*data_it)->T;
		                 	}
			if (component==4){
				Q4=(*data_it)->Q*2;
				T4 = (*data_it)->T;
		                 	}
			if (component==5){
				Q5=(*data_it)->Q*2;
				T5 = (*data_it)->T;
		                 	}
			if (component==6){
				Q6=(*data_it)->Q*2;
				T6 = (*data_it)->T;
		                 	}
			if (component==7){
				Q7=(*data_it)->Q*2;
				T7 = (*data_it)->T;
		                 	}
			if (component==8){
				Q8=(*data_it)->Q*2;
				T8 = (*data_it)->T;
		                    }
			if (component==9){
				Q9=(*data_it)->Q*2;
				T9 = (*data_it)->T;
		                 	}
			if (component==10){
				Q10=(*data_it)->Q*2;
				T10 = (*data_it)->T;
		                 	}
			if (component==11){
				Q11=(*data_it)->Q*2;
				T11= (*data_it)->T;
		                 	}
			Q=(*data_it)->Q;
			Tw=tData->triggerWords.at(0);
		//	T = (*data_it)->T;


			for (int ii=0;ii<fa->samples.size();ii++){
                 A[ii]=(fa->samples.at(ii));
                 time[ii] = ii;
             }


			t->Fill();



}

	japp->RootUnLock();

//	downthres:


	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t ExtVeto_PheDisplay_ch::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t ExtVeto_PheDisplay_ch::fini(void)
{

	return NOERROR;
}





