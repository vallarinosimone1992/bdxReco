// BDX headers
#include "BDXFactoryGenerator.h"


// DAQ
#include "fa250Mode1Hit_factory.h"
#include "fa250Mode7Hit_factory.h"


BDXFactoryGenerator::BDXFactoryGenerator(goptions Opt)
{
	bdxOpt = Opt;
}

BDXFactoryGenerator::~BDXFactoryGenerator() {}


// GenerateFactories
jerror_t BDXFactoryGenerator::GenerateFactories(JEventLoop *loop)
{



	loop->AddFactory(new fa250Mode1Hit_factory());
	loop->AddFactory(new fa250Mode7Hit_factory());
	/*loop->AddFactory(new JFactory<CTOFhit>());
	loop->AddFactory(new ctofHitR_factory());
	loop->AddFactory(new marcoCluster_factory());
	*/
	return NOERROR;
}

