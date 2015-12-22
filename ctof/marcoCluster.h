// $Id$
//
//    File: marcoCluster.h
// Created: Thu Dec 17 16:37:25 EST 2015
// Creator: ungaro (on Darwin MauriMP.local 14.5.0 i386)
//

#ifndef _marcoCluster_
#define _marcoCluster_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>

class marcoCluster:public jana::JObject{
	public:
		JOBJECT_PUBLIC(marcoCluster);
	
	

	double clusterEnergy;
	int clusterID;
	
	
};

#endif // _marcoCluster_

