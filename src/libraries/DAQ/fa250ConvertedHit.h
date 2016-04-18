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
#include "TObject.h"

/*A common class for fad250 converted hits. Can be usefull*/
class fa250ConvertedHit :public jana::JObject {
public:
	JOBJECT_PUBLIC(fa250ConvertedHit);
	fa250ConvertedHit(){};
	virtual ~fa250ConvertedHit(){};
#if (!defined(__APPLE__))
	ClassDef(fa250ConvertedHit,1);
#endif
};



#endif /* FAINTEGRATEDHIT_H_ */
