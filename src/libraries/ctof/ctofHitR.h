// $Id$
//
//    File: ctofHit.h
// Created: Thu Dec 17 14:09:30 EST 2015
// Creator: ungaro (on Darwin MauriMP.local 14.5.0 i386)
//

#ifndef _ctofHitR_
#define _ctofHitR_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
using namespace jana;

class ctofHitR:public JObject
{
	public:
		JOBJECT_PUBLIC(ctofHitR);
	
	
	ctofHitR(double e)
	{
		E = e;
	}
	
	~ctofHitR(){;}
	
		// Add data members here. For example:
		 double E;
		int paddle;
	
		
};

#endif 

