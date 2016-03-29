// $Id$
//
//    File: CalorimeterCluster.h
// Created: Tue Mar 22 11:19:57 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _CalorimeterCluster_
#define _CalorimeterCluster_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>

class CalorimeterCluster:public jana::JObject{
	public:
		JOBJECT_PUBLIC(CalorimeterCluster);
		
		// Add data members here. For example:
		// int id;
		// double E;
		
		// This method is used primarily for pretty printing
		// the second argument to AddString is printf style format
		void toStrings(vector<pair<string,string> > &items)const{
			// AddString(items, "id", "%4d", id);
			// AddString(items, "E", "%f", E);
		}
		
		double Eseed;
		double E,T;
		double x,y;
		int N;

};

#endif // _CalorimeterCluster_

