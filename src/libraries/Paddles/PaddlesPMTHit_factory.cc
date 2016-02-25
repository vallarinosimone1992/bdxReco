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
#include <Paddles/PaddlesPMTHit.h>
#include <Paddles/Paddlesfa250Converter.h>

#include "PaddlesPMTHit_factory.h"
//#include <Paddles/PaddlesPMTHit_factory.h>

using namespace jana;

//------------------
// init
//------------------
jerror_t PaddlesPMTHit_factory::init(void)
{
	return NOERROR;

	VERBOSE=0;

}

//------------------
// brun
//------------------
jerror_t PaddlesPMTHit_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	jout<<"PaddlesPMTHit_factory::brun new run number: "<<runnumber<<endl;
	m_tt=0;
	eventLoop->GetSingle(m_tt);
	if (m_tt==0){
		jerr<<" unable to get the TranslationTable from this run!"<<endl;
		return OBJECT_NOT_AVAILABLE;
	}

	m_Paddlesfa250Converter=0;
	eventLoop->GetSingle(m_Paddlesfa250Converter);
	if (m_Paddlesfa250Converter==0){
		jerr<<" unable to get the Paddlesfa250Converter!"<<endl;
		return OBJECT_NOT_AVAILABLE;
	}

	vector<vector < double> > m_rawcalib;
		eventLoop->GetCalib("/Paddles/PMT_gain", m_rawcalib);
		m_PMT_gain.fillCalib(m_rawcalib);
		gPARMS->GetParameter("PADDLES:VERBOSE",VERBOSE);
		if (VERBOSE>3){
			std::map  < TranslationTable::PADDLES_Index_t, std::vector < double > > gainCalibMap;
			std::map  < TranslationTable::PADDLES_Index_t, std::vector < double > >::iterator gainCalibMap_it;
			gainCalibMap=m_PMT_gain.getCalibMap();
			jout<<"Got following PMT_gain for run number: "<<runnumber<<endl;
			jout<<"Rows: "<<gainCalibMap.size()<<endl;
			for (gainCalibMap_it=gainCalibMap.begin();gainCalibMap_it!=gainCalibMap.end();gainCalibMap_it++){
				jout<<gainCalibMap_it->first.id<<" "<<gainCalibMap_it->second.at(0)<<endl;
			}
		}

	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t PaddlesPMTHit_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{
	TranslationTable::ChannelInfo m_channel;
//	TranslationTable::csc_t		  m_csc;
	vector<double> 				  m_q_calib;

	PaddlesPMTHit *m_PaddlesPMTHit=0;



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
	/*First, mode 1*/
	/*Note that in this case we have to integrate the pulse - it is a mode 1 pulse! */

	for (it_fa250Mode1CalibHit=m_fa250Mode1CalibHit.begin();it_fa250Mode1CalibHit!=m_fa250Mode1CalibHit.end();it_fa250Mode1CalibHit++){
		m_channel=m_tt->getChannelInfo((*it_fa250Mode1CalibHit)->m_channel);

		if (m_channel.det_sys==TranslationTable::PADDLES){
						m_PaddlesPMTHit=m_Paddlesfa250Converter->convertHit((fa250Hit*)*it_fa250Mode1CalibHit,m_channel);
						m_PaddlesPMTHit->AddAssociatedObject(*it_fa250Mode1CalibHit);
//						jout<<"Q= "<<m_PaddlesPMTHit->Q<<endl;

						/*Apply phe conversion */
															m_PMT_gain.getCalib(m_channel.paddles,m_q_calib);
//															jout<<"**********"<<endl;
//															jout<<m_q_calib.size()<<endl;
//															jout<<m_q_calib.at(0)<<" "<<endl;
															if ((m_q_calib.size()==1)&&(m_q_calib.at(0)>0)){
																m_PaddlesPMTHit->Q/=((1.602*1E-19)*1E9);	// number of electrons at the exit of the PMT
																m_PaddlesPMTHit->Q/=m_q_calib.at(0);		// number of phe
															}

						_data.push_back(m_PaddlesPMTHit);

					}
		}



	/*Then, mode 7*/
	/*Note that in this case we do not have to integrate the pulse - it is a mode 7 pulse! */
	for (it_fa250Mode7Hit=m_fa250Mode7Hit.begin();it_fa250Mode7Hit!=m_fa250Mode7Hit.end();it_fa250Mode7Hit++){

		m_channel=m_tt->getChannelInfo((*it_fa250Mode7Hit)->m_channel);

		if (m_channel.det_sys==TranslationTable::PADDLES){
			//A.C. do not touch these
						m_PaddlesPMTHit=m_Paddlesfa250Converter->convertHit((fa250Hit*)*it_fa250Mode7Hit,m_channel);
						m_PaddlesPMTHit->AddAssociatedObject(*it_fa250Mode7Hit);

						/*Apply phe conversion */
																					m_PMT_gain.getCalib(m_channel.paddles,m_q_calib);
						//															jout<<"**********"<<endl;
						//															jout<<m_q_calib.size()<<endl;
						//															jout<<m_q_calib.at(0)<<" "<<endl;
																					if ((m_q_calib.size()==1)&&(m_q_calib.at(0)>0)){
																						m_PaddlesPMTHit->Q/=((1.602*1E-19)*1E9);	// number of electrons at the exit of the PMT
																						m_PaddlesPMTHit->Q/=m_q_calib.at(0);		// number of phe
						//																jout<<"Q= "<<m_PaddlesPMTHit->Q<<endl;
																					}



						_data.push_back(m_PaddlesPMTHit);

		}
	}


	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t PaddlesPMTHit_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t PaddlesPMTHit_factory::fini(void)
{
	return NOERROR;
}

