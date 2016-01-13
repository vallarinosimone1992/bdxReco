// $Id$
//
//    File: VetoIntDigiHit_factory.cc
// Created: Tue Jan 12 11:52:41 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

//objects we need from the framework
#include <DAQ/fa250Mode1Hit.h>
#include <DAQ/fa250Mode7Hit.h>
#include <TT/TranslationTable.h>
//objects we put in the framework
#include <VETO_INT/VetoIntDigiHit.h>


#include "VetoIntDigiHit_factory.h"
using namespace jana;

//------------------
// init
//------------------
jerror_t VetoIntDigiHit_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t VetoIntDigiHit_factory::brun(jana::JEventLoop *eventLoop, int runnumber)
{
	jout<<"VetoIntDigiHit_factory::brun new run number: "<<runnumber<<endl;
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
jerror_t VetoIntDigiHit_factory::evnt(JEventLoop *loop, int eventnumber)
{
	TranslationTable::ChannelInfo m_channel;
	TranslationTable::csc_t		  m_csc;
	VetoIntDigiHit *m_VetoIntDigiHit=0;

	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector <const fa250Mode1Hit*> m_fa250Mode1Hit;
	vector <const fa250Mode7Hit*> m_fa250Mode7Hit;
	vector <const fa250Mode1Hit*>::const_iterator it_fa250Mode1Hit;
	vector <const fa250Mode7Hit*>::const_iterator it_fa250Mode7Hit;

	//1b: retrieve objects
	loop->Get(m_fa250Mode1Hit);
	loop->Get(m_fa250Mode7Hit);

	/*2: Now we have the daq objects, still indexed as "crate-slot-channel"
	 *	 Use the translation table to produce the digitized hit of the inner veto
	 *	 Note that we can produce a single object type here, i.e. VetoIntDigiHit,
	 *	 but we have 2 possible source, mode 1 and mode 7.
	 *	 Therefore, we will use the TranslationTable ONLY to check if this crate-slot-channel
	 *	 combination refers to a InnerVeto hit and, in case, to determine which one, i.e. which ID in the InnerVeto scheme.
	 *	 Then, we will proceed in two different ways.
	 */

	/*First, mode 1*/
	/*Note that in this case we have to integrate the pulse - it is a mode 1 pulse! */
	for (it_fa250Mode1Hit=m_fa250Mode1Hit.begin();it_fa250Mode1Hit!=m_fa250Mode1Hit.end();it_fa250Mode1Hit++){
		m_csc.rocid=(*it_fa250Mode1Hit)->crate;
		m_csc.slot=(*it_fa250Mode1Hit)->slot;
		m_csc.channel=(*it_fa250Mode1Hit)->channel;

		m_channel=m_tt->getChannelInfo(m_csc);

		//jout<<m_csc.rocid<<" "<<m_csc.slot<<" "<<m_csc.channel<<" "<<endl;
		if (m_channel.det_sys==TranslationTable::VETO_INT){

			m_VetoIntDigiHit=new VetoIntDigiHit;
			m_VetoIntDigiHit->m_channel=m_channel;

			m_VetoIntDigiHit->Q=(*it_fa250Mode1Hit)->samples.at(0)+(*it_fa250Mode1Hit)->samples.at(1)+(*it_fa250Mode1Hit)->samples.at(2);
			m_VetoIntDigiHit->T=0;
			_data.push_back(m_VetoIntDigiHit);
		}
	}


	/*Then, mode 7*/
	/*Note that in this case we do not have to integrate the pulse - it is a mode 7 pulse! */
	for (it_fa250Mode7Hit=m_fa250Mode7Hit.begin();it_fa250Mode7Hit!=m_fa250Mode7Hit.end();it_fa250Mode7Hit++){
		m_csc.rocid=(*it_fa250Mode7Hit)->crate;
		m_csc.slot=(*it_fa250Mode7Hit)->slot;
		m_csc.channel=(*it_fa250Mode7Hit)->channel;
		m_channel=m_tt->getChannelInfo(m_csc);
		if (m_channel.det_sys==TranslationTable::VETO_INT){
			m_VetoIntDigiHit->m_channel=m_channel;

			m_VetoIntDigiHit->Q=0;
			m_VetoIntDigiHit->T=0;
			_data.push_back(m_VetoIntDigiHit);
		}
	}



	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t VetoIntDigiHit_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t VetoIntDigiHit_factory::fini(void)
{
	return NOERROR;
}

