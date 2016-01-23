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

class FadcConverter:public jana::JObject{
	public:
		JOBJECT_PUBLIC(FadcConverter);
		
		// Add data members here. For example:
		// int id;
		// double E;
		
		// This method is used primarily for pretty printing
		// the second argument to AddString is printf style format
		void toStrings(vector<pair<string,string> > &items)const{
			// AddString(items, "id", "%4d", id);
			// AddString(items, "E", "%f", E);
		}
		
};

#endif // _FadcConverter_

