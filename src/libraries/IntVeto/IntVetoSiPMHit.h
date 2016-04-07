// $Id$
//
//    File: VetoIntDigiHit.h
// Created: Tue Jan 12 11:52:41 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _VetoIntSiPMHit_
#define _VetoIntSiPMHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <DAQ/fa250ConvertedHit.h>
#include <TT/TranslationTable.h>


class IntVetoSiPMHit:public fa250ConvertedHit{
public:
	JOBJECT_PUBLIC(IntVetoSiPMHit);

	// Add data members here. For example:
	// int id;
	// double E;

	// This method is used primarily for pretty printing
	// the second argument to AddString is printf style format
	void toStrings(vector<pair<string,string> > &items)const{
		AddString(items, "sector", "%4d", m_channel.int_veto->sector);
		AddString(items, "layer", "%4d", m_channel.int_veto->layer);
		AddString(items, "component", "%4d", m_channel.int_veto->component);
		AddString(items, "readout", "%4d", m_channel.int_veto->readout);
	}
	typedef enum{
		noise=0,
		real_signal=1, //not much more to distinguish!
		num_hit_type
	}hit_type;



	//A.C. do not touch these
	TranslationTable::ChannelInfo m_channel; //both crate-slot channel and detector-specific ID. Since this is a sensor-based object, the readout field will be !=0

	double Qraw,Qphe,T,A;
	double average;

	int nSingles;
	hit_type m_type;
};

#endif // _VetoIntDigiHit_

