// $Id$
//
//    File: JEventProcessor_MC_Full_Det.cc
// Created: Mon Mar  7 11:23:10 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//
#include "MC_Full_Det.h"

#include <TT/TranslationTable.h>


#include <MC/CalorimeterMCHit.h>
#include <MC/ExtVetoMCHit.h>
#include <MC/IntVetoMCHit.h>

#include <Calorimeter/CalorimeterHit.h>
#include <ExtVeto/ExtVetoHit.h>
#include <IntVeto/IntVetoHit.h>

#include <EventBuilder/MCEvent_EM.h>

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
	app->AddProcessor(new MC_Full_Det());
}
} // "C"


//------------------
// JEventProcessor_MC_Full_Det (Constructor)
//------------------
MC_Full_Det::MC_Full_Det()
{


}

//------------------
// ~JEventProcessor_MC_Full_Det (Destructor)
//------------------
MC_Full_Det::~MC_Full_Det()
{

}

//------------------
// init
//------------------

std::ostream& bold_on(std::ostream& os)
{
    return os << "\e[1m";
}

std::ostream& bold_off(std::ostream& os)
{
    return os << "\e[0m";
}
jerror_t MC_Full_Det::init(void)
{

	app->RootWriteLock();

	jout<<"test::init is called"<<std::endl;

	t=new TTree("t","t");


	t->Branch("eventN",&eventN);


	t->Branch("phe1_tot",&phe1_tot);
	t->Branch("phe2_tot",&phe2_tot);
	t->Branch("E_tot",&E_tot);
	t->Branch("E_tot_MC",&E_tot_MC);
	t->Branch("totEdep",&totEdep);
	t->Branch("E_single_crys",&E_single_crys);


	t->Branch("multi_cal",&multi_cal);
	t->Branch("multi_cal_ext_layer",&multi_cal_ext_layer);


	t->Branch("multi_ev",&multi_ev);
	t->Branch("nhit_ev", &nhit_cal, "nhit_ev/I");
	t->Branch("sector_ev",sector_ev,"sector_ev[nhit_ev]/I");
	t->Branch("channel_ev",channel_ev,"channel_ev[nhit_ev]/I");

	t->Branch("nhit_iv", &nhit_cal, "nhit_iv/I");
	t->Branch("multi_iv",&multi_iv);
	t->Branch("sector_iv",sector_iv,"sector_iv[nhit_iv]/I");
	t->Branch("channel_iv",channel_iv,"channel_iv[nhit_iv]/I");


	t->Branch("nhit_cal", &nhit_cal, "nhit_cal/I");
 	t->Branch("sector_cal",sector_cal,"sector_cal[nhit_cal]/I");
	t->Branch("x_cal",x_cal,"x_cal[nhit_cal]/I");
	t->Branch("y_cal",y_cal,"y_cal[nhit_cal]/I");
    t->Branch("E1",E1,"E1[nhit_cal]/D");
    t->Branch("E2",E2,"E2[nhit_cal]/D");
    t->Branch("E",E,"E[nhit_cal]/D");



    t->Branch("multi_sect0",&multi_sect0);
    t->Branch("multi_sect1",&multi_sect1);
    t->Branch("multi_sect2",&multi_sect2);
    t->Branch("multi_sect3",&multi_sect3);
    t->Branch("multi_sect4",&multi_sect4);
    t->Branch("multi_sect5",&multi_sect5);
    t->Branch("multi_sect6",&multi_sect6);
    t->Branch("multi_sect7",&multi_sect7);
    t->Branch("multi_cl",&multi_cl);
    t->Branch("multi_cl_near_seed",&multi_cl_near_seed);

    t->Branch("Eseed",&Eseed);
    t->Branch("Ecluster",&Ecluster);
    t->Branch("sector_seed",&sector_seed);
    t->Branch("x_seed",&x_seed);
    t->Branch("y_seed",&y_seed);
    t->Branch("theta_cl",&theta_cl);
    t->Branch("number_sector_cl",&number_sector_cl);



    //  t->Branch("h_wave",&h_wave);

	app->RootUnLock();
	return NOERROR;
}


