// bdx reconstruction software

// JANA headers
#include <JANA/JApplication.h>
using namespace jana;

// bdx headers
#include "options.h"
#include "JEventSourceEVIOGenerator.h"
#include "BDXEventProcessor.h"
#include "BDXFactoryGenerator.h"


// C++ headers
#include <iostream>
using namespace std;


//  command line help:
// -PEVENTS_TO_KEEP=220
// -PPLUGINS=janadot ; dot -Tpng jana.dot -o jana.png ; open jana.png
// --nthreads=3
// -PJANA:JOUT_TAG="aaa"
//


int main(int narg, char *argv[])
{
	goptions bdxOpt;
	bdxOpt.setGoptions();
	bdxOpt.setOptMap(narg, argv);
 
	
	JApplication app(narg, argv);

	if(narg==1) app.Usage();

	app.AddEventSourceGenerator(new JEventSourceEvioGenerator(bdxOpt));
	app.AddFactoryGenerator(new BDXFactoryGenerator(bdxOpt));
	app.AddProcessor(new BDXEventProcessor(bdxOpt));

	app.Run();

	return 1;
}


