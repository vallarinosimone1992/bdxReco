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
		

		virtual T* convertHit(const fa250Hit *hit,const TranslationTable::ChannelInfo &m_channel)const=0; //also this one is purely virtual
		
		/*Add here some "common" member functions*/
		double sumSamples(vector<double> &samples) const;
		
};






template<class T> double fa250Converter< T >::sumSamples(vector<double> &samples) const{
	double ret=0;
	for (int ii=0;ii<samples.size();ii++) ret+=samples[ii];
	return ret;
}


#endif // _FadcConverter_

