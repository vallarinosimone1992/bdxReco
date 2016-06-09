// $Id$
//
//    File: JEventProcessor_MC_Full_Det_p.cc
// Created: Mon Mar  7 11:23:10 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//
#include "MC_Full_Det_p.h"

#include <TT/TranslationTable.h>


#include <MC/CalorimeterMCHit.h>
#include <MC/ExtVetoMCHit.h>
#include <MC/IntVetoMCHit.h>

#include <Calorimeter/CalorimeterHit.h>
#include <ExtVeto/ExtVetoHit.h>
#include <IntVeto/IntVetoHit.h>

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
	app->AddProcessor(new MC_Full_Det_p());
}
} // "C"


//------------------
// JEventProcessor_MC_Full_Det_p (Constructor)
//------------------
MC_Full_Det_p::MC_Full_Det_p()
{


}

//------------------
// ~JEventProcessor_MC_Full_Det_p (Destructor)
//------------------
MC_Full_Det_p::~MC_Full_Det_p()
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
jerror_t MC_Full_Det_p::init(void)
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
	t->Branch("multi_cal_thr",&multi_cal_thr);
	t->Branch("multi_cal_ext_layer",&multi_cal_ext_layer);


	t->Branch("multi_ev",&multi_ev);
	t->Branch("nhit_ev", &nhit_cal, "nhit_ev/I");
	t->Branch("sector_ev",sector_ev,"sector_ev[nhit_ev]/I");
	t->Branch("channel_ev",channel_ev,"channel_ev[nhit_ev]/I");

	t->Branch("nhit_iv", &nhit_cal, "nhit_iv/I");
	t->Branch("multi_iv",&multi_iv);
	t->Branch("multi_iv_channel0",&multi_iv_channel0);
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
    t->Branch("Tcluster",&Tcluster);
    t->Branch("sector_seed",&sector_seed);
    t->Branch("x_seed",&x_seed);
    t->Branch("y_seed",&y_seed);

    t->Branch("x_crysEmax",&x_crysEmax);
    t->Branch("y_crysEmax",&y_crysEmax);

    t->Branch("block",&block);
    t->Branch("nblock",&nblock);

    t->Branch("theta",&theta);





    //  t->Branch("h_wave",&h_wave);

	app->RootUnLock();
	return NOERROR;
}


