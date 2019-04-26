// $Id$
//
//    File: BDXMiniCalorimeterEnergyCalibrationHit.h
// Created: Wed Apr  3 17:08:28 CEST 2019
// Creator: celentan (on Linux apcx4 2.6.32-696.13.2.el6.x86_64 x86_64)
//

#ifndef _BDXMiniCalorimeterEnergyCalibrationHit_
#define _BDXMiniCalorimeterEnergyCalibrationHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>

#include <TT/TranslationTable.h>

class BDXMiniCalorimeterEnergyCalibrationHit: public jana::JObject {
public:
	JOBJECT_PUBLIC(BDXMiniCalorimeterEnergyCalibrationHit)
	;

	// Add data members here. For example:
	// int id;
	// double E;

	// This method is used primarily for pretty printing
	// the second argument to AddString is printf style format
	void toStrings(vector<pair<string, string> > &items) const {
		AddString(items, "sector", "%i", m_channel.sector);
		AddString(items, "x", "%i", m_channel.x);
		AddString(items, "y", "%i", m_channel.y);
		AddString(items, "readout", "%i", m_channel.readout);
		AddString(items, "Q", "%f", Q);
		AddString(items, "E", "%f", E);
		AddString(items, "A", "%f", A);
	}

	double E,Q, T, A; //Energy(MeV) Charge (phe?), time (ns), Amplitude (mV)

	TranslationTable::CALO_Index_t m_channel; //Detector-specific ID. Since this is a detector-based object, the readout field will be 0

};

#endif // _BDXMiniCalorimeterEnergyCalibrationHit_

