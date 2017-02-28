// $Id$
//
//    File: genParticle.h
// Created: Tue Mar  1 00:59:14 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#ifndef _JCalorimeterMCHit_
#define _JCalorimeterMCHit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>

/*This class is basically a place-holder for the data coming from EVIO file,
 * for the calorimeter raw and digitized banks
 */
class CalorimeterMCHit: public jana::JObject {
public:
	JOBJECT_PUBLIC(CalorimeterMCHit)
	;

	// Add data members here. For example:
	// int id;
	// double E;

	// This method is used primarily for pretty printing
	// the second argument to AddString is printf style format
	void toStrings(vector<pair<string, string> > &items) const {
		AddString(items, "sector(MC)", "%i", sector);
		AddString(items, "x(MC)", "%i", x);
		AddString(items, "y(MC)", "%i", y);
		AddString(items, "E", "%f", totEdep);
	}
	int x, y, sector;

	//dgtz banks
	int adcl, adcr, tdcl, tdcr;
	int adcb, adcf, tdcb, tdcf;

	//raw banks
	double totEdep;
	int pid;
};

#endif // _genParticle_

