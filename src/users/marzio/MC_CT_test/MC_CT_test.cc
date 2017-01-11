// $Id$
//
//    File: JEventProcessor_MC_CT_test.cc
// Created: Mon Mar  7 11:23:10 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#include <MC/ExtVetoMCHit.h>

#include <TT/TranslationTable.h>


#include <MC/CalorimeterMCHit.h>
#include <MC/ExtVetoMCHit.h>
#include <MC/IntVetoMCHit.h>


#include <Calorimeter/CalorimeterHit.h>

#include <ExtVeto/ExtVetoHit.h>
#include <ExtVeto/ExtVetoPMTHit.h>


#include <IntVeto/IntVetoHit.h>

#include <EventBuilder/CataniaEvent.h>

#include <system/JROOTOutput.h>
#include "MC_CT_test.h"
#include "TTree.h"



using namespace jana;

using namespace std;

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>

extern "C"{
void InitPlugin(JApplication *app){
	InitJANAPlugin(app);
	app->AddProcessor(new MC_CT_test());
}
} // "C"


//------------------
// JEventProcessor_MC_CT_test (Constructor)
//------------------
MC_CT_test::MC_CT_test()
{


}

//------------------
// ~JEventProcessor_MC_CT_test (Destructor)
//------------------
MC_CT_test::~MC_CT_test()
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
jerror_t MC_CT_test::init(void)
{

	app->RootWriteLock();

	jout<<"test::init is called"<<std::endl;

	t=new TTree("t","t");


	t->Branch("eventN",&eventN);



	t->Branch("Ecrys",&Ecrys);

	t->Branch("multi_ev",&multi_ev);
	t->Branch("multi_ev_coinc",&multi_ev_coinc);
	t->Branch("multi_iv",&multi_iv);
	t->Branch("multi_iv_coinc",&multi_iv_coinc);


 	t->Branch("channel_ev",channel_ev,"channel_ev[12]/I");
 	t->Branch("Ene_ev",Ene_ev,"Ene_ev[12]/D");
 	t->Branch("channel_ev_MC",channel_ev,"channel_ev_MC[12]/I");
 	t->Branch("Ene_ev_MC",Ene_ev,"Ene_ev_MC[12]/D");


	app->RootUnLock();
	return NOERROR;
}


//------------------
// brun
//------------------
jerror_t MC_CT_test::brun(JEventLoop *eventLoop, int32_t runnumber)
{
	// This is called whenever the run number changes
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t MC_CT_test::evnt(JEventLoop *loop, uint64_t eventnumber)
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


	vector<const CataniaEvent*> data;
	vector<const CataniaEvent*>::const_iterator data_hit;

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

	vector<const ExtVetoPMTHit*> data_ev_pmt;
	vector<const ExtVetoPMTHit*>::const_iterator data_ev_pmt_hit;


	loop->Get(data);
	loop->Get(data_calo_mc);
	loop->Get(data_iv_mc);
	loop->Get(data_ev_mc);
	loop->Get(data_calo);
	loop->Get(data_iv);
	loop->Get(data_ev);
	loop->Get(data_ev_pmt);




		japp->RootWriteLock();

		for(int i=0; i<11; i++){
				   channel_ev[i]= -99;
				   Ene_ev[i]=-99;
				   channel_ev_MC[i]= -99;
				   Ene_ev_MC[i]=-99;

				}


	   eventN=eventnumber;

	   jout << "start event= "<< eventN<< endl;


	for (data_hit=data.begin();data_hit<data.end();data_hit++){		// loop over MC events

			const CataniaEvent *clhit = *data_hit;
            multi_iv = clhit->nIntVetoHits;
            multi_iv_coinc = clhit->nIntVetoHitsCoincidence;
            jout << "Mult IV="<<multi_iv << " Mult EV time coinc= "<<multi_ev_coinc <<endl;

            multi_ev = clhit->nExtVetoHits;
            multi_ev_coinc = clhit->nExtVetoHitsCoincidence;
            jout << "Mult EV="<<multi_ev << " Mult EV time coinc= "<<multi_ev_coinc <<endl;

            Ecrys=clhit->E;

	}


	jout <<" $$$$$$$$$$$$$ Now MC HIT $$$$$$$$$$$$$$$"<<endl;

    jout << "/// Calorimeter ///"<<endl;

	int i=0;
	for (data_calo_mc_hit=data_calo_mc.begin();data_calo_mc_hit<data_calo_mc.end();data_calo_mc_hit++){	// loop over CaloMC hits
		i++;
				const CalorimeterMCHit *calo_hit = *data_calo_mc_hit;
				jout<<"Sector= "<<calo_hit->sector<<" X= "<<calo_hit->x<<" Y= "<<calo_hit->y<<endl;
				jout<<"adcr= "<<calo_hit->adcr<<" adcl= "<<calo_hit->adcl<<endl;				// adcr == SiPM1  , adcl=SiPM2
				jout<<" totEdep= "<<calo_hit->totEdep<<endl;
//				totEdep=calo_hit->totEdep;
//				E1[i] = calo_hit->adcr/9.5;
//				E2[i] = calo_hit->adcl/17.;
//				jout << "(hardcoded calib) E1= "<<E1[i]<<" E2= "<< E2[i] <<endl;
//				E_tot_MC+=calo_hit->totEdep;
				         }
    jout <<"////////////"<<endl;
 //    jout<<"E_tot_MC= "<<E_tot_MC<<endl;
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
							jout<<"adc= "<<ev_hit->adc1<<" tdc= "<<ev_hit->tdc1<<endl;
							Ene_ev_MC[i]=ev_hit->adc1;
							channel_ev_MC[i]=ev_hit->channel;

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
	    							jout<<"Q= "<<ev_hit->E<<endl;
	    							jout<<"T= "<<ev_hit->T<<endl;
	    							Ene_ev[i]=ev_hit->E;
	    							channel_ev[i]=ev_hit->m_channel.component;
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
jerror_t MC_CT_test::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t MC_CT_test::fini(void)
{
	// Called before program exit after event processing is finished.
	return NOERROR;
}



