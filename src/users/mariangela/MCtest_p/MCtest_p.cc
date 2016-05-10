// $Id$
//
//    File: JEventProcessor_MCtest.cc
// Created: Mon Mar  7 11:23:10 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include <MC/ExtVetoMCHit.h>

#include <ExtVeto/ExtVetoDigiHit.h>
#include <ExtVeto/ExtVetoHit.h>
#include <ExtVeto/ExtVetoSummary.h>

#include <MC/CalorimeterMCHit.h>

#include <Calorimeter/CalorimeterDigiHit.h>
#include <Calorimeter/CalorimeterHit.h>
#include <EventBuilder/MCEvent.h>

#include <system/JROOTOutput.h>
#include "MCtest_p.h"
#include "TTree.h"



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

	app->RootWriteLock();

	jout<<"test::init is called"<<std::endl;

	t=new TTree("t","t");


	t->Branch("E1",&E1);
	t->Branch("E2",&E2);

	t->Branch("phe1",&phe1);
	t->Branch("phe2",&phe2);


	t->Branch("E",&E);
	t->Branch("sector_cal",sector_cal,"sector_cal[800]/I");
	t->Branch("x_cal",x_cal,"x_cal[800]/I");
	t->Branch("y_cal",x_cal,"y_cal[800]/I");
	t->Branch("multi_cal",&multi_cal);

	t->Branch("multi_ev",&multi_ev);
 	t->Branch("sector_ev",sector_ev,"sector_ev[800]/I");
 	t->Branch("channel_ev",channel_ev,"channel_ev[800]/I");

	t->Branch("multi_iv",&multi_iv);
 	t->Branch("sector_iv",sector_iv,"sector_iv[800]/I");
 	t->Branch("channel_iv",channel_iv,"channel_iv[800]/I");



    //  t->Branch("h_wave",&h_wave);

	app->RootUnLock();
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


	vector<const MCEvent*> data;
	vector<const MCEvent*>::const_iterator data_it;



//	loop->Get(data_hit);
	loop->Get(data);



		japp->RootWriteLock();


		for(int i=0; i<800; i++){

			sector_cal[i]= 99;
			x_cal[i] = 99;
			y_cal[i] = 99;
		   sector_iv[i]= 99;
		   channel_iv[i]= 99;
		   sector_ev[i] = 99;
		   channel_ev[i]= 99;
		}

	//  jout << "start event"<< std::endl;
	   eventN=eventnumber;



	for (data_it=data.begin();data_it<data.end();data_it++){

			const MCEvent *clhit = *data_it;


			E1 = clhit->E1;
			E2 = clhit->E2;
			phe1 = clhit->phe1;
			phe2 = clhit->phe2;
            E = clhit->E;
			multi_cal = clhit->nCalorimeterHits;


         for (int i=0; i<multi_cal;i++){

			sector_cal[i] = clhit->vCalorimeterHits.at(i).sector;
              x_cal[i] = clhit->vCalorimeterHits.at(i).x;
              y_cal[i] = clhit->vCalorimeterHits.at(i).y;
         //      jout << sector_cal[i]<<" "<< x_cal[i] << " "<< y_cal[i]<<endl;
         }

/*
          if(multi_cal==35){
        	  jout << "*******"<<endl;
        	  jout << eventN<<endl;
        	  for (int i=0; i<multi_cal;i++){
            jout << sector_cal[i]<<" "<< x_cal[i] << " "<< y_cal[i]<<endl;
                                            }
        	  jout <<"E-tot= " <<E<<endl;
        	  }

*/
      //    jout << sector_cal << endl;
            multi_iv = clhit->nIntVetoHits;

            for (int i=0; i<multi_iv;i++){
         			sector_iv[i] = clhit->vIntVetoHits.at(i).sector;
                    channel_iv[i] = clhit->vIntVetoHits.at(i).component;

                  }

            multi_ev = clhit->nExtVetoHits;
        //    jout << "plug"<<endl;
            for (int i=0; i<multi_ev;i++){
                     			sector_ev[i] = clhit->vExtVetoHits.at(i).sector;
                                channel_ev[i] = clhit->vExtVetoHits.at(i).component;


            }


	}



		t->Fill();






		japp->RootUnLock();

	//	downthres:


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

