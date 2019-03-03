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
#include <MC/VetoMCHit.h>

/*This class is basically a place-holder for the data coming from EVIO file,
 * for the raw and digitized banks
 */
class IntVetoMCHit: public VetoMCHit {
public:
	JOBJECT_PUBLIC(IntVetoMCHit)
	;

	//dgtz banks
	int adc1, adc2, adc3, adc4,adc5,adc6,adc7,adc8;
	int tdc1, tdc2, tdc3, tdc4,tdc5,tdc6,tdc7,tdc8;

	//raw banks
	double totEdep;

	// This method is used primarily for pretty printing
	// the second argument to AddString is printf style format
	void toStrings(vector<pair<string, string> > &items) const {
		AddString(items, "id", "%4d", id);
		AddString(items, "totEdep", "%f", totEdep);
		AddString(items, "adc1", "%d", adc1);
		AddString(items, "adc2", "%d", adc2);
		AddString(items, "tdc1", "%d", tdc1);
		AddString(items, "tdc2", "%d", tdc2);
	}
};

#endif

