#ifndef _Factory_
#define _Factory_

// JANA headers
#include "JANA/JFactoryGenerator.h"
#include "JANA/jerror.h"
using namespace jana;

// bdx headers
#include "options.h"

// class definition
class Factory: public JFactoryGenerator
{
	public:
		Factory(goptions Opt);
		goptions bdxOpt;

		virtual ~Factory();
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "Factory";}
		
		jerror_t GenerateFactories(JEventLoop*);
};

#endif
