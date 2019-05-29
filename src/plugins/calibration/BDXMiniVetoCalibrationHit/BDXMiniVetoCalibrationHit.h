// $Id$
//
//    File: BDXMiniVetoCalibrationHit.h
// Created: Fri May 17 11:56:57 CEST 2019
// Creator: celentan (on Linux apcx4 3.10.0-957.el7.x86_64 x86_64)
//

#ifndef _BDXMiniVetoCalibrationHit_
#define _BDXMiniVetoCalibrationHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <TT/TranslationTable.h>
class BDXMiniVetoCalibrationHit: public jana::JObject {
public:
	JOBJECT_PUBLIC(BDXMiniVetoCalibrationHit)
	;

	// Add data members here. For example:
	// int id;
	// double E;

	// This method is used primarily for pretty printing
	// the second argument to AddString is printf style format
	void toStrings(vector<pair<string, string> > &items) const {
		 AddString(items, "sector", "%4d",m_channel.sector);
		 AddString(items, "layer", "%4d",m_channel.layer);
		 AddString(items, "component", "%4d",m_channel.component);
		 AddString(items, "Qraw", "%f",Qraw);
		 AddString(items, "Qphe", "%f",Qphe);
		 AddString(items, "T","%4f",T);
		 AddString(items, "A","%4f",A);

		// AddString(items, "id", "%4d", id);
		// AddString(items, "E", "%f", E);
	}

	double Qraw,Qphe;
	double T,A;

	int isMatched; //relevant for lateral veto only. Is this matched to an hit in the corresponding veto?

	TranslationTable::INT_VETO_Index_t m_channel; //Detector-specific ID. Since this is a detector-based object, the readout field will be 0


};

#endif // _BDXMiniVetoCalibrationHit_

