// $Id$
//
//    File: fa250Mode7Hit.h
// Created: Mon Dec 28 15:05:37 CET 2015
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _fa250Mode7Hit_
#define _fa250Mode7Hit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>



class fa250Mode7Hit:public jana::JObject{

public:
	typedef struct{
		int tdc;
		int adc;
		int ped;
		int max;
	}pulse_t;

public:
	JOBJECT_PUBLIC(fa250Mode7Hit);



	// Add data members here. For example:
	// int id;
	// double E;
	int crate,slot,channel;
	int time,trigger;
	vector <pulse_t> pulses;

	// This method is used primarily for pretty printing
	// the second argument to AddString is printf style format
	void toStrings(vector<pair<string,string> > &items)const{
		// AddString(items, "id", "%4d", id);
		// AddString(items, "E", "%f", E);
	}



};

#endif // _fa250Mode7Hit_

