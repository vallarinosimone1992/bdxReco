// $Id$
//
//    File: JEventProcessor_MCtest.cc
// Created: Mon Mar  7 11:23:10 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include "MCtest.h"
//#include <system/BDXEventProcessor.h>
#include <MC/ExtVetoMCHit.h>

#include <ExtVeto/ExtVetoDigiHit.h>
#include <ExtVeto/ExtVetoHit.h>
#include <ExtVeto/ExtVetoSummary.h>

#include <MC/CalorimeterMCHit.h>

#include <Calorimeter/CalorimeterDigiHit.h>
#include <Calorimeter/CalorimeterHit.h>



using namespace jana;

using namespace std;

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>

extern "C"{
void InitPlugin(JApplication *app){
	InitJANAPlugin(app);
	app->AddProcessor(new MCtest());
}
} // "C"


//------------------
// JEventProcessor_MCtest (Constructor)
//------------------
MCtest::MCtest()
{

}

//------------------
// ~JEventProcessor_MCtest (Destructor)
//------------------
MCtest::~MCtest()
{

}

//------------------
// init
//------------------
jerror_t MCtest::init(void)
{
	// This is called once at program startup. If you are creating
	// and filling historgrams in this plugin, you should lock the
	// ROOT mutex like this:
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();
	//



	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t MCtest::brun(JEventLoop *eventLoop, int32_t runnumber)
{
	// This is called whenever the run number changes
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t MCtest::evnt(JEventLoop *loop, uint64_t eventnumber)
{
	// This is called for every event. Use of common resources like writing
	// to a file or filling a histogram should be mutex protected. Using
	// loop->Get(...) to get reconstructed objects (and thereby activating the
	// reconstruction algorithm) should be done outside of any mutex lock
	// since multiple threads may call this method at the same time.
	// Here's an example:
	//
	// vector<const MyDataClass*> mydataclasses;
	// loop->Get(mydataclasses);
	//
	// japp->RootWriteLock();
	//  ... fill historgrams or trees ...
	// japp->RootUnLock();
/*
	vector <const ExtVetoHit*> data;
	vector<const ExtVetoHit*>::const_iterator data_it;
*/


	vector <const CalorimeterMCHit*> data;
	vector<const CalorimeterMCHit*>::const_iterator data_it;
	loop->Get(data);


		japp->RootWriteLock();


	   jout << "start event"<< std::endl;
	   eventN=eventnumber;
	   				jout << eventN<<std::endl;

		for (data_it=data.begin();data_it<data.end();data_it++){

			const CalorimeterMCHit *evhit = *data_it;


				// component=evhit->m_channel.component;
 /*
			sector = evhit->m_channel.sector;
				x = evhit->m_channel.x;
				y = evhit->m_channel.y;
*/
			sector = evhit->sector;
			x = evhit->x;
			y = evhit->y;
		// 	jout<<"component="<< component<<endl;
/*
			   E=evhit->E;
				T = evhit->T;

				*/
			El = evhit->adcl;
			Tl = evhit->tdcl;

			Er = evhit->adcr;
			Tr = evhit->tdcr;

			Ef = evhit->adcf;
			Tf = evhit->tdcf;

			Eb = evhit->adcb;
			Tb = evhit->tdcb;

			//		pedestal=(*data_it)->pedestal;

		//		T = (*data_it)->T;
		// jout << sector << " "<< component <<std::endl;
		 jout << sector << " "<<x<<" " <<y <<std::endl;
		jout <<"l= "<< El <<" "<<Tl<<" "<<"r = "<< Er << " "<<Tr<<std::endl;
		jout <<"f= "<< Ef <<" "<<Tf<<" "<<"b = "<< Eb << " "<<Tb<<std::endl;

		}






 //   jout << "Sono dentro il plugin"<<std::endl;




	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t MCtest::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t MCtest::fini(void)
{
	// Called before program exit after event processing is finished.
	return NOERROR;
}

