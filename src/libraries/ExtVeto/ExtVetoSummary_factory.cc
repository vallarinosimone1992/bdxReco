// $Id$
//
//    File: ExtVetoSummary_factory.cc
// Created: Fri Mar 11 16:16:37 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "ExtVetoHit.h"
#include "ExtVetoSummary_factory.h"
#include <TT/TranslationTable.h>
using namespace jana;

//------------------
// init
//------------------
jerror_t ExtVetoSummary_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t ExtVetoSummary_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t ExtVetoSummary_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	// Code to generate factory data goes here. Add it like:
	//
	// ExtVetoSummary *myExtVetoSummary = new ExtVetoSummary;
	// myExtVetoSummary->x = x;
	// myExtVetoSummary->y = y;
	// ...
	// _data.push_back(myExtVetoSummary);
	//
	// Note that the objects you create here will be deleted later
	// by the system and the _data vector will be cleared automatically.
	vector<const ExtVetoHit*> m_extVetoHits;
	vector<const ExtVetoHit*>::const_iterator it;
	const ExtVetoHit* m_extVetoHit;
	ExtVetoSummary *m_extVetoSummary;

	int sector;

	loop->Get(m_extVetoHits);

	m_map.clear();
	/*We need to handle in a different way hits corresponding to different sectors*/
	for (it=m_extVetoHits.begin();it!=m_extVetoHits.end();it++){
		m_extVetoHit=(*it);
		sector=m_extVetoHit->m_channel.sector;
		/*Check if this sector is already in the map*/
		if (m_map.find(sector)==m_map.end()){ //NO
			m_extVetoSummary=new ExtVetoSummary(); //Create it
			m_extVetoSummary->sector=sector;  //associate with the sector
			m_extVetoSummary->nHits=0;        //nhits at 0
			m_map[sector]=m_extVetoSummary;   //add it to the map, so that other hits with the same sector will find it.
		}
		else{ //YES, the object is already there
			m_extVetoSummary=m_map[sector];
		}
		/*Ok, from here on we have the m_extVetoSummary associated with the sector of this hit (if it was not existing, we create id. Otherwise, we got it)
		 * Need to check if the hit is above a given thr / is on time: if so, need to increment the nHits, to add the hit index to the hits vector, and to associate the object via AddAssociatedObject
		 */

		/*Dummy example: need to implement better selection*/
		if (m_extVetoHit->E>0){
			m_extVetoSummary->nHits++;     //increment number of hits
			m_extVetoSummary->hits.push_back(m_extVetoHit->m_channel);  //add this hit channel to the list of channels
		}

	}

	/*Add all the objects now*/
	for (m_it=m_map.begin();m_it!=m_map.end();m_it++){
		_data.push_back(m_it->second);
	}


	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t ExtVetoSummary_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t ExtVetoSummary_factory::fini(void)
{
	return NOERROR;
}

