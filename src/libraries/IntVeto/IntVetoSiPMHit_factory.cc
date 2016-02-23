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
#include <DAQ/fa250Mode1CalibHit.h>
#include <DAQ/fa250Mode7Hit.h>
#include <TT/TranslationTable.h>
//objects we put in the framework
#include <IntVeto/IntVetoSiPMHit.h>
#include <IntVeto/IntVetofa250Converter.h>

#include "IntVetoSiPMHit_factory.h"
using namespace jana;

//------------------
// init
//------------------
jerror_t IntVetoSiPMHit_factory::init(void)
{
	return NOERROR;

	VERBOSE=0;

}

//------------------
// brun
//------------------
jerror_t IntVetoSiPMHit_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	jout<<"VetoIntSiPMHit_factory::brun new run number: "<<runnumber<<endl;
	m_tt=0;
	eventLoop->GetSingle(m_tt);
	if (m_tt==0){
		jerr<<" unable to get the TranslationTable from this run!"<<endl;
		return OBJECT_NOT_AVAILABLE;
	}

	m_intVetofa250Converter=0;
	eventLoop->GetSingle(m_intVetofa250Converter);
	if (m_intVetofa250Converter==0){
		jerr<<" unable to get the intVetofa250Converter!"<<endl;
		return OBJECT_NOT_AVAILABLE;
	}

	vector<vector < double> > m_rawcalib;
	eventLoop->GetCalib("/InnerVeto/sipm_gain", m_rawcalib);
	m_sipm_gain.fillCalib(m_rawcalib);
	gPARMS->GetParameter("INTVETO:VERBOSE",VERBOSE);
	if (VERBOSE>3){
		std::map  < TranslationTable::INT_VETO_Index_t, std::vector < double > > gainCalibMap;
		std::map  < TranslationTable::INT_VETO_Index_t, std::vector < double > >::iterator gainCalibMap_it;
		gainCalibMap=m_sipm_gain.getCalibMap();
		jout<<"Got following sipm_gain for run number: "<<runnumber<<endl;
		jout<<"Rows: "<<gainCalibMap.size()<<endl;
		for (gainCalibMap_it=gainCalibMap.begin();gainCalibMap_it!=gainCalibMap.end();gainCalibMap_it++){
			jout<<gainCalibMap_it->first.sector<<" "<<gainCalibMap_it->first.layer<<" "<<gainCalibMap_it->first.component<<" "<<gainCalibMap_it->first.readout<<" "<<gainCalibMap_it->second.at(0)<<endl;
		}
	}

	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t IntVetoSiPMHit_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{
	TranslationTable::ChannelInfo m_channel;
	TranslationTable::csc_t		  m_csc;
	vector<double> 				  m_q_calib;
	IntVetoSiPMHit *m_IntVetoSiPMHit=0;

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
	 *	 Note that we can produce a single object type here, i.e. VetoIntDigiHit,
	 *	 but we have 2 possible source, mode 1 and mode 7.
	 *	 Therefore, we will use the TranslationTable ONLY to check if this crate-slot-channel
	 *	 combination refers to a InnerVeto hit and, in case, to determine which one, i.e. which ID in the InnerVeto scheme.
	 *	 Then, we will proceed in two different ways.
	 */

	/*First, mode 1*/
	/*Note that in this case we have to integrate the pulse - it is a mode 1 pulse! */
	for (it_fa250Mode1CalibHit=m_fa250Mode1CalibHit.begin();it_fa250Mode1CalibHit!=m_fa250Mode1CalibHit.end();it_fa250Mode1CalibHit++){
		m_csc.rocid=(*it_fa250Mode1CalibHit)->crate;
		m_csc.slot=(*it_fa250Mode1CalibHit)->slot;
		m_csc.channel=(*it_fa250Mode1CalibHit)->channel;

		m_channel=m_tt->getChannelInfo(m_csc);


		if (m_channel.det_sys==TranslationTable::INT_VETO){
			//A.C. do not touch these
			m_IntVetoSiPMHit=m_intVetofa250Converter->convertHit((fa250Hit*)*it_fa250Mode1CalibHit,m_channel);
			//m_IntVetoSiPMHit->AddAssociatedObject(*it_fa250Mode1CalibHit);

			/*Apply phe conversion if possible*/
			m_sipm_gain.getCalib(m_channel.int_veto,m_q_calib);
			if ((m_q_calib.size()==1)&&(m_q_calib.at(0)>0)){
				m_IntVetoSiPMHit->Qphe=	m_IntVetoSiPMHit->Qraw/m_q_calib.at(0);
			}

			_data.push_back(m_IntVetoSiPMHit);
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
			m_IntVetoSiPMHit=m_intVetofa250Converter->convertHit((fa250Hit*)*it_fa250Mode7Hit,m_channel);

			/*Apply phe conversion if possible*/
			m_sipm_gain.getCalib(m_channel.int_veto,m_q_calib);
			if ((m_q_calib.size()==1)&&(m_q_calib.at(0)>0)){
				m_IntVetoSiPMHit->Qphe=	m_IntVetoSiPMHit->Qraw/m_q_calib.at(0);
			}

			_data.push_back(m_IntVetoSiPMHit);
		}
	}



	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t IntVetoSiPMHit_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t IntVetoSiPMHit_factory::fini(void)
{
	return NOERROR;
}

