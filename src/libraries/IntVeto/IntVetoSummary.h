// $Id$
//
//    File: IntVetoSummary.h
// Created: Tue Mar 29 15:07:39 CEST 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _IntVetoSummary_
#define _IntVetoSummary_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <TT/TranslationTable.h>
class IntVetoSummary:public jana::JObject{
	public:
		JOBJECT_PUBLIC(IntVetoSummary);
		
		// Add data members here. For example:
		// int id;
		// double E;
		
		// This method is used primarily for pretty printing
		// the second argument to AddString is printf style format
		void toStrings(vector<pair<string,string> > &items)const{
			// AddString(items, "id", "%4d", id);
			// AddString(items, "E", "%f", E);
		}
		int sector;
		
		int nHits;
		vector<TranslationTable::INT_VETO_Index_t> hits;


};

#endif // _IntVetoSummary_

