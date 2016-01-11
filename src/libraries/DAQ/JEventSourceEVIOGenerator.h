#ifndef _JEventSourceEvioGenerator_
#define _JEventSourceEvioGenerator_

// JANA headers
#include "JANA/JEventSourceGenerator.h"
using namespace jana;

// bdx headers
#include "options.h"

class JEventSourceEvioGenerator:public JEventSourceGenerator
{
	public:
		 JEventSourceEvioGenerator(goptions Opt){bdxOpt = Opt;}
		~JEventSourceEvioGenerator(){}


		const char* className(void)               {return static_className();}
		static const char* static_className(void) {return "JEventSourceEvioGenerator";}
		
		const char* Description(void);
		
		double CheckOpenable(string source);
		
		JEventSource* MakeJEventSource(string source);
		
	private:
		goptions bdxOpt;
};

#endif // _JEventSourceEvioGenerator_

