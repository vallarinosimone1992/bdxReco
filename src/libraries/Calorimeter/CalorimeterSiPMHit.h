// $Id$
//
//    File: CalorimeterSiPMHit.h
// Created: Wed Jan 27 16:39:49 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _CalorimeterSiPMHit_
#define _CalorimeterSiPMHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <DAQ/fa250ConvertedHit.h>
#include <TT/TranslationTable.h>

class TF1;

typedef enum{
	noise=0,
	one_phe=1,
	single_phes=1,
	real_signal=2,
	num_hit_type
}hit_type;



class CalorimeterSiPMHit:public fa250ConvertedHit{
public:
	JOBJECT_PUBLIC(CalorimeterSiPMHit);

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
	TranslationTable::ChannelInfo m_channel; //both crate-slot channel and detector-specific ID. Since this is a sensor-based object, the readout field will be !=0
	/*These 3 variables are: hit charge (u.a.), hit time (in ns), hit amplitude (in mV)*/
	double Q,T,A;
	/*miniped is always calculated event-by-event, on the first NPEDS samples*/
	/*ped can be by event OR by run, from dB*/
	double ped,miniped;

	union{
		TF1 *fSinglePhe;
	}fit_function;

	hit_type m_type;

};

#endif // _CalorimeterSiPMHit_

