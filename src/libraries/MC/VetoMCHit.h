/*
 * VetoMCHit.h
 *
 *  Created on: May 5, 2016
 *      Author: celentan
 */

#ifndef VETOMCHIT_H_
#define VETOMCHIT_H_


#include <JANA/JObject.h>
#include <JANA/JFactory.h>

class VetoMCHit : public jana::JObject{


public:
	enum SYSTEMS{
			CATANIA_INTVETO=1,
			CATANIA_EXTVETO=2,
			CATANIA_PADDLES=3,
			FULL_INTVETO=4,
			FULL_EXTVETO=5
	};

	VetoMCHit(){};
	virtual ~VetoMCHit(){};

	int sector,channel,system;
};

#endif /* VETOMCHIT_H_ */
