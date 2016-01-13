// bdx reconstruction software

// JANA headers
#include <JANA/JApplication.h>
using namespace jana;

// bdx headers
#include <system/options.h>
#include <DAQ/JEventSourceEVIOGenerator.h>
#include <system/BDXEventProcessor.h>

//factory generators
#include <DAQ/JFactoryGenerator_DAQ.h>
#include <TT/JFactoryGenerator_TT.h>
#include <VETO_INT/JFactoryGenerator_VETO_INT.h>


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


	app.AddFactoryGenerator(new JFactoryGenerator_DAQ());
	app.AddFactoryGenerator(new JFactoryGenerator_TT());
	app.AddFactoryGenerator(new JFactoryGenerator_VETO_INT());

	app.AddProcessor(new BDXEventProcessor(bdxOpt));

	app.Run();

	return 1;
}


