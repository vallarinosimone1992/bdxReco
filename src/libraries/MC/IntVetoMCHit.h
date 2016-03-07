// $Id$
//
//    File: genParticle.h
// Created: Tue Mar  1 00:59:14 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#ifndef _JIntVetoMCHit_
#define _JIntVetoMCHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>


/*This class is basically a place-holder for the data coming from EVIO file,
 * for the calorimeter raw and digitized banks
 */
class IntVetoMCHit:public jana::JObject{
	public:
		JOBJECT_PUBLIC(IntVetoMCHit);

		// Add data members here. For example:
		// int id;
		// double E;

		// This method is used primarily for pretty printing
		// the second argument to AddString is printf style format
		void toStrings(vector<pair<string,string> > &items)const{
			 AddString(items, "id", "%4d", id);
		}
		int sector,channel;

		//dgtz banks
		int adc1,adc2,adc3,adc4;
		int tdc1,tdc2,tdc3,tdc4;

		//raw banks
		double totEdep;
};

#endif

