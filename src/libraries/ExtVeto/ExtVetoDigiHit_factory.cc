// $Id$
//
//    File: ExtVetoDigiHit_factory.cc
// Created: Wed Jan 20 18:22:52 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "ExtVetoDigiHit_factory.h"
#include <ExtVeto/ExtVetoPMTHit.h>
using namespace jana;

//------------------
// init
//------------------
jerror_t ExtVetoDigiHit_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t ExtVetoDigiHit_factory::brun(jana::JEventLoop *eventLoop, int runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t ExtVetoDigiHit_factory::evnt(JEventLoop *loop, int eventnumber)
{

	vector <const ExtVetoPMTHit *> m_data;
	vector <const ExtVetoPMTHit *>::const_iterator m_it;

	loop->Get(m_data);



	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t ExtVetoDigiHit_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t ExtVetoDigiHit_factory::fini(void)
{
	return NOERROR;
}

