

#ifndef _MCEvent_EM_
#define _MCEvent_EM_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include "TObject.h"

#include <TT/TranslationTable.h>

class MCEvent_EM:public jana::JObject,public TObject{
	public:
		JOBJECT_PUBLIC(MCEvent_EM);
		
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
		double E,T, E1, E2, phe1, phe2;
		int nCalorimeterHits;  //mr

		int nCalorimeterHits_S0, nCalorimeterHits_S1, nCalorimeterHits_S2, nCalorimeterHits_S3, nCalorimeterHits_S4, nCalorimeterHits_S5, nCalorimeterHits_S6, nCalorimeterHits_S7;


		vector <TranslationTable::CALO_Index_t> vCalorimeterHits;
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

		// For clustering
		double Eseed,Nhit_cluster,E_cluster,T_cluster;
		double xseed,yseed, sectorseed;

#if (!defined(__APPLE__))
		ClassDef(MCEvent_EM,1);
#endif
};

#endif // _MCEvent_

