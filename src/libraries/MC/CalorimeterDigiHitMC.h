// $Id$
//
//    File: genParticle.h
// Created: Tue Mar  1 00:59:14 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#ifndef _JCalorimeterDigiHitMC_
#define _JCalorimeterDigiHitMC_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>


/*This class is basically a place-holder for the data coming from EVIO file,
 * for the calorimeter digitized banks
 */
class CalorimeterDigiHitMC:public jana::JObject{
	public:
		JOBJECT_PUBLIC(CalorimeterDigiHitMC);
		
		// Add data members here. For example:
		// int id;
		// double E;
		
		// This method is used primarily for pretty printing
		// the second argument to AddString is printf style format
		void toStrings(vector<pair<string,string> > &items)const{
			 AddString(items, "id", "%4d", id);
		}
		int x,y,sector;
		int adcl,adrc,tdcl,tdcr;
		int adcb,adcf,tdcb,tdcf;
};

#endif // _genParticle_

