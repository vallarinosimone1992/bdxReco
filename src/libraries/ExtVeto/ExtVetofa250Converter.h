// $Id$
//
//    File: ExtVetofa250Converter.h
// Created: Mon Jan 25 16:28:59 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _ExtVetofa250Converter_
#define _ExtVetofa250Converter_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>


#include <ExtVeto/ExtVetoPMTHit.h>
#include <DAQ/fa250Converter.h>
#include <TT/TranslationTable.h>
#include <system/CalibrationHandler.h>
#include <DAQ/DAQCalibrationHandler.h>
class fa250Mode1CalibHit;
class fa250Mode7Hit;

class ExtVetofa250Converter:public fa250Converter<ExtVetoPMTHit>{
	public:
		JOBJECT_PUBLIC(ExtVetofa250Converter);
		

		virtual ExtVetoPMTHit* convertHit(const fa250Hit *hit,const TranslationTable::ChannelInfo &m_channel) const;
		jerror_t convertMode1Hit(ExtVetoPMTHit* output,const fa250Mode1CalibHit *input) const;
		jerror_t convertMode7Hit(ExtVetoPMTHit* output,const fa250Mode7Hit *input) const;

		CalibrationHandler<TranslationTable::EXT_VETO_Index_t> *threshold;
		DAQCalibrationHandler *m_pedestals;
	//	vector<vector < double> > m_rawpedestal;
};

#endif // _ExtVetofa250Converter_

