// $Id$
//
//    File: fa250Mode1PedSubHit_factory.cc
// Created: Wed Feb  3 07:38:15 EST 2016
// Creator: davidl (on Darwin harriet.jlab.org 13.4.0 i386)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "fa250Mode1CalibHit.h"
#include "fa250Mode1PedSubHit_factory.h"
#include <TT/TranslationTable.h>
using namespace jana;

//------------------
// init
//------------------
jerror_t fa250Mode1PedSubHit_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t fa250Mode1PedSubHit_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	// Here, we would normally get this from the CalibPedSubration DB.
	// For now, we hard code it...

	m_pedestals=new DAQCalibrationHandler();
	vector<vector < double> > m_rawpedestal;
	eventLoop->GetCalib("/DAQ/pedestals",m_rawpedestal);
	m_pedestals->fillCalib(m_rawpedestal);




	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t fa250Mode1PedSubHit_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{
	vector<const fa250Mode1Hit*> hits;
	double pedestal;
	double sample;
	TranslationTable::csc_t index;
	loop->Get(hits);



	for(uint32_t i=0; i<hits.size(); i++){

		const fa250Mode1Hit *hit = hits[i];

		// Create new fa250Mode1PedSubHit
		fa250Mode1PedSubHit *PedSubHit = new fa250Mode1PedSubHit;

		// Copy the fa250Hit part (crate, slot, channel, ...)
		// doing it this way allow one to modify fa250 later and
		// not have to change this code.
		fa250Hit *a = PedSubHit;
		const fa250Hit *b = hit;
		*a = *b;

		// Copy all samples, applying PedSubration constant as we go
		for(uint32_t j=0; j<hit->samples.size(); j++){
			sample = (double)hit->samples[j];
			m_pedestals->getCalibSingle(hit->m_channel,pedestal);
			sample = sample - pedestal;
			PedSubHit->samples.push_back(sample);
		}

		// Add original as associated object 
		PedSubHit->AddAssociatedObject(hit);
		_data.push_back(PedSubHit);
	}

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t fa250Mode1PedSubHit_factory::erun(void)
{
	if (m_pedestals) delete m_pedestals;
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t fa250Mode1PedSubHit_factory::fini(void)
{
	if (m_pedestals) delete m_pedestals;
	return NOERROR;
}

