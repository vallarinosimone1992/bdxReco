// $Id$
//
//    File: VetoIntDigiHit.h
// Created: Tue Jan 12 11:52:41 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _VetoIntDigiHit_
#define _VetoIntDigiHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>

#include <TT/TranslationTable.h>

class VetoIntDigiHit:public jana::JObject{
	public:
		JOBJECT_PUBLIC(VetoIntDigiHit);
		
		// Add data members here. For example:
		// int id;
		// double E;
		
		// This method is used primarily for pretty printing
		// the second argument to AddString is printf style format
		void toStrings(vector<pair<string,string> > &items)const{
			// AddString(items, "id", "%4d", id);
			// AddString(items, "E", "%f", E);
		}
		
	public: //yes, these are public to keep this simple.
		TranslationTable::ChannelInfo m_channel; //both crate-slot channel and detector-specific ID
		double Q,T;

};

#endif // _VetoIntDigiHit_

