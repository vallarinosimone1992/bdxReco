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
	vector<vector < double> > m_rawcalib;
	eventLoop->GetCalib("/InnerVeto/Ene", m_rawcalib);
	m_ene.fillCalib(m_rawcalib);
	gPARMS->GetParameter("INTVETO:VERBOSE",VERBOSE);
	if (VERBOSE>3){
		std::map  < TranslationTable::INT_VETO_Index_t, std::vector < double > > eneMap;
		std::map  < TranslationTable::INT_VETO_Index_t, std::vector < double > >::iterator eneMap_it;
		eneMap=m_ene.getCalibMap();
		jout<<"Got following ene for run number: "<<runnumber<<endl;
		jout<<"Rows: "<<eneMap.size()<<endl;
		for (eneMap_it=eneMap.begin();eneMap_it!=eneMap.end();eneMap_it++){
			jout<<eneMap_it->first.sector<<" "<<eneMap_it->first.layer<<" "<<eneMap_it->first.component<<" "<<eneMap_it->first.readout<<" "<<eneMap_it->second.at(0)<<endl;
		}
	}

	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t IntVetoHit_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	// Code to generate factory data goes here. Add it like:
	//
	// IntVetoHit *myIntVetoHit = new IntVetoHit;
	// myIntVetoHit->x = x;
	// myIntVetoHit->y = y;
	// ...
	// _data.push_back(myIntVetoHit);
	//
	// Note that the objects you create here will be deleted later
	// by the system and the _data vector will be cleared automatically.



	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector <const IntVetoDigiHit*> m_IntVetoDigiHits;
	vector <const IntVetoDigiHit*>::const_iterator it;
	const IntVetoDigiHit* m_IntVetoDigiHit;
	IntVetoHit *m_IntVetoHit=0;


	double Q,T,E,Qmax,Tmax,m_E_calib;


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
	E=0;
	for (it=m_IntVetoDigiHits.begin();it!=m_IntVetoDigiHits.end();it++){
		m_IntVetoDigiHit=(*it);
		m_IntVetoHit=new IntVetoHit();
		m_IntVetoHit->m_channel = m_IntVetoDigiHit->m_channel;

		//Do whatever you need here
		for (int idigi=0;idigi<m_IntVetoDigiHit->m_data.size();idigi++){
			Q=m_IntVetoDigiHit->m_data[idigi].Q;
			T=m_IntVetoDigiHit->m_data[idigi].T;
			E+=Q;
			if (Q>Qmax){
				Qmax=m_IntVetoDigiHit->m_data[idigi].Q;
				Tmax=T;
			}
		}

		/*Apply cal. constant if possible*/
		m_E_calib=m_ene.getCalibSingle(m_IntVetoHit->m_channel);


		if ((m_E_calib>0)){
			E =	E/m_E_calib;
		}


		m_IntVetoHit->T=T;
		m_IntVetoHit->E=E;

		_data.push_back(m_IntVetoHit);
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

