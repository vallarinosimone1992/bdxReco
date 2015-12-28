#ifndef _CTOFhit_
#define _CTOFhit_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
using namespace jana;

class CTOFhit:public JObject
{
	public:
		JOBJECT_PUBLIC(CTOFhit);
		
		int  paddle;
		double ADCL;
		double ADCR;
};

#endif