//------------------
// brun
//------------------
jerror_t MC_Full_Det_p::brun(JEventLoop *eventLoop, int32_t runnumber)
{
	// This is called whenever the run number changes
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t MC_Full_Det_p::evnt(JEventLoop *loop, uint64_t eventnumber)
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

	vector<const MCEvent*> data;
	vector<const MCEvent*>::const_iterator data_hit;

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

			const MCEvent *clhit = *data_hit;

			phe1_tot = clhit->phe1;
			phe2_tot = clhit->phe2;
            E_tot = clhit->E;
            E_single_crys=clhit->E_single_crys;
			multi_cal = clhit->nCalorimeterHits;
			multi_cal_thr = clhit->nCalorimeterHits_thr;    // number of crystals above the threshold
			multi_cal_ext_layer = clhit->nCalorimeterHits_ext_layer;
			nhit_cal=multi_cal;
			jout<<" phe1_tot= "<<phe1_tot<<" phe2_tot= "<<phe2_tot<<" E_tot= "<<E_tot<<endl;
            jout<<" Mult_Cal= "<<multi_cal<<" Multi_cal_ext_layer= "<<multi_cal_ext_layer<<endl;

         for (int i=0; i<multi_cal;i++){
			sector_cal[i] = clhit->vCalorimeterHits.at(i).sector;
              x_cal[i] = clhit->vCalorimeterHits.at(i).x;
              y_cal[i] = clhit->vCalorimeterHits.at(i).y;
             jout << "Sector Cal= "<<sector_cal[i]<<" X= "<< x_cal[i] << " Y= "<< y_cal[i]<<endl;
         }

            multi_iv = clhit->nIntVetoHits;   // multiplcity IV total
            multi_iv_channel0 = clhit->nIntVetoHits_0; //multiplicity IV top

			nhit_iv=multi_iv;
            jout << "Mult IV="<<multi_iv << endl;

            for (int i=0; i<multi_iv;i++){
         			sector_iv[i] = clhit->vIntVetoHits.at(i).sector;
                    channel_iv[i] = clhit->vIntVetoHits.at(i).component;
                    jout<<"Sector IV = "<<sector_iv[i]<<" Component IV= "<<channel_iv[i]<<endl;
                  }

            multi_ev = clhit->nExtVetoHits;
			nhit_ev=multi_ev;
            jout << "Mult EV="<<multi_ev << endl;

            for (int i=0; i<multi_ev;i++){
                     			sector_ev[i] = clhit->vExtVetoHits.at(i).sector;
                                channel_ev[i] = clhit->vExtVetoHits.at(i).component;
                                jout<<"Sector EV = "<<sector_ev[i]<<" Component EV= "<<channel_ev[i]<<endl;

            }

            multi_sect0 = clhit->nCalorimeterHits_S0;
            multi_sect1 = clhit->nCalorimeterHits_S1;
            multi_sect2 = clhit->nCalorimeterHits_S2;
            multi_sect3 = clhit->nCalorimeterHits_S3;
            multi_sect4 = clhit->nCalorimeterHits_S4;
            multi_sect5 = clhit->nCalorimeterHits_S5;
            multi_sect6 = clhit->nCalorimeterHits_S6;
            multi_sect7 = clhit->nCalorimeterHits_S7;
            /*
            			jout<<"multi_sect0= "<<multi_sect0<<endl;
            			jout<<"multi_sect1= "<<multi_sect1<<endl;
            			jout<<"multi_sect2= "<<multi_sect2<<endl;
            			jout<<"multi_sect3= "<<multi_sect3<<endl;
            			jout<<"multi_sect4= "<<multi_sect4<<endl;
            			jout<<"multi_sect5= "<<multi_sect5<<endl;
            			jout<<"multi_sect6= "<<multi_sect6<<endl;
            			jout<<"multi_sect7= "<<multi_sect7<<endl;
            */

            multi_cl = clhit->Nhit_cluster;
            multi_cl_near_seed = clhit->Nhit_cluster_near_seed;

            Eseed = clhit->Eseed;
            Ecluster = clhit->E_cluster;
            sector_seed = clhit->sectorseed;
            x_seed = clhit->xseed;
            y_seed = clhit->yseed;
            theta=clhit->theta;
            x_crysEmax = clhit->x_crys_max;
            y_crysEmax = clhit->y_crys_max;

            block=clhit->Block;
            nblock=clhit->Nblock;



            jout<<"Eseed= "<<clhit->Eseed<<" Xseed= "<<clhit->xseed<<" Yseed= "<<clhit->yseed<<" Theta= "<<theta<<endl;
            jout<<"Nhit_cluster= "<<clhit->Nhit_cluster<<" E_cluster= "<<clhit->E_cluster<<" T_cluster= "<<clhit->T_cluster<<endl;


	}			// end loop over MC events


	jout <<" $$$$$$$$$$$$$ Now MC HIT $$$$$$$$$$$$$$$"<<endl;

    jout << "/// Calorimeter ///"<<endl;
    E_tot_MC=0;

	int i=0;
	for (data_calo_mc_hit=data_calo_mc.begin();data_calo_mc_hit<data_calo_mc.end();data_calo_mc_hit++){	// loop over CaloMC hits
		i++;
				const CalorimeterMCHit *calo_hit = *data_calo_mc_hit;
				jout<<"Sector= "<<calo_hit->sector<<" X= "<<calo_hit->x<<" Y= "<<calo_hit->y<<endl;
				jout<<"adcr= "<<calo_hit->adcr<<" adcl= "<<calo_hit->adcl<<endl;				// adcr == SiPM1  , adcl=SiPM2
				jout<<" totEdep= "<<calo_hit->totEdep<<endl;
				jout<<" PID= "<<calo_hit->pid<<endl;

				totEdep=calo_hit->totEdep;
//				E1[i] = calo_hit->adcr/9.5;
//				E2[i] = calo_hit->adcl/17.;
				jout << "(hardcoded calib) E1= "<<calo_hit->adcr/9.5<<" E2= "<< calo_hit->adcl/17. <<endl;
				E_tot_MC+=calo_hit->totEdep;
				         }
    jout <<"////////////"<<endl;
     jout<<"E_tot_MC= "<<E_tot_MC<<endl;
	jout << "/// IV /// "<<endl;
	 i=0;
		for (data_iv_mc_hit=data_iv_mc.begin();data_iv_mc_hit<data_iv_mc.end();data_iv_mc_hit++){	// loop over Interna Veto MC hits
			i++;
					const IntVetoMCHit *iv_hit = *data_iv_mc_hit;
					jout<<"Sector= "<<iv_hit->sector<< " Channel= "<<iv_hit->channel<<" System= "<<iv_hit->system<<endl;
					jout<<"totEdep= "<<iv_hit->totEdep<<endl;
					jout<<"adc1= "<<iv_hit->adc1<<" adc2= "<<iv_hit->adc2<<endl;
					jout<<"adc3= "<<iv_hit->adc3<<" adc4= "<<iv_hit->adc4<<endl;
					jout<<"tdc1= "<<iv_hit->tdc1<<" tdc2= "<<iv_hit->tdc2<<endl;
					jout<<"tdc3= "<<iv_hit->tdc3<<" tdc4= "<<iv_hit->tdc4<<endl;

					         }
	    jout <<"////////////"<<endl;


		jout << "/// EV /// "<<endl;
	  i=0;
				for (data_ev_mc_hit=data_ev_mc.begin();data_ev_mc_hit<data_ev_mc.end();data_ev_mc_hit++){	// loop over External Veto MC hits
					i++;
							const ExtVetoMCHit *ev_hit = *data_ev_mc_hit;
							jout<<"Sector= "<<ev_hit->sector<< " Channel= "<<ev_hit->channel<<" System= "<<ev_hit->system<<endl;
							jout<<"totEdep= "<<ev_hit->totEdep<<endl;
							jout<<"adc= "<<ev_hit->adc<<" tdc= "<<ev_hit->tdc<<endl;
							         }
			    jout <<"////////////"<<endl;


				jout <<" $$$$$$$$$$$$$ Now Data HIT $$$$$$$$$$$$$$$"<<endl;

		jout << "/// Calorimeter /// "<<endl;
			 i=0;
					for (data_calo_hit=data_calo.begin();data_calo_hit<data_calo.end();data_calo_hit++){	// loop over reconstructed Calorimeter hits
									 	i++;
									    const CalorimeterHit *calo_hit = *data_calo_hit;

									    jout<<"Sector= "<<calo_hit->m_channel.sector<<" X= "<<calo_hit->m_channel.x<<" Y= "<<calo_hit->m_channel.y<<" Readout= "<<calo_hit->m_channel.readout<<endl;
									    jout<<"E= "<<calo_hit->E<<endl;
									    jout<<"Q= "<<calo_hit->Q<<endl;			// Q in p.e. is the sum of SiPM1+SiPM2
									    jout<<"T= "<<calo_hit->T<<endl;

									    for (int ihit=0;ihit<calo_hit->m_data.size();ihit++){			// loop over the 2 SiPMs
									    				switch (calo_hit->m_data[ihit].readout){
									    				case 1:
									    //					E1[i]=(calo_hit->m_data[ihit].E); 		// energy calibration for muons
									    					E1[i]=(calo_hit->m_data[ihit].Q)/9.5; // energy calibration from data : 16 MeV  protons at 12cm from the SiPM
									    					break;
									    				case 2:
									    //					E2[i]=(calo_hit->m_data[ihit].E); 	// energy calibration for muons
									    					E2[i]=(calo_hit->m_data[ihit].Q)/17; // energy calibration from data : 16 MeV  protons at 12cm from the SiPM
									    					break;
									    				}
									    			}
									    	E[i]=(E1[i]+E2[i])/2;

									    	jout<<"i= "<<i<<" E1= "<<E1[i]<<" E2= "<<E2[i]<< " E= "<<E[i]<<endl;

									}
									    jout <<"////////////"<<endl;


	    jout << "/// IV /// "<<endl;
			  i=0;
				  	for (data_iv_hit=data_iv.begin();data_iv_hit<data_iv.end();data_iv_hit++){	// loop over External Veto MC hits
					    			i++;
					    				const IntVetoHit *iv_hit = *data_iv_hit;

					    			jout<<"Sector= "<<iv_hit->m_channel.sector<<" Layer= "<<iv_hit->m_channel.layer<<" Component= "<<iv_hit->m_channel.component<<endl;
					    			jout<<"Q= "<<iv_hit->Q<<endl;
					    		    jout<<"T= "<<iv_hit->T<<endl;
					    							         }

					    			    jout <<"////////////"<<endl;

	    jout << "/// EV /// "<<endl;
	    	  i=0;
	    				for (data_ev_hit=data_ev.begin();data_ev_hit<data_ev.end();data_ev_hit++){	// loop over External Veto MC hits
	    					i++;
	    							const ExtVetoHit *ev_hit = *data_ev_hit;

	    							jout<<"Sector= "<<ev_hit->m_channel.sector<<" Component= "<<ev_hit->m_channel.component<<endl;
	    							jout<<"Q= "<<ev_hit->E<<endl;
	    							jout<<"T= "<<ev_hit->T<<endl;
	    							         }

	    			    jout <<"////////////"<<endl;

		t->Fill();

		japp->RootUnLock();

		jout<<"**********************************************"<<endl;
		jout<<"**********************************************"<<endl;
		jout<<"**********************************************"<<endl;

		return NOERROR;
		}

//------------------
// erun
//------------------
jerror_t MC_Full_Det_p::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t MC_Full_Det_p::fini(void)
{
	// Called before program exit after event processing is finished.
	return NOERROR;
}



