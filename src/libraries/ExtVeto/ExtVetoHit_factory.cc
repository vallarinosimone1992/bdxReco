// $Id$
//
//    File: ExtVetoHit_factory.cc
// Created: Wed Jan 13 21:06:44 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "ExtVetoDigiHit.h"
//#include "ExtVetoHit.h"

#include "ExtVetoHit_factory.h"
#include <TT/TranslationTable.h>

using namespace jana;

//------------------
// init
//------------------
jerror_t ExtVetoHit_factory::init(void)
{
	gPARMS->GetParameter("MC",isMC);
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t ExtVetoHit_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	vector<vector < double> > m_enecalib;
		eventLoop->GetCalib("/ExtVeto/Ene", m_enecalib);
		m_ENE_gain.fillCalib(m_enecalib);
		return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t ExtVetoHit_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	// Code to generate factory data goes here. Add it like:
	//
	// ExtVetoHit *myExtVetoHit = new ExtVetoHit;
	// myExtVetoHit->x = x;
	// myExtVetoHit->y = y;
	// ...
	// _data.push_back(myExtVetoHit);
	//
	// Note that the objects you create here will be deleted later
	// by the system and the _data vector will be cleared automatically.


	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector <const ExtVetoDigiHit*> m_ExtVetoDigiHit;
	vector <const ExtVetoDigiHit*>::const_iterator it;

		double m_Ene;

	ExtVetoHit *m_ExtVetoHit=0;

//	int readout;
//	double Q,T;

	//1b: retrieve ExtVetoDigiHit objects

	/*This is very important!! Select - or not - the MC case*/
	if (isMC){
		loop->Get(m_ExtVetoDigiHit,"MC");
	}
	else{
		loop->Get(m_ExtVetoDigiHit);
	}

	for (it=m_ExtVetoDigiHit.begin();it!=m_ExtVetoDigiHit.end();it++){
		m_ExtVetoHit=new ExtVetoHit();
		m_ExtVetoHit->m_channel = (*it)->m_channel;

		m_Ene=m_ENE_gain.getCalibSingle(m_ExtVetoHit->m_channel);

		//jout <<m_ExtVetoHit->m_channel.component << " "<< m_Ene<<endl;
					m_ExtVetoHit->E=((*it)->Q)*m_Ene;
					m_ExtVetoHit->T=(*it)->T;

					m_ExtVetoHit->AddAssociatedObject(*it);



		//Do whatever you need here

		_data.push_back(m_ExtVetoHit);
	}



	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t ExtVetoHit_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t ExtVetoHit_factory::fini(void)
{
	return NOERROR;
}

