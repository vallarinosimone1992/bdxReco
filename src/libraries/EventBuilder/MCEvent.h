

#ifndef _MCEvent_
#define _MCEvent_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include "TObject.h"

#include <TT/TranslationTable.h>

class MCEvent:public jana::JObject,public TObject{
	public:
		JOBJECT_PUBLIC(MCEvent);
		
		// Add data members here. For example:
		// int id;
		// double E;
		
		// This method is used primarily for pretty printing
		// the second argument to AddString is printf style format
		void toStrings(vector<pair<string,string> > &items)const{
			// AddString(items, "id", "%4d", id);
			// AddString(items, "E", "%f", E);
		}
	//	double Ec1,Ec2;
		double E,T;
		int nCalorimeterHits;  //mr
		bool flag_RMS;

		int nExtVetoHits;
		int nExtVetoHitsCoincidence;
		vector <TranslationTable::EXT_VETO_Index_t> vExtVetoHits;
		vector <TranslationTable::EXT_VETO_Index_t> vExtVetoHitsCoincidence;

		int nIntVetoHits;
		int nIntVetoHitsCoincidence;
		vector <TranslationTable::INT_VETO_Index_t> vIntVetoHits;
		vector <TranslationTable::INT_VETO_Index_t> vIntVetoHitsCoincidence;

		double Ep1,Ep2;

#if (!defined(__APPLE__))
		ClassDef(MCEvent,1);
#endif
};

#endif // _MCEvent_

