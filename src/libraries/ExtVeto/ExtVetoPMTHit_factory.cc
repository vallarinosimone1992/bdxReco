// $Id$
//
//    File: ExtVetoDigiHit_factory.cc
// Created: Wed Jan 13 21:06:40 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "ExtVetoPMTHit_factory.h"

#include <ExtVeto/ExtVetoPMTHit.h>
#include <TT/TranslationTable.h>
using namespace jana;

//------------------
// init
//------------------
jerror_t ExtVetoPMTHit_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t ExtVetoPMTHit_factory::brun(jana::JEventLoop *eventLoop, int runnumber)
{
	jout<<"VetoIntDigiHit_factory::brun new run number: "<<runnumber<<endl;
	m_tt=0;
	eventLoop->GetSingle(m_tt);
	if (m_tt==0){
		jerr<<" unable to get the TranslationTable from this run!"<<endl;
		return OBJECT_NOT_AVAILABLE;
	}
	return NOERROR;
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t ExtVetoPMTHit_factory::evnt(JEventLoop *loop, int eventnumber)
{
	TranslationTable::ChannelInfo m_channel;
	TranslationTable::csc_t		  m_csc;
	IntVetoSiPMHit *m_ExtVetoPMTHit=0;

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
		if (m_channel.det_sys==TranslationTable::INT_VETO){

			//A.C. do not touch these
			m_ExtVetoPMTHit=new ExtVetoPMTHit;
			m_ExtVetoPMTHit->m_channel=m_channel;
			m_ExtVetoPMTHit->fa250Hit_id=(*it_fa250Mode1Hit)->id;

			//Routines to integrate charge and get time
			m_ExtVetoPMTHit->Q=(*it_fa250Mode1Hit)->samples.at(0)+(*it_fa250Mode1Hit)->samples.at(1)+(*it_fa250Mode1Hit)->samples.at(2);
			m_ExtVetoPMTHit->T=0;



			_data.push_back(m_ExtVetoPMTHit);
		}
	}


	/*Then, mode 7*/
	/*Note that in this case we do not have to integrate the pulse - it is a mode 7 pulse! */
	for (it_fa250Mode7Hit=m_fa250Mode7Hit.begin();it_fa250Mode7Hit!=m_fa250Mode7Hit.end();it_fa250Mode7Hit++){
		m_csc.rocid=(*it_fa250Mode7Hit)->crate;
		m_csc.slot=(*it_fa250Mode7Hit)->slot;
		m_csc.channel=(*it_fa250Mode7Hit)->channel;
		m_channel=m_tt->getChannelInfo(m_csc);
		if (m_channel.det_sys==TranslationTable::INT_VETO){


			//A.C. do not touch these
			m_ExtVetoPMTHit=new IntVetoSiPMHit;
			m_ExtVetoPMTHit->m_channel=m_channel;
			m_ExtVetoPMTHit->fa250Hit_id==(*it_fa250Mode1Hit)->id;


			//Routines to get charge and get time
			m_ExtVetoPMTHit->Q=0;
			m_ExtVetoPMTHit->T=0;
			_data.push_back(m_ExtVetoPMTHit);
		}
	}



	return NOERROR;


	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t ExtVetoPMTHit_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t ExtVetoPMTHit_factory::fini(void)
{
	return NOERROR;
}

