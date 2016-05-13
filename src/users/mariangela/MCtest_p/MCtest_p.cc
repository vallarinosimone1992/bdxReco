// $Id$
//
//    File: JEventProcessor_MCtest.cc
// Created: Mon Mar  7 11:23:10 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include <MC/ExtVetoMCHit.h>

#include <TT/TranslationTable.h>


#include <MC/CalorimeterMCHit.h>
#include <MC/ExtVetoMCHit.h>
#include <MC/IntVetoMCHit.h>

#include <Calorimeter/CalorimeterDigiHit.h>
#include <Calorimeter/CalorimeterHit.h>

#include <ExtVeto/ExtVetoDigiHit.h>
#include <ExtVeto/ExtVetoHit.h>
#include <ExtVeto/ExtVetoSummary.h>

#include <IntVeto/IntVetoHit.h>


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

std::ostream& bold_on(std::ostream& os)
{
    return os << "\e[1m";
}

std::ostream& bold_off(std::ostream& os)
{
    return os << "\e[0m";
}
jerror_t MCtest::init(void)
{

	app->RootWriteLock();

	jout<<"test::init is called"<<std::endl;

	t=new TTree("t","t");


	t->Branch("eventN",&eventN);


	t->Branch("phe1_tot",&phe1_tot);
	t->Branch("phe2_tot",&phe2_tot);
	t->Branch("E_tot",&E_tot);
	t->Branch("totEdep",&totEdep);


	t->Branch("sector_cal",sector_cal,"sector_cal[800]/I");
	t->Branch("x_cal",x_cal,"x_cal[800]/I");
	t->Branch("y_cal",y_cal,"y_cal[800]/I");
	t->Branch("multi_cal",&multi_cal);
	t->Branch("multi_cal_ext_layer",&multi_cal_ext_layer);


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

	   jout << "start event= "<< eventN<< endl;


	for (data_hit=data.begin();data_hit<data.end();data_hit++){		// loop over MC events

			const MCEvent *clhit = *data_hit;

//			E1 = clhit->E1;
//			E2 = clhit->E2;
			phe1_tot = clhit->phe1;
			phe2_tot = clhit->phe2;
            E_tot = clhit->E;
			multi_cal = clhit->nCalorimeterHits;
			multi_cal_ext_layer = clhit->nCalorimeterHits_ext_layer;

			jout<<" phe1_tot= "<<phe1_tot<<" phe2_tot= "<<phe2_tot<<" E_tot= "<<E_tot<<endl;
            jout<<" Mult_Cal= "<<multi_cal<<" Multi_cal_ext_layer= "<<multi_cal_ext_layer<<endl;

         for (int i=0; i<multi_cal;i++){
			sector_cal[i] = clhit->vCalorimeterHits.at(i).sector;
              x_cal[i] = clhit->vCalorimeterHits.at(i).x;
              y_cal[i] = clhit->vCalorimeterHits.at(i).y;
             jout << "Sector Cal= "<<sector_cal[i]<<" X= "<< x_cal[i] << " Y= "<< y_cal[i]<<endl;
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
            multi_iv = clhit->nIntVetoHits;

            jout << "Mult IV="<<multi_iv << endl;

            for (int i=0; i<multi_iv;i++){
         			sector_iv[i] = clhit->vIntVetoHits.at(i).sector;
                    channel_iv[i] = clhit->vIntVetoHits.at(i).component;
                    jout<<"Sector IV = "<<sector_iv[i]<<" Component IV= "<<channel_iv[i]<<endl;
                  }

            multi_ev = clhit->nExtVetoHits;
            jout << "Mult EV="<<multi_ev << endl;

            for (int i=0; i<multi_ev;i++){
                     			sector_ev[i] = clhit->vExtVetoHits.at(i).sector;
                                channel_ev[i] = clhit->vExtVetoHits.at(i).component;
                                jout<<"Sector EV = "<<sector_ev[i]<<" Component EV= "<<channel_ev[i]<<endl;

            }


	}


	jout <<" $$$$$$$$$$$$$ Now MC HIT $$$$$$$$$$$$$$$"<<endl;

    jout << "/// Calorimeter ///"<<endl;

	int i=0;
	for (data_calo_mc_hit=data_calo_mc.begin();data_calo_mc_hit<data_calo_mc.end();data_calo_mc_hit++){	// loop over CaloMC hits
		i++;
				const CalorimeterMCHit *calo_hit = *data_calo_mc_hit;
				jout<<"adcr= "<<calo_hit->adcr<<" adcl= "<<calo_hit->adcl<<endl;				// adcr == SiPM1  , adcl=SiPM2
				jout<<"Sector= "<<calo_hit->sector<<" X= "<<calo_hit->x<<" Y= "<<calo_hit->y<<endl;
				jout<<" totEdep= "<<calo_hit->totEdep<<endl;
				totEdep=calo_hit->totEdep;
				E1[i] = calo_hit->adcr/9.5;			//7.3
				E2[i] = calo_hit->adcl/17.;		//14.6
				jout << "(hardcoded calib) E1= "<<E1[i]<<" E2= "<< E2[i] <<endl;
				         }
    jout <<"////////////"<<endl;

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
	    							jout<<"E= "<<ev_hit->E<<endl;
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



