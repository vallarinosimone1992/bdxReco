#ifndef _Factory_
#define _Factory_

// JANA headers
#include "JANA/JObject.h"
#include "JANA/JFactoryGenerator.h"
#include "JANA/jerror.h"
using namespace jana;

// bdx headers
#include "options.h"

// class definition
class BDXFactoryGenerator: public JFactoryGenerator
{
	public:
		JOBJECT_PUBLIC(BDXFactoryGenerator);
		BDXFactoryGenerator(goptions Opt);

		virtual ~BDXFactoryGenerator();

		
		jerror_t GenerateFactories(JEventLoop*);

	private:
		goptions bdxOpt;

};

#endif
