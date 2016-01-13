/*
 * JOutput.h
 *
 *  Created on: Jan 13, 2016
 *      Author: celentan
 */

#ifndef JOUTPUT_H_
#define JOUTPUT_H_


#include <JANA/JObject.h>
#include <JANA/JFactory.h>


class JOutput : public jana::JObject{
public:
	JOBJECT_PUBLIC(JOutput);

	//This virtual method has to be implemented in derived classes, specifying what to do when "fini" is called in the BDXEventProcess
	virtual int CloseOutput()=0;
};

#endif /* JOUTPUT_H_ */
