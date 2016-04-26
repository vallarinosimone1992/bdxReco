// $Id$
//
//    File: Calorimeterfa250Converter.h
// Created: Mon Jan 25 17:02:27 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _Calorimeterfa250Converter_
#define _Calorimeterfa250Converter_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>


#include <vector>
#include <algorithm>

#include <Calorimeter/CalorimeterSiPMHit.h>
#include <DAQ/fa250Converter.h>
#include <TT/TranslationTable.h>
#include <system/CalibrationHandler.h>
class fa250Mode1CalibPedSubHit;
class fa250Mode7Hit;
class CalorimeterCalibration;
class TMinuit;

class Calorimeterfa250Converter:public fa250Converter<CalorimeterSiPMHit>{
	public:
		JOBJECT_PUBLIC(Calorimeterfa250Converter);// Add data members here. For example:

		virtual CalorimeterSiPMHit* convertHit(const fa250Hit *hit,const TranslationTable::ChannelInfo &m_channel,int eventN=0) const;
		jerror_t convertMode1Hit(CalorimeterSiPMHit* output,const fa250Mode1CalibPedSubHit *input) const;
		jerror_t convertMode7Hit(CalorimeterSiPMHit* output,const fa250Mode7Hit *input) const;



	//	CalibrationHandler<TranslationTable::CALO_Index_t> *thr;

		double m_THR;
		double m_SINGLE_SIGNAL_TOT;
		double m_MIN_TOT;

		int m_NSB,m_NSA;
		int m_NPED;
		int m_NSAMPLES;




};

#endif // _Calorimeterfa250Converter_

