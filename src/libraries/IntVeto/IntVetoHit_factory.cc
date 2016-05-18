// $Id$
//
//    File: IntVetoHit_factory.cc
// Created: Wed Jan 13 21:08:09 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;
#include "IntVetoDigiHit.h"
#include "IntVetoHit.h"
#include "IntVetoHit_factory.h"
using namespace jana;


IntVetoHit_factory::IntVetoHit_factory():
												isMC(0){

	m_THR_singleReadout=5;
	m_THR_multipleReadout=3;
	m_DT_multipleReadout=100; //ns
	m_N_multipleReadout=2;
	m_hit_bottom_workAround=0;

	gPARMS->SetDefaultParameter("INTVETO:HIT_THR_SINGLE",m_THR_singleReadout,"Threshold in phe (charge) for a detector with single readout");
	gPARMS->SetDefaultParameter("INTVETO:HIT_THR_MULTI",m_THR_multipleReadout,"Threshold in phe (charge) for a detector with multi readout");
	gPARMS->SetDefaultParameter("INTVETO:HIT_DT_MULTI",m_DT_multipleReadout,"Time coincidence window (in ns) for hits in a detector with multi readout");
	gPARMS->SetDefaultParameter("INTVETO:HIT_N_MULTI",m_N_multipleReadout,"Multiplicity for a detector with multi readout");
	gPARMS->SetDefaultParameter("INTVETO:HIT_BOTTOM_WORKAROUND",m_hit_bottom_workAround,"Workaround for bottom (component==3) that is not a multi counter but 4 singles");
}

//------------------
// init
//------------------
jerror_t IntVetoHit_factory::init(void)
{
	gPARMS->GetParameter("MC", isMC);



	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t IntVetoHit_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{


	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t IntVetoHit_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{




	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector <const IntVetoDigiHit*> m_IntVetoDigiHits;
	vector <const IntVetoDigiHit*>::const_iterator it;
	const IntVetoDigiHit* m_IntVetoDigiHit;
	IntVetoHit *m_IntVetoHit=0;


	double Q,T,Qtot,Qmax,Tmax;
	int nReadout;
	int flagOk;

	//1b: retrieve IntVetoDigiHit objects
	/*This is very important!! Select - or not - the MC case*/
	if (isMC){
		loop->Get(m_IntVetoDigiHits,"MC");
	}
	else{
		loop->Get(m_IntVetoDigiHits);
	}

	Qmax=-9999;
	Q=0;
	Qtot=0;
	for (it=m_IntVetoDigiHits.begin();it!=m_IntVetoDigiHits.end();it++){
		m_IntVetoDigiHit=(*it);
		nReadout=m_IntVetoDigiHit->m_data.size();
		if (nReadout==1){
			Q=m_IntVetoDigiHit->m_data[0].Q;
			T=m_IntVetoDigiHit->m_data[0].T;
			if (Q>m_THR_singleReadout){
				m_IntVetoHit=new IntVetoHit();
				m_IntVetoHit->m_channel = m_IntVetoDigiHit->m_channel;
				m_IntVetoHit->Q=Q;
				m_IntVetoHit->T=T;
				m_IntVetoHit->N=1; //there is only 1 readout!
				m_IntVetoHit->AddAssociatedObject(m_IntVetoDigiHit);
				_data.push_back(m_IntVetoHit);
			}
		}
		else if ((m_IntVetoDigiHit->m_channel.component==3)&&(m_hit_bottom_workAround)){ //this is the case of the bottom counter (for Catania only)
			flagOk=0;
			Qmax=-9999;
			for (int idigi=0;idigi<m_IntVetoDigiHit->m_data.size();idigi++){
				Q=m_IntVetoDigiHit->m_data[idigi].Q;
				T=m_IntVetoDigiHit->m_data[idigi].T;
				if (Q>Qmax){
					Qmax=Q;
					Tmax=T;
				}
				if (Q>m_THR_singleReadout){
					flagOk++;
				}
			}
			/*At the end of this loop, flagOK is >=1 only if there is at least one counter over m_THR_singleReadout.*/
			/*If this is true, create the IntVetoHit, and associate it the charge and the time of the maximum*/
			if (flagOk>=1){
				m_IntVetoHit=new IntVetoHit();
				m_IntVetoHit->m_channel = m_IntVetoDigiHit->m_channel;
				m_IntVetoHit->Q=Qmax;
				m_IntVetoHit->T=Tmax;
				m_IntVetoHit->N=flagOk;   /*This is the number of bars over thr (1..4). Note that bars here are NOT optically related.*/
				m_IntVetoHit->AddAssociatedObject(m_IntVetoDigiHit); /*Note that with this instruction we can always go back to the raw info*/
				_data.push_back(m_IntVetoHit);
			}
		}
		else{ /*multiCounter case*/
			flagOk=0;
			Qmax=-9999;
			Qtot=0;
			for (int idigi=0;idigi<m_IntVetoDigiHit->m_data.size();idigi++){
				Q=m_IntVetoDigiHit->m_data[idigi].Q;
				T=m_IntVetoDigiHit->m_data[idigi].T;
				if (Q>Qmax){
					Qmax=Q;
					Tmax=T;
				}
			}
			/*At the end of this loop, flagOK is the number of counters above thr*/
			/*Now use timing too: Tmax is the time of the hit with the highest charge. See if the other counters are in coincidence with this*/
			if (Qmax<m_THR_multipleReadout) continue; //if the max charge is less than the treshold, by definition this hit is irrelevant.
			for (int idigi=0;idigi<m_IntVetoDigiHit->m_data.size();idigi++){
				Q=m_IntVetoDigiHit->m_data[idigi].Q;
				T=m_IntVetoDigiHit->m_data[idigi].T;
				if ((Q>m_THR_multipleReadout)&&(fabs(T-Tmax)<m_DT_multipleReadout)){
					flagOk++;
					Qtot+=Q;
				}
			}
			/*Ok. From this point on, flagOK is the number of hits above thr, that are grouped together wrt the hit with the maximum charge*/
			if (flagOk>=m_N_multipleReadout){
				m_IntVetoHit=new IntVetoHit();
				m_IntVetoHit->m_channel = m_IntVetoDigiHit->m_channel;
				m_IntVetoHit->Q=Qtot;
				m_IntVetoHit->T=Tmax;
				m_IntVetoHit->N=flagOk;
				m_IntVetoHit->AddAssociatedObject(m_IntVetoDigiHit); /*Note that with this instruction we can always go back to the raw info*/
				_data.push_back(m_IntVetoHit);
			}
		}
	}


	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t IntVetoHit_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t IntVetoHit_factory::fini(void)
{
	return NOERROR;
}

