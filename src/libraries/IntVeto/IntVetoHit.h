// $Id$
//
//    File: IntVetoHit.h
// Created: Wed Jan 13 21:08:09 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _IntVetoHit_
#define _IntVetoHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>

class IntVetoHit:public jana::JObject{
	public:
		JOBJECT_PUBLIC(IntVetoHit);
		
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

#endif // _IntVetoHit_

