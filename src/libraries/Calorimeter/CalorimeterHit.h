// $Id$
//
//    File: CalorimeterHit.h
// Created: Wed Jan 27 16:40:00 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _CalorimeterHit_
#define _CalorimeterHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <TT/TranslationTable.h>
#include <system/BDXObject.h>
#include <Calorimeter/CalorimeterSiPMHit.h>

#include <TObject.h>

class TH1D;
class TCanvas;


class CalorimeterHit:public TObject,public BDXObject{
private:
	mutable vector<TH1D*> hWaves; //!

public:
	struct CalorimeterComponentHit{
	public:
		int readout;
		double E;
		double Q,Qs;
		double T;
		bool good_ped_RMS;
		CalorimeterSiPMHit::hit_type type;
	};

	JOBJECT_PUBLIC(CalorimeterHit);

	// Add data members here. For example:
	// int id;
	// double E;

	// This method is used primarily for pretty printing
	// the second argument to AddString is printf style format
	void toStrings(vector<pair<string,string> > &items)const{
		// AddString(items, "id", "%4d", id);
		// AddString(items, "E", "%f", E);
	}
	vector<CalorimeterComponentHit> m_data;
	double E,Q,T;
	int N; //how many counters associated with this object were above thr (i.e. how many entries in m_data?

	virtual TCanvas* Draw(int id)const; //!

	TranslationTable::CALO_Index_t m_channel; //Detector-specific ID. Since this is a detector-based object, the readout field will be ==0

	uint64_t timestamp;

	ClassDef(CalorimeterHit,1);
};

#endif // _CalorimeterHit_

