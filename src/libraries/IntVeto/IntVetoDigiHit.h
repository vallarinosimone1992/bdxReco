// $Id$
//
//    File: IntVetoDigiHit.h
// Created: Wed Jan 20 16:42:38 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _IntVetoDigiHit_
#define _IntVetoDigiHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <TT/TranslationTable.h>
/*
 * A.C.
 * This object represents a not-yet calibrated it in the inner veto.
 * The goal of this object is dual:
 * - Do the matching of the different IntVetoSiPMhit hits corresponding to the SAME active volume (sector / layer / component)
 * - Be as similar as possible to what is obtained from GEMC, while we wait GEMC producing composite, fadc-like, banks.
 */

class IntVetoDigiHit:public jana::JObject{
public:
	struct IntVetoSiPMDigiHit{
			int readout;
			double Q;
			double T;
		};



	JOBJECT_PUBLIC(IntVetoDigiHit);

	// Add data members here. For example:
	// int id;
	// double E;

	// This method is used primarily for pretty printing
	// the second argument to AddString is printf style format
	void toStrings(vector<pair<string,string> > &items)const{
		// AddString(items, "id", "%4d", id);
		// AddString(items, "E", "%f", E);
	}
	//A.C. do not touch these
	TranslationTable::INT_VETO_Index_t m_channel; //both crate-slot channel and detector-specific ID. Since this is a detector-based object, the readout field will be ==0
	vector <IntVetoSiPMDigiHit> m_data; //keep in this way to be MC-compatible!



};

#endif // _IntVetoDigiHit_

