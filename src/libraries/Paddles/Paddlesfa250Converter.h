// $Id$
//
//    File: ExtVetofa250Converter.h
// Created: Mon Jan 25 16:28:59 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _Paddlesfa250Converter_
#define _Paddlesfa250Converter_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>

#include <vector>

#include <Paddles/PaddlesPMTHit.h>
#include <DAQ/fa250Converter.h>
#include <TT/TranslationTable.h>
#include <system/CalibrationHandler.h>
class fa250Mode1CalibHit;
class fa250Mode7Hit;
class PaddlesCalibration;


class Paddlesfa250Converter:public fa250Converter<PaddlesPMTHit>{
	public:
		JOBJECT_PUBLIC(Paddlesfa250Converter);
		

		virtual PaddlesPMTHit* convertHit(const fa250Hit *hit,const TranslationTable::ChannelInfo &m_channel) const;
		jerror_t convertMode1Hit(PaddlesPMTHit* output,const fa250Mode1CalibHit *input, const TranslationTable::ChannelInfo &m_channel) const;
		jerror_t convertMode7Hit(PaddlesPMTHit* output,const fa250Mode7Hit *input) const;

		CalibrationHandler<TranslationTable::PADDLES_Index_t> *threshold,*pedestal_init;

};

#endif // _Paddlesfa250Converter_

