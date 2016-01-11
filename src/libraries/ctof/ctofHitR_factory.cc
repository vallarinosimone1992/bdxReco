// $Id$
//
//    File: ctofHit_factory.cc
// Created: Thu Dec 17 14:09:30 EST 2015
// Creator: ungaro (on Darwin MauriMP.local 14.5.0 i386)
//


#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

#include "CTOFhit.h"

#include "ctofHitR.h"
#include "ctofHitR_factory.h"
using namespace jana;




// init
jerror_t ctofHitR_factory::init(void)
{
	cout << " Initizalizing CTOF factory "  << endl;

	return NOERROR;
}

// brun
jerror_t ctofHitR_factory::brun(jana::JEventLoop *eventLoop, int runnumber)
{

	return NOERROR;
}

// evnt
jerror_t ctofHitR_factory::evnt(JEventLoop *loop, int eventnumber)
{
	
	
	vector<const CTOFhit*> chits;
	loop->Get(chits);

	
	for(unsigned h=0; h<chits.size(); h++)
	{

		ctofHitR *thisHit = new ctofHitR(sqrt(chits[h]->ADCL * chits[h]->ADCR)/100);
		thisHit->paddle = chits[h]->paddle;

		// publish the cluster
		_data.push_back(thisHit);
	}
	

	return NOERROR;
}

// erun
jerror_t ctofHitR_factory::erun(void)
{
	return NOERROR;
}

// fini
jerror_t ctofHitR_factory::fini(void)
{
	return NOERROR;
}

