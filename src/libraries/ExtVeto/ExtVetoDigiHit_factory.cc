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

#include <TT/TranslationTable.h>

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
jerror_t ExtVetoDigiHit_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	jout<<"ExtVetoDigiHit_factory::brun new run number: "<<runnumber<<endl;
	m_tt=0;
	eventLoop->GetSingle(m_tt);
	if (m_tt==0){
		jerr<<" unable to get the TranslationTable from this run!"<<endl;
		return OBJECT_NOT_AVAILABLE;
	}
	return NOERROR;

}

//------------------
// evnt
//------------------
jerror_t ExtVetoDigiHit_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	vector <const ExtVetoPMTHit *> m_data;
	vector <const ExtVetoPMTHit *>::const_iterator m_it;

	loop->Get(m_data);

	/*Create here the digi hit from the PMT hit*/
	ExtVetoDigiHit *m_ExtVetoDigiHit=0;

	for (m_it=m_data.begin();m_it!=m_data.end();m_it++){
		m_ExtVetoDigiHit=new ExtVetoDigiHit;

		/*For now, very dummy!*/
		m_ExtVetoDigiHit->m_channel=(*m_it)->m_channel.ext_veto;
		m_ExtVetoDigiHit->m_channel.readout=0;  //since this is detector-based!
		m_ExtVetoDigiHit->Q=(*m_it)->Q;
		m_ExtVetoDigiHit->T=(*m_it)->T;
		m_ExtVetoDigiHit->ExtVetoPMTHit_id=(*m_it)->id;

		_data.push_back(m_ExtVetoDigiHit);
	}



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

