// $Id$
//
//    File: JEventProcessor_test.cc
// Created: Mon Dec 28 15:48:21 CET 2015
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include <iostream>
#include <iomanip>
using namespace std;

#include "Paddles_basic_track.h"
#include "system/BDXEventProcessor.h"
#include <TT/TranslationTable.h>
#include <DAQ/eventData.h>
#include <DAQ/fa250Mode1CalibHit.h>


#include <Paddles/PaddlesHit.h>
#include <ExtVeto/ExtVetoPMTHit.h>


#include <system/JROOTOutput.h>
#include "TTree.h"

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>

using namespace jana;

extern "C"{
void InitPlugin(JApplication *app){
	InitJANAPlugin(app);
	app->AddProcessor(new Paddles_basic_track());
}
} // "C"


//------------------
//  (Constructor)
//------------------
Paddles_basic_track::Paddles_basic_track():m_isFirstCallToBrun(1),m_ROOTOutput(0)
{

}

//------------------
//  (Destructor)
//------------------
Paddles_basic_track::~Paddles_basic_track()
{

}
//------------------
// init
//------------------
jerror_t Paddles_basic_track::init(void)
{
	// This is called once at program startup. If you are creating
	// and filling historgrams in this plugin, you should lock the
	// ROOT mutex like this:
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();
	//

	app->RootWriteLock();

	t=new TTree("t","t");

	t->Branch("id",&id);
	t->Branch("component",&component);
	t->Branch("E_up",&E_up);
	t->Branch("E_down",&E_down);
	t->Branch("T_up",&T_up);
	t->Branch("T_down",&T_down);
	t->Branch("E6",&E6);
	t->Branch("E9",&E9);
	t->Branch("T6",&T6);
	t->Branch("T9",&T9);
	t->Branch("eventN",&eventN);
	t->Branch("tword",&tword);
	t->Branch("mult",&mult);

	t->Branch("amp",amp,"amp[100]/D");
	t->Branch("time",time,"time[100]/I");


	app->RootUnLock();
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t Paddles_basic_track::brun(JEventLoop *loop, int32_t runnumber)
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
			m_ROOTOutput->AddObject(t);
		}

	}

	return NOERROR;


}

//------------------
// evnt
//------------------
jerror_t Paddles_basic_track::evnt(JEventLoop *loop,uint64_t eventnumber)
{
	// This is called for every event. Use of common resources like writing
	// to a file or filling a histogram should be mutex protected. Using
	// loop->Get(...) to get reconstructed objects (and thereby activating the
	// reconstruction algorithm) should be done outside of any mutex lock
	// since multiple threads may call this method at the same time.
	// Here's an example:
	//
	vector<const PaddlesHit*> data;
	vector<const PaddlesHit*>::const_iterator data_it;

	vector <const fa250Mode1CalibHit*> faV;

	loop->Get(data);

	const eventData* tData;
	try{
		loop->GetSingle(tData);
	}
	catch(unsigned long e){
		jout<<"No trig bank this event"<<endl;
		return 	OBJECT_NOT_AVAILABLE;
	}

	int tWord=tData->triggerWords.at(0);



	japp->RootWriteLock();

	E_up=0;
	E_down=0;
	T_up=0;
	T_down=0;

	double E[2];
	double T[2];

	for(int i=0;i<2;i++){
		E[i]=0;
		T[i]=0;
	}

	for (data_it=data.begin();data_it<data.end();data_it++){

		const PaddlesHit *evhit = *data_it;
//		evhit->Get(faV);
//		const fa250Mode1CalibHit *fa=faV[0];
//						for (int ii=0;ii<fa->samples.size();ii++){
//												amp[ii]=0;
//												time[ii]=0;		// in nsec
//												}


				eventN=eventnumber;
				tword=tWord;
				id=evhit->m_channel.id;

				if(evhit->m_channel.id==0){
					E[0]=(*data_it)->E;
					T[0]=(*data_it)->T;
				}
				if(evhit->m_channel.id==1){
									E[1]=(*data_it)->E;
									T[1]=(*data_it)->T;
								}
//			jout<<"Nevent= "<<eventN<<" id= "<<id<<" E_down= "<<E[0]<<" E_up= "<<E[1]<<" T_down= "<<T[0]<<" T_up="<<T[1]<<endl;

//				if (faV.size()!=1){
//				    jout<<"Not associated object or more than 1 fa250Mode1CalibHit associated with this object"<<endl;
//				}
//				else{
//				    const fa250Mode1CalibHit *fa=faV[0];
//				}


//				if (id==0) {
//				for (int ii=0;ii<fa->samples.size();ii++){
//										amp[ii]=fa->samples.at(ii);
//										time[ii]=ii*4;		// in nsec
//										}
//				}

	}

	E_down=E[0];
	E_up=E[1];
	T_down=T[0];
	T_up=T[1];


	for (data_it=data.begin();data_it<data.end();data_it++){			// loop paddles up - down
				const PaddlesHit *evhit = *data_it;
				evhit->Get(faV);
				const fa250Mode1CalibHit *fa=faV[0];

						if(evhit->m_channel.id==0){
						for (int ii=0;ii<fa->samples.size();ii++){
												amp[ii]=fa->samples.at(ii);
												time[ii]=ii*4;		// in nsec
												}
						}
			}



	vector<const ExtVetoPMTHit*> data_extveto;
	vector<const ExtVetoPMTHit*>::const_iterator data_extveto_it;
	loop->Get(data_extveto);


		E6=0;
		E9=0;
		T6=0;
		T9=0;

		for(int i=0;i<2;i++){
			E[i]=0;
			T[i]=0;
		}

		for (data_extveto_it=data_extveto.begin();data_extveto_it<data_extveto.end();data_extveto_it++){
			const ExtVetoPMTHit *evhit = *data_extveto_it;


					component=evhit->m_channel.ext_veto.component;

					if(evhit->m_channel.ext_veto.component==6){

						E[0]=(*data_extveto_it)->Q;
						T[0]=(*data_extveto_it)->T;
					}
					if(evhit->m_channel.ext_veto.component==9){
										E[1]=(*data_extveto_it)->Q;
										T[1]=(*data_extveto_it)->T;
									}
	//			jout<<"Nevent= "<<eventN<<" id= "<<id<<" E_down= "<<E[0]<<" E_up= "<<E[1]<<" T_down= "<<T[0]<<" T_up="<<T[1]<<endl;

		}
		E6=E[0];
		E9=E[1];
		T6=T[0];
		T9=T[1];

//		cout<<"E6= "<<E6<<"E9= "<<E9<<endl;


	//			jout<<"Nevent= "<<eventN<<" id= "<<id<<" E_down= "<<E_down<<" E_up= "<<E_up<<" T_down= "<<T_down<<" T_up="<<T_up<<endl;
	//			jout<<"********************"<<endl;



			t->Fill();


	japp->RootUnLock();


	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t Paddles_basic_track::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t Paddles_basic_track::fini(void)
{

	return NOERROR;
}

