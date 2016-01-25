/*
 * fa250Hit.h
 *
 *  Created on: Jan 25, 2016
 *      Author: celentan
 */

#ifndef FA250HIT_H_
#define FA250HIT_H_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>

class fa250Hit : public jana::JObject{
public:

	virtual ~fa250Hit()=0; //A.C. purely virtual!

	int crate,slot,channel;
	int time,trigger;

};

#endif /* FA250HIT_H_ */
