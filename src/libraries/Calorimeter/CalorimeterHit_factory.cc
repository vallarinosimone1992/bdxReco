// $Id$
//
//    File: CalorimeterHit_factory.cc
// Created: Wed Jan 27 16:40:00 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "CalorimeterHit_factory.h"

//objects we need from the framework
#include <TT/TranslationTable.h>
#include <Calorimeter/CalorimeterDigiHit.h>
//objects we put in the framework
#include <Calorimeter/CalorimeterHit.h>


using namespace jana;

//------------------
// init
//------------------
jerror_t CalorimeterHit_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t CalorimeterHit_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t CalorimeterHit_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{


	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector <const CalorimeterDigiHit*> m_CalorimeterDigiHit;
	vector <const CalorimeterDigiHit*>::const_iterator it;


	vector <CalorimeterDigiHit::CalorimeterSiPMDigiHit>::const_iterator digi_it;

	CalorimeterHit *m_CalorimeterHit=0;

	int readout;
	double Q,T;

	//1b: retrieve CalorimeterSiPMHit objects
	loop->Get(m_CalorimeterDigiHit);

	for (it=m_CalorimeterDigiHit.begin();it!=m_CalorimeterDigiHit.end();it++){
		m_CalorimeterHit=new CalorimeterHit();
		m_CalorimeterHit->m_channel = (*it)->m_channel;
		m_CalorimeterHit->Q1=0;
		m_CalorimeterHit->Q2=0;
		/*Now loop over the digi-hits of the DigiHit object. Not so easy, but necessary for MC compatibility*/
		if ((*it)->m_data.size()>2){
			jerr<<"CalormeterHit_factory::evnt error, a CalorimeterDigiHit has more than 2 entries in the data vector??"<<endl;
		}
		for (digi_it=(*it)->m_data.begin();digi_it!=(*it)->m_data.end();digi_it++){
			if ((*digi_it).readout==1){
				m_CalorimeterHit->Q1=(*digi_it).Q;
				m_CalorimeterHit->T1=(*digi_it).T;
			}
			else if ((*digi_it).readout==2){
				m_CalorimeterHit->Q2=(*digi_it).Q;
				m_CalorimeterHit->T2=(*digi_it).T;
			}
		}

		/*For now*/
		m_CalorimeterHit->Q=m_CalorimeterHit->Q1+m_CalorimeterHit->Q2;


		_data.push_back(m_CalorimeterHit);
	}

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t CalorimeterHit_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t CalorimeterHit_factory::fini(void)
{
	return NOERROR;
}

