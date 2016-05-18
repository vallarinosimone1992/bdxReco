// $Id$
//
//    File: CataniaEvent.h
// Created: Mon Apr 25 23:25:44 CEST 2016
// Creator: celentan (on Linux dhcp-visitor-enr-116-196.slac.stanford.edu 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#ifndef _CataniaEvent_
#define _CataniaEvent_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include "TObject.h"

#include <TT/TranslationTable.h>

class CataniaEvent:public jana::JObject,public TObject{
	public:
		JOBJECT_PUBLIC(CataniaEvent);
		
		// Add data members here. For example:
		// int id;
		// double E;
		
		// This method is used primarily for pretty printing
		// the second argument to AddString is printf style format
		void toStrings(vector<pair<string,string> > &items)const{
			// AddString(items, "id", "%4d", id);
			// AddString(items, "E", "%f", E);
		}
		double Ec1,Ec2;
		double E,T;
		bool flag_RMS;

		int nExtVetoHits;
		int nExtVetoHitsCoincidence;
		vector <TranslationTable::EXT_VETO_Index_t> vExtVetoHits;
		vector <bool> vExtVetoHitsIsInCoincidence;
		vector <double>	vExtVetoHitsT;
		vector <double>	vExtVetoHitsE;


		int nIntVetoHits;
		int nIntVetoHitsCoincidence;
		vector <TranslationTable::INT_VETO_Index_t> vIntVetoHits;
		vector <bool> vIntVetoHitsIsInCoincidence;
		vector <double>	vIntVetoHitsT;
		vector <double>	vIntVetoHitsQ;


		double Ep1,Ep2;

		int time; //in s
		int tWord;

		int eventN,runN;

		uint64_t timestamp;


#if (!defined(__APPLE__))
		ClassDef(CataniaEvent,1);
#endif
};

#endif // _CataniaEvent_

