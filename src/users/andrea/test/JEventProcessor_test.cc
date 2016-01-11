// $Id$
//
//    File: JEventProcessor_test.cc
// Created: Mon Dec 28 15:48:21 CET 2015
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include "JEventProcessor_test.h"
#include "fa250Mode1Hit.h"

#include "TApplication.h"
#include "TH1D.h"
#include "TCanvas.h"

using namespace jana;


// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>
extern "C"{
void InitPlugin(JApplication *app){
	InitJANAPlugin(app);
	app->AddProcessor(new JEventProcessor_test());
}
} // "C"


//------------------
// JEventProcessor_test (Constructor)
//------------------
JEventProcessor_test::JEventProcessor_test()
{

}

//------------------
// ~JEventProcessor_test (Destructor)
//------------------
JEventProcessor_test::~JEventProcessor_test()
{

}

//------------------
// init
//------------------
jerror_t JEventProcessor_test::init(void)
{
	// This is called once at program startup. If you are creating
	// and filling historgrams in this plugin, you should lock the
	// ROOT mutex like this:
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();
	//
	jout<<"test::init is called"<<std::endl;
	app->RootWriteLock();

	vector<JEventProcessor*>::iterator it;
	it=app->GetProcessors().begin();


	for (it=app->GetProcessors().begin();it<app->GetProcessors().end();it++){

		if ((*it)!= NULL) jout<<"here: "<<(*it)->className()<<std::endl;
	}

	h=new TH1D("h","h",400,-0.5,399.5);
	app->RootUnLock();
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t JEventProcessor_test::brun(JEventLoop *eventLoop, int runnumber)
{
	// This is called whenever the run number changes
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t JEventProcessor_test::evnt(JEventLoop *loop, int eventnumber)
{
	// This is called for every event. Use of common resources like writing
	// to a file or filling a histogram should be mutex protected. Using
	// loop->Get(...) to get reconstructed objects (and thereby activating the
	// reconstruction algorithm) should be done outside of any mutex lock
	// since multiple threads may call this method at the same time.
	// Here's an example:
	//
	vector<const fa250Mode1Hit*> data;
	vector<const fa250Mode1Hit*>::const_iterator data_it;
	loop->Get(data);

	 japp->RootWriteLock();
	//  ... fill historgrams or trees ...
		for (data_it=data.begin();data_it<data.end();data_it++){
			if (((*data_it)->slot==4)&&((*data_it)->channel==3)){
				for (int ii=0;ii<(*data_it)->samples.size();ii++){
					h->Fill(ii,(*data_it)->samples.at(ii));
				}
			}
		}

	 japp->RootUnLock();


	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t JEventProcessor_test::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t JEventProcessor_test::fini(void)
{
	// Called before program exit after event processing is finished.
	TCanvas *c=new TCanvas("c","c");
	h->Draw();
//	TApplication gui("gui",0,NULL);
	c->Print("out.ps");
//	gui.Run(1);
	return NOERROR;
}

