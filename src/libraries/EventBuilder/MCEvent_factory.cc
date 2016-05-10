
#include <iostream>
#include <iomanip>
using namespace std;

#include "MCEvent_factory.h"

#include <Calorimeter/CalorimeterHit.h>
#include <Calorimeter/CalorimeterCluster.h>			// mz
#include <IntVeto/IntVetoHit.h>
#include <ExtVeto/ExtVetoHit.h>
#include <Paddles/PaddlesHit.h>


using namespace jana;

//------------------
// init
//------------------
jerror_t MCEvent_factory::init(void)
{

	m_EC2_cut=20; //MeV
	m_ExtVeto_timeWindows=250; //ns
	m_IntVeto_timeWindows=250; //ns
	gPARMS->SetDefaultParameter("CATANIAEVENT:EC2_CUT",m_EC2_cut,"Cut on MPPC#2 energy in MeV");
	gPARMS->SetDefaultParameter("CATANIAEVENT:EXTVETO_TIME_WINDOW",m_ExtVeto_timeWindows,"ExtVeto time window (ns)");
	gPARMS->SetDefaultParameter("CATANIAEVENT:INTVETO_TIME_WINDOW",m_ExtVeto_timeWindows,"IntVeto time window (ns)");




	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t MCEvent_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t MCEvent_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{



	vector <const CalorimeterHit*> chits;
	vector <const CalorimeterHit*>::const_iterator chits_it;

	vector <const CalorimeterCluster*> cclusters;						//mz
	vector <const CalorimeterCluster*>::const_iterator cclusters_it;	//mz

	vector <const IntVetoHit*> ivhits;
	vector <const IntVetoHit*>::const_iterator ivhits_it;

	vector <const ExtVetoHit*> evhits;
	vector <const ExtVetoHit*>::const_iterator evhits_it;



	loop->Get(chits);
//	loop->Get(cclusters);	//mz
	loop->Get(ivhits);
	loop->Get(evhits);



	MCEvent *m_event=new MCEvent();
	m_event->E=0;
	m_event->E1=0;
	m_event->E2=0;
	m_event->phe1=0;
	m_event->phe2=0;
	m_event->T=0;
	m_event->nCalorimeterHits=0;


	m_event->flag_RMS=false;
	double T1,T2,dT;
	bool flag1,flag2;

//	E1=E2=0;
	T1=T2=0;
	flag1=false;
	flag2=false;
	for (chits_it=chits.begin();chits_it!=chits.end();chits_it++){
		const CalorimeterHit *hit=(*chits_it);

		/*
	     if (hit->E>0 && hit->T<=0 ){

	        	jout << "sono un evento"<<endl;

	        	jout << hit->m_channel.sector << " "<< hit->m_channel.x << " "<<hit->m_channel.y<<endl;
                jout << "Sipm1: "<< hit->m_data[0].E << " "<< hit->m_data[0].T<<endl;
                jout << "Sipm2: "<< hit->m_data[1].E << " "<< hit->m_data[1].T<<endl;

	        }
		if(hit->T>0){
*/
		if(hit->T<0) continue;
		else{
		for (int ihit=0;ihit<hit->m_data.size();ihit++){
				switch (hit->m_data[ihit].readout){
				case 1:
				//	m_event->Ec1=hit->m_data[ihit].E;
				//	T1=hit->m_data[ihit].T;
					m_event->phe1 +=(hit->m_data[ihit].E);
					m_event->E1=(hit->m_data[ihit].E)/7.3;
				//	jout << "E1= " <<hit->m_data[ihit].E<<endl;
			//		flag1=hit->m_data[ihit].good_ped_RMS;
					break;
				case 2:
				//	m_event->Ec2=hit->m_data[ihit].E;
			//		T2=hit->m_data[ihit].T;
					m_event->phe2 +=(hit->m_data[ihit].E);
					m_event->E2=(hit->m_data[ihit].E)/14.6;
					//jout << "E2= " <<E2<<endl;
				//	flag2=hit->m_data[ihit].good_ped_RMS;
					break;
				}
			}


       // jout << "E= "<< hit->E<<endl;
		m_event->E += (m_event->E1 +m_event->E2)/2;
	    m_event->nCalorimeterHits++;
	    m_event->vCalorimeterHits.push_back(hit->m_channel);
	              }
	              }
	//jout << "***********"<<endl;
//	    jout << m_event->E << " "<<m_event->nCalorimeterHits<<endl;


	/*Now loop on external veto hits*/
	m_event->nExtVetoHits=0;
	m_event->nExtVetoHitsCoincidence=0;
	for (evhits_it=evhits.begin();evhits_it!=evhits.end();evhits_it++){
		const ExtVetoHit *hit=(*evhits_it);

		if (hit->T<0) continue; //The ExtVeto condition for a "good" hit

		else{
			m_event->nExtVetoHits++;
			m_event->vExtVetoHits.push_back(hit->m_channel);
			 // if (hit->m_channel.component>6){
				//		jout << hit->m_channel.sector<< " "<<hit->m_channel.component<<endl;
				//		}



		}
	}
	/*Now loop on inner veto hits*/
	m_event->nIntVetoHits=0;
	m_event->nIntVetoHitsCoincidence=0;
	for (ivhits_it=ivhits.begin();ivhits_it!=ivhits.end();ivhits_it++){
		const IntVetoHit *hit=(*ivhits_it);

		if (hit->T<0) continue; //The IntVeto condition for a "good" hit
		else{
			m_event->nIntVetoHits++;
			m_event->vIntVetoHits.push_back(hit->m_channel);

		}
	}

	_data.push_back(m_event);

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t MCEvent_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t MCEvent_factory::fini(void)
{
	return NOERROR;
}

