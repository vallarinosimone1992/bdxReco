// JANA headers
#include "Factory.h"


// CTOF
#include "CTOFhit.h"
#include "ctofHitR_factory.h"




Factory::Factory(goptions Opt)
{
	bdxOpt = Opt;
}

Factory::~Factory() {}


// GenerateFactories
jerror_t Factory::GenerateFactories(JEventLoop *loop)
{
	// CTOF
	loop->AddFactory(new JFactory<CTOFhit>());
	loop->AddFactory(new ctofHitR_factory());
	
	return NOERROR;
}

