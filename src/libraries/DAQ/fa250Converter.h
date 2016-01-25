// $Id$
//
//    File: FadcConverter.h
// Created: Wed Jan 20 19:27:33 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _FadcConverter_
#define _FadcConverter_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>

#include <DAQ/fa250Hit.h>
#include <TT/TranslationTable.h>

template<class T> class fa250Converter:public jana::JObject{
	public:
		JOBJECT_PUBLIC(fa250Converter);
		

		virtual T* convertHit(const fa250Hit *hit,const TranslationTable::ChannelInfo &m_channel) const=0; //also this one is purely virtual
		
		
};

#endif // _FadcConverter_

