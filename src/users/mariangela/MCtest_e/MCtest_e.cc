// $Id$
//
//    File: JEventProcessor_MCtest.cc
// Created: Mon Mar  7 11:23:10 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include "MCtest_e.h"


#include <MC/CalorimeterMCHit.h>
#include <Calorimeter/CalorimeterHit.h>

#include <EventBuilder/MCEvent_EM.h>
#include <EventBuilder/MCEvent.h>

#include <system/JROOTOutput.h>
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


	t->Branch("eventN",&eventN);


	t->Branch("phe1_tot",&phe1_tot);
	t->Branch("phe2_tot",&phe2_tot);
	t->Branch("E_tot",&E_tot);

	t->Branch("sector_cal",sector_cal,"sector_cal[800]/I");
	t->Branch("x_cal",x_cal,"x_cal[800]/I");
	t->Branch("y_cal",y_cal,"y_cal[800]/I");
	t->Branch("multi_cal",&multi_cal);
	t->Branch("multi_sect0",&multi_sect0);
	t->Branch("multi_sect1",&multi_sect1);
	t->Branch("multi_sect2",&multi_sect2);
	t->Branch("multi_sect3",&multi_sect3);
	t->Branch("multi_sect4",&multi_sect4);
	t->Branch("multi_sect5",&multi_sect5);
	t->Branch("multi_sect6",&multi_sect6);
	t->Branch("multi_sect7",&multi_sect7);


	t->Branch("multi_ev",&multi_ev);
 	t->Branch("sector_ev",sector_ev,"sector_ev[800]/I");
 	t->Branch("channel_ev",channel_ev,"channel_ev[800]/I");

	t->Branch("multi_iv",&multi_iv);
 	t->Branch("sector_iv",sector_iv,"sector_iv[800]/I");
 	t->Branch("channel_iv",channel_iv,"channel_iv[800]/I");

    t->Branch("E1",E1,"E1[800]/D");
    t->Branch("E2",E2,"E2[800]/D");


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


	vector<const MCEvent_EM*> data;
	vector<const MCEvent_EM*>::const_iterator data_it;

	vector<const CalorimeterMCHit*> data_calo;
	vector<const CalorimeterMCHit*>::const_iterator data_calo_it;


	loop->Get(data);
	loop->Get(data_calo);

		japp->RootWriteLock();

		for(int i=0; i<800; i++){
		   sector_cal[i]= -99;
		   x_cal[i] = -99;
		   y_cal[i] = -99;
		   sector_iv[i]= -99;
		   channel_iv[i]= -99;
		   sector_ev[i] = -99;
		   channel_ev[i]= -99;
		   E1[i]= -99;
		   E2[i]= -99;
		}

	   eventN=eventnumber;

	//   jout << "start event= "<< eventN<< endl;


	for (data_it=data.begin();data_it<data.end();data_it++){		// loop over MC events

			const MCEvent_EM *clhit = *data_it;

//			E1 = clhit->E1;
//			E2 = clhit->E2;
			phe1_tot = clhit->phe1;
			phe2_tot = clhit->phe2;
            E_tot = clhit->E;
			multi_cal = clhit->nCalorimeterHits;


			multi_sect0 = clhit->nCalorimeterHits_S0;
			multi_sect1 = clhit->nCalorimeterHits_S1;
			multi_sect2 = clhit->nCalorimeterHits_S2;
			multi_sect3 = clhit->nCalorimeterHits_S3;
			multi_sect4 = clhit->nCalorimeterHits_S4;
			multi_sect5 = clhit->nCalorimeterHits_S5;
			multi_sect6 = clhit->nCalorimeterHits_S6;
			multi_sect7 = clhit->nCalorimeterHits_S7;

			jout<<"multi_sect0= "<<multi_sect0<<endl;
			jout<<"multi_sect1= "<<multi_sect1<<endl;
			jout<<"multi_sect2= "<<multi_sect2<<endl;
			jout<<"multi_sect3= "<<multi_sect3<<endl;
			jout<<"multi_sect4= "<<multi_sect4<<endl;
			jout<<"multi_sect5= "<<multi_sect5<<endl;
			jout<<"multi_sect6= "<<multi_sect6<<endl;
			jout<<"multi_sect7= "<<multi_sect7<<endl;


			jout<<"Eseed= "<<clhit->Eseed<<" Xseed= "<<clhit->xseed<<" Yseed= "<<clhit->yseed<<endl;
			jout<<"Nhit_cluster= "<<clhit->Nhit_cluster<<" E_cluster= "<<clhit->E_cluster<<endl;

		//	jout<<clhit->E1<< " "<<clhit->E2<<endl;
		//	jout<<" phe1_tot= "<<phe1_tot<<" phe2_tot= "<<phe2_tot<<" E_tot= "<<E_tot<<" muliti_cal= "<<multi_cal<<endl;


         for (int i=0; i<multi_cal;i++){
			sector_cal[i] = clhit->vCalorimeterHits.at(i).sector;
              x_cal[i] = clhit->vCalorimeterHits.at(i).x;
              y_cal[i] = clhit->vCalorimeterHits.at(i).y;
         //    jout << "sector= "<<sector_cal[i]<<" X= "<< x_cal[i] << " Y= "<< y_cal[i]<<endl;
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




	int i=0;
	for (data_calo_it=data_calo.begin();data_calo_it<data_calo.end();data_calo_it++){	// loop over CaloMC hits
		i++;
				const CalorimeterMCHit *calo_hit = *data_calo_it;
				jout<<"adcr= "<<calo_hit->adcr<<" adcl= "<<calo_hit->adcl<<endl;				// adcr == SiPM1  , adcl=SiPM2
				jout<<" X= "<<calo_hit->x<<" Y= "<<calo_hit->y<<endl;
				E1[i] = calo_hit->adcr/7.3;
				E2[i] = calo_hit->adcl/14.6;
				jout << "E1= "<<E1[i]<<" E2= "<< E2[i] <<endl;

				         }






		t->Fill();

		japp->RootUnLock();

//		jout<<"*******************"<<endl;

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

