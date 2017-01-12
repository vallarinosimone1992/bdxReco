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

	m_ENE_gain=new CalibrationHandler<TranslationTable::EXT_VETO_Index_t>("/ExtVeto/Ene");
	this->mapCalibrationHandler(m_ENE_gain);

	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t ExtVetoHit_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{


	this->updateCalibrationHandler(m_ENE_gain,eventLoop);

	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t ExtVetoHit_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{


	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector <const ExtVetoDigiHit*> m_ExtVetoDigiHit;
	vector <const ExtVetoDigiHit*>::const_iterator it;

	double m_Ene;

	ExtVetoHit *m_ExtVetoHit=0;

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
		m_Ene=m_ENE_gain->getCalibSingle(m_ExtVetoHit->m_channel);
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

	this->clearCalibrationHandler(m_ENE_gain);

	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t ExtVetoHit_factory::fini(void)
{
	return NOERROR;
}

