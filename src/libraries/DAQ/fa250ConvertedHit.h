/*
 * faIntegratedHit.h
 *
 *  Created on: Jan 21, 2016
 *      Author: celentan
 */

#ifndef FAINTEGRATEDHIT_H_
#define FAINTEGRATEDHIT_H_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>

class fa250ConvertedHit :public jana::JObject {
public:
	JOBJECT_PUBLIC(fa250ConvertedHit);

	virtual ~fa250ConvertedHit()=0; //A.C. purely virtual since I do not want to instantiate from this!

};



#endif /* FAINTEGRATEDHIT_H_ */
