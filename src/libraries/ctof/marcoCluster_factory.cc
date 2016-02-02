// $Id$
//
//    File: marcoCluster_factory.cc
// Created: Thu Dec 17 16:37:25 EST 2015
// Creator: ungaro (on Darwin MauriMP.local 14.5.0 i386)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "marcoCluster_factory.h"
using namespace jana;


#include "ctofHitR.h"




//------------------
// init
//------------------
jerror_t marcoCluster_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t marcoCluster_factory::brun(jana::JEventLoop *eventLoop, int runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t marcoCluster_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	
	vector<const ctofHitR*> chits;
	loop->Get(chits);

	
	

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t marcoCluster_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t marcoCluster_factory::fini(void)
{
	return NOERROR;
}

