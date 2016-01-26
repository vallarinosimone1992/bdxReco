// $Id$
//
//    File: fa250Mode1Hit.h
// Created: Mon Dec 28 11:04:04 CET 2015
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _fa250Mode1Hit_
#define _fa250Mode1Hit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>


#include "fa250Hit.h"

class fa250Mode1Hit:public fa250Hit{
	public:
		JOBJECT_PUBLIC(fa250Mode1Hit);
		
		// Add data members here. For example:
		// int id;
		// double E;
		

		vector <short> samples;

		// This method is used primarily for pretty printing
		// the second argument to AddString is printf style format
		void toStrings(vector<pair<string,string> > &items)const{
			 AddString(items, "crate", "%4d", crate);
			 AddString(items, "slot", "%4d", slot);
			 AddString(items, "channel", "%4d", channel);

			// AddString(items, "E", "%f", E);
		}
		
};

#endif // _fa250Mode1Hit_
