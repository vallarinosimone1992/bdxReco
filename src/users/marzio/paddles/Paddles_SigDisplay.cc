// $Id$
//
//    File: JEventProcessor_test.cc
// Created: Mon Dec 28 15:48:21 CET 2015
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include <iostream>
#include <iomanip>
using namespace std;

#include "Paddles_SigDisplay.h"
#include "system/BDXEventProcessor.h"

#include <DAQ/fa250Mode1CalibHit.h>

#include <TT/TranslationTable.h>

#include <DAQ/triggerData.h>

#include <Paddles/PaddlesPMTHit.h>
#include <Paddles/PaddlesDigiHit.h>


#include <system/JROOTOutput.h>

//#include "TH1D.h"
//#include "TH2D.h"
#include "TTree.h"

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>

using namespace jana;



extern "C"{
void InitPlugin(JApplication *app){
	InitJANAPlugin(app);
	app->AddProcessor(new Paddles_SigDisplay());
}
} // "C"


//------------------
// Paddles_SigDisplay (Constructor)
//------------------
Paddles_SigDisplay::Paddles_SigDisplay():m_isFirstCallToBrun(1)
{

}

//------------------
// ~Paddles_SigDisplay (Destructor)
//------------------
Paddles_SigDisplay::~Paddles_SigDisplay()
{

}
//------------------
// init
//------------------
jerror_t Paddles_SigDisplay::init(void)
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

//	jout<<"test::init is called"<<std::endl;
//	h=new TH1D("h","h",100,-0.5,99.5);
	t=new TTree("t","t");

	t->Branch("component",&component);
	t->Branch("Q",&Q);
	t->Branch("eventN",&eventN);
	t->Branch("tword",&tword);

	t->Branch("Q_first",&Q_first);
	t->Branch("Q_after",&Q_after);
	t->Branch("Q_evtbefore",&Q_evtbefore);
	t->Branch("Q0",&Q0);
	t->Branch("Q1",&Q1);



	t->Branch("ped_first",&ped_first);
	t->Branch("ped_after",&ped_after);


	t->Branch("amp",amp,"amp[100]/D");
	t->Branch("time",time,"time[100]/I");

	t->Branch("T0",&T0);
	t->Branch("T1",&T1);
	t->Branch("T0_twc",&T0_twc);
	t->Branch("T1_twc",&T1_twc);



	app->RootUnLock();
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t Paddles_SigDisplay::brun(JEventLoop *loop, int32_t runnumber)
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
jerror_t Paddles_SigDisplay::evnt(JEventLoop *loop,uint64_t eventnumber)
{
	// This is called for every event. Use of common resources like writing
	// to a file or filling a histogram should be mutex protected. Using
	// loop->Get(...) to get reconstructed objects (and thereby activating the
	// reconstruction algorithm) should be done outside of any mutex lock
	// since multiple threads may call this method at the same time.
	// Here's an example:
	//
	vector<const PaddlesPMTHit*> data;
	vector<const PaddlesPMTHit*>::const_iterator data_it;
	const fa250Mode1CalibHit *fa;
	loop->Get(data);

	const double thr_0=90; // in mV
	const double thr_1=104;

	const triggerData* tData;
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
//	jout<<"Evt number="<< eventnumber<<" tWord= "<<tWord<<endl;
	japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	double ped_evtbefore=100;

	for (data_it=data.begin();data_it<data.end();data_it++){

		const PaddlesPMTHit *evhit = *data_it;

			// Get associated fa250Mode1CalibHit object
			fa = NULL;
			evhit->GetSingle(fa);

			if(!fa) continue; // need fa250Mode1CalibHit to continue

				eventN=eventnumber;
				tword=tWord;
				component=evhit->m_channel.paddles.id;


				if(tword==2&&eventN==445){

				Q=(*data_it)->Q;
				Q_first=0;
				Q_after=0;
				Q_evtbefore=0;

				T0=0;
				T0_twc=0;
				T1=0;
				T1_twc=0;

				Q0=0;
				Q1=0;

				double max=0;
				if(eventN!=1)ped_evtbefore=ped_first;

//				jout<<"*************************** "<<eventN<<endl;
//				jout<<ped_first<<" "<<ped_after<<" "<<ped_evtbefore<<endl;
//				jout<<"Component= "<<component<<endl;
				ped_first=0;
				ped_after=0;

				double thr_0_twc=0;

				for (int ii=0;ii<fa->samples.size();ii++){

					    if(ii<30)ped_first+=fa->samples.at(ii);
					    if(ii>80)ped_after+=fa->samples.at(ii);
						amp[ii]=fa->samples.at(ii);
						time[ii]=ii*4;		// in nsec
//						jout<<ped_first<<endl;
						 if(amp[ii] > max) max = amp[ii];
						}

					ped_first=ped_first/30;
					ped_after=ped_after/20;

//					jout<<"Max= "<<max<<endl;

//				jout<<"PED= "<<ped_first<<" "<<ped_after<<" "<<ped_evtbefore<<endl;

					for (int ii=0;ii<fa->samples.size();ii++){
										    Q_first+=(fa->samples.at(ii)-ped_first);
										    Q_after+=(fa->samples.at(ii)-ped_after);
										    Q_evtbefore+=(fa->samples.at(ii)-ped_evtbefore);
//										    cout<<ii<<" "<<(fa->samples.at(ii)-ped_first)<<endl;

											}

//					jout<<"Q= "<<Q_first<<" "<<Q_after<<" "<<Q_evtbefore<<endl;
//					jout<<"Q_first= "<<Q_first<<endl;

					//					jout<<(0.1*(max-ped_first))<<endl;
					if(component==0){
						Q0=Q_first;
						thr_0_twc=ped_first+(0.1*(max-ped_first));
//						jout<<thr_0_twc<<endl;
						for (int ii=0;ii<fa->samples.size();ii++){
										if (fa->samples.at(ii)>thr_0){
										 double Tinf=(ii-1)*4;
										 double Tsup=ii*4;
										 T0=Tinf+((Tsup-Tinf)/2);
										 break;
										}
									}
						for (int ii=0;ii<fa->samples.size();ii++){
																 if (fa->samples.at(ii)>thr_0_twc){
																 double Tinf=(ii-1)*4;
																 double Tsup=ii*4;
																 T0_twc=Tinf+((Tsup-Tinf)/2);
																 break;
																}
															}
					}


//					jout<<"T0= "<<T0<<"T0_twc="<<T0_twc<<endl;




					if(component==1){
						Q1=Q_first;

											for (int ii=0;ii<fa->samples.size();ii++){
															if (fa->samples.at(ii)>thr_1){
															 double Tinf=(ii-1)*4;
															 double Tsup=ii*4;
															 T1=Tinf+((Tsup-Tinf)/2);
															 break;
															}
														}

										}
					if(T0==0)Q0=0;
					if(T1==0)Q1=0;
//					jout<<"T0= "<<T0<<" T1= "<<T1<<endl;
//					jout<<"Q0= "<<Q0<<" Q1= "<<Q1<<endl;

			t->Fill();
			}
	}

	japp->RootUnLock();




	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t Paddles_SigDisplay::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t Paddles_SigDisplay::fini(void)
{

	return NOERROR;
}

