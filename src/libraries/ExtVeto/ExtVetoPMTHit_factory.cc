// $Id$
//
//    File: ExtVetoDigiHit_factory.cc
// Created: Wed Jan 13 21:06:40 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

//objects we need from the framework
#include <DAQ/fa250Mode1CalibHit.h>
#include <DAQ/fa250Mode7Hit.h>
#include <TT/TranslationTable.h>
//objects we put in the framework
#include <ExtVeto/ExtVetoPMTHit.h>
#include <ExtVeto/ExtVetofa250Converter.h>

#include "ExtVetoPMTHit_factory.h"
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
jerror_t ExtVetoPMTHit_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	jout<<"VetoExtDigiHit_factory::brun new run number: "<<runnumber<<endl;
	m_tt=0;
	eventLoop->GetSingle(m_tt);
	if (m_tt==0){
		jerr<<" unable to get the TranslationTable from this run!"<<endl;
		return OBJECT_NOT_AVAILABLE;
	}

	m_extVetofa250Converter=0;
	eventLoop->GetSingle(m_extVetofa250Converter);
	if (m_extVetofa250Converter==0){
		jerr<<" unable to get the extVetofa250Converter!"<<endl;
		return OBJECT_NOT_AVAILABLE;
	}

	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t ExtVetoPMTHit_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{
	TranslationTable::ChannelInfo m_channel;
	TranslationTable::csc_t		  m_csc;
	ExtVetoPMTHit *m_ExtVetoPMTHit=0;

	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector <const fa250Mode1CalibHit*> m_fa250Mode1CalibHit;
	vector <const fa250Mode7Hit*> m_fa250Mode7Hit;
	vector <const fa250Mode1CalibHit*>::const_iterator it_fa250Mode1CalibHit;
	vector <const fa250Mode7Hit*>::const_iterator it_fa250Mode7Hit;

	//1b: retrieve objects
	loop->Get(m_fa250Mode1CalibHit);
	loop->Get(m_fa250Mode7Hit);

	/*2: Now we have the daq objects, still indexed as "crate-slot-channel"
	 *	 Use the translation table to produce the digitized hit of the inner veto
	 *	 Note that we can produce a single object type here, i.e. ExtVetoPMTHit,
	 *	 but we have 2 possible source, mode 1 and mode 7.
	 *	 Therefore, we will use the TranslationTable ONLY to check if this crate-slot-channel
	 *	 combination refers to a InnerVeto hit and, in case, to determine which one, i.e. which ID in the InnerVeto scheme.
	 *	 Then, we will proceed in two different ways.
	 */
	int mz=0;
	int mz2=0;
	/*First, mode 1*/
	/*Note that in this case we have to integrate the pulse - it is a mode 1 pulse! */
	for (it_fa250Mode1CalibHit=m_fa250Mode1CalibHit.begin();it_fa250Mode1CalibHit!=m_fa250Mode1CalibHit.end();it_fa250Mode1CalibHit++){
		m_csc.rocid=(*it_fa250Mode1CalibHit)->crate;
		m_csc.slot=(*it_fa250Mode1CalibHit)->slot;
		m_csc.channel=(*it_fa250Mode1CalibHit)->channel;
		m_channel=m_tt->getChannelInfo(m_csc);
		mz++;
		jout<<mz<<endl;
        jout<<"M CHANNEL= "<<m_channel.det_sys<<endl;

		if (m_channel.det_sys==TranslationTable::EXT_VETO){
	        jout<<"M CHANNEL EXT VETO= "<<m_channel.det_sys<<endl;

			//A.C. do not touch these
			mz2++;
					jout<<"MZ2 "<<mz2<<endl;
						m_ExtVetoPMTHit=new ExtVetoPMTHit;
						m_ExtVetoPMTHit->m_channel=m_channel;
						m_ExtVetoPMTHit=m_extVetofa250Converter->convertHit((fa250Hit*)*it_fa250Mode1CalibHit,m_channel);
				        jout<<"M CHANNEL EXT VETO 2 = "<<m_channel.det_sys<<endl;

						m_ExtVetoPMTHit->AddAssociatedObject(*it_fa250Mode1CalibHit);
						_data.push_back(m_ExtVetoPMTHit);
					}


			m_ExtVetoPMTHit=m_extVetofa250Converter->convertHit((fa250Hit*)*it_fa250Mode1CalibHit,m_channel);
			m_ExtVetoPMTHit->AddAssociatedObject(*it_fa250Mode1CalibHit);
			_data.push_back(m_ExtVetoPMTHit);
		}



	/*Then, mode 7*/
	/*Note that in this case we do not have to integrate the pulse - it is a mode 7 pulse! */
	for (it_fa250Mode7Hit=m_fa250Mode7Hit.begin();it_fa250Mode7Hit!=m_fa250Mode7Hit.end();it_fa250Mode7Hit++){
		m_csc.rocid=(*it_fa250Mode7Hit)->crate;
		m_csc.slot=(*it_fa250Mode7Hit)->slot;
		m_csc.channel=(*it_fa250Mode7Hit)->channel;

		m_channel=m_tt->getChannelInfo(m_csc);
		if (m_channel.det_sys==TranslationTable::EXT_VETO){
			//A.C. do not touch these
						m_ExtVetoPMTHit=new ExtVetoPMTHit;
						m_ExtVetoPMTHit->m_channel=m_channel;
						m_ExtVetoPMTHit=m_extVetofa250Converter->convertHit((fa250Hit*)*it_fa250Mode7Hit,m_channel);
						m_ExtVetoPMTHit->AddAssociatedObject(*it_fa250Mode7Hit);
						_data.push_back(m_ExtVetoPMTHit);

			m_ExtVetoPMTHit=m_extVetofa250Converter->convertHit((fa250Hit*)*it_fa250Mode7Hit,m_channel);
			m_ExtVetoPMTHit->AddAssociatedObject(*it_fa250Mode7Hit);
			_data.push_back(m_ExtVetoPMTHit);

		}
	}


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

