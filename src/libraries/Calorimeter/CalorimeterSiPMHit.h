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


#include <TObject.h>

class CalorimeterSiPMHit:public fa250ConvertedHit , public TObject{
public:
	JOBJECT_PUBLIC(CalorimeterSiPMHit);

	typedef enum{
		noise=0,
		one_phe=1,
		good_one_phe=2,
		many_phe=3,
		real_signal=4,
		num_hit_type
	}hit_type;


	// Add data members here. For example:
	// int id;
	// double E;

	// This method is used primarily for pretty printing
	// the second argument to AddString is printf style format
	void toStrings(vector<pair<string,string> > &items)const{
		 AddString(items, "Qraw", "%f",Qraw);
		 AddString(items,"Qphe","%f",Qphe);
		 AddString(items,"A","%f",A);
		 AddString(items,"ped","%f",ped);
		 AddString(items,"rms_flag","%i",1*good_ped_RMS);
	}

	//A.C. do not touch these
	//both crate-slot channel and detector-specific ID. Since this is a sensor-based object, the readout field will be !=0
	TranslationTable::ChannelInfo m_channel; //!
	/*These 3 variables are: hit charge (u.a.), hit time (in ns), hit amplitude (in mV)*/
	double Qraw,Qphe;
	double QrawS,QpheS;
	double T,A;
	double average;
	double ped,pedSigma;
	bool good_ped_RMS;
	hit_type m_type;

	uint64_t timestamp;

	int nSingles,nSignals;
#if (!defined(__APPLE__))
	ClassDef(CalorimeterSiPMHit,1);
#endif

};
#endif // _CalorimeterSiPMHit_