//------------------
// brun
//------------------
jerror_t MC_Full_Det::brun(JEventLoop *eventLoop, int32_t runnumber)
{
	// This is called whenever the run number changes
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t MC_Full_Det::evnt(JEventLoop *loop, uint64_t eventnumber)
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
	vector<const ExtVetoHit*>::const_hiterator data_hit;
*/

	vector<const MCEvent_EM*> data;
	vector<const MCEvent_EM*>::const_iterator data_hit;

	vector<const CalorimeterMCHit*> data_calo_mc;
	vector<const CalorimeterMCHit*>::const_iterator data_calo_mc_hit;

	vector<const IntVetoMCHit*> data_iv_mc;
	vector<const IntVetoMCHit*>::const_iterator data_iv_mc_hit;

	vector<const ExtVetoMCHit*> data_ev_mc;
	vector<const ExtVetoMCHit*>::const_iterator data_ev_mc_hit;

	vector<const CalorimeterHit*> data_calo;
	vector<const CalorimeterHit*>::const_iterator data_calo_hit;

	vector<const IntVetoHit*> data_iv;
	vector<const IntVetoHit*>::const_iterator data_iv_hit;

	vector<const ExtVetoHit*> data_ev;
	vector<const ExtVetoHit*>::const_iterator data_ev_hit;


	loop->Get(data);
	loop->Get(data_calo_mc);
	loop->Get(data_iv_mc);
	loop->Get(data_ev_mc);
	loop->Get(data_calo);
	loop->Get(data_iv);
	loop->Get(data_ev);



		japp->RootWriteLock();

	   eventN=eventnumber;

	   jout << "start event= "<< eventN<< endl;


	for (data_hit=data.begin();data_hit<data.end();data_hit++){		// loop over MC events

			const MCEvent_EM *clhit = *data_hit;

			phe1_tot = clhit->phe1;
			phe2_tot = clhit->phe2;
            E_tot = clhit->E;
            E_single_crys=clhit->E_single_crys;
			multi_cal = clhit->nCalorimeterHits;
			multi_cal_ext_layer = clhit->nCalorimeterHits_ext_layer;
			nhit_cal=multi_cal;

         for (int i=0; i<multi_cal;i++){
			sector_cal[i] = clhit->vCalorimeterHits.at(i).sector;
              x_cal[i] = clhit->vCalorimeterHits.at(i).x;
              y_cal[i] = clhit->vCalorimeterHits.at(i).y;

         }

            multi_iv = clhit->nIntVetoHits;
			nhit_iv=multi_iv;


            for (int i=0; i<multi_iv;i++){
         			sector_iv[i] = clhit->vIntVetoHits.at(i).sector;
                    channel_iv[i] = clhit->vIntVetoHits.at(i).component;

                  }

            multi_ev = clhit->nExtVetoHits;
			nhit_ev=multi_ev;


            for (int i=0; i<multi_ev;i++){
                     			sector_ev[i] = clhit->vExtVetoHits.at(i).sector;
                                channel_ev[i] = clhit->vExtVetoHits.at(i).component;

            }

            multi_sect0 = clhit->nCalorimeterHits_S0;
            multi_sect1 = clhit->nCalorimeterHits_S1;
            multi_sect2 = clhit->nCalorimeterHits_S2;
            multi_sect3 = clhit->nCalorimeterHits_S3;
            multi_sect4 = clhit->nCalorimeterHits_S4;
            multi_sect5 = clhit->nCalorimeterHits_S5;
            multi_sect6 = clhit->nCalorimeterHits_S6;
            multi_sect7 = clhit->nCalorimeterHits_S7;


            multi_cl = clhit->Nhit_cluster;
            multi_cl_near_seed = clhit->Nhit_cluster_near_seed;

            Eseed = clhit->Eseed;
            Ecluster = clhit->E_cluster;
            sector_seed = clhit->sectorseed;
            x_seed = clhit->xseed;
            y_seed = clhit->yseed;

            theta_cl = clhit->theta;
            number_sector_cl = clhit->sector_EM;

//            jout<<"Eseed= "<<clhit->Eseed<<" Xseed= "<<clhit->xseed<<" Yseed= "<<clhit->yseed<<endl;
//            jout<<"Nhit_cluster= "<<clhit->Nhit_cluster<<" E_cluster= "<<clhit->E_cluster<<endl;


	}			// end loop over MC events





			int i=0;
					for (data_calo_hit=data_calo.begin();data_calo_hit<data_calo.end();data_calo_hit++){	// loop over reconstructed Calorimeter hits
									 	i++;
									    const CalorimeterHit *calo_hit = *data_calo_hit;


									    for (int ihit=0;ihit<calo_hit->m_data.size();ihit++){			// loop over the 2 SiPMs
									    				switch (calo_hit->m_data[ihit].readout){
									    				case 1:
									    					E1[i]=(calo_hit->m_data[ihit].E); 		// energy calibration for muons
									    //					E1[i]=(calo_hit->m_data[ihit].Q)/9.5; // energy calibration from data : 16 MeV  protons at 12cm from the SiPM
									    					break;
									    				case 2:
									    					E2[i]=(calo_hit->m_data[ihit].E); 	// energy calibration for muons
									    //					E2[i]=(calo_hit->m_data[ihit].Q)/17; // energy calibration from data : 16 MeV  protons at 12cm from the SiPM
									    					break;
									    				}
									    			}
									    	E[i]=(E1[i]+E2[i])/2;



									}



		t->Fill();

		japp->RootUnLock();



		return NOERROR;
		}

//------------------
// erun
//------------------
jerror_t MC_Full_Det::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t MC_Full_Det::fini(void)
{
	// Called before program exit after event processing is finished.
	return NOERROR;
}



