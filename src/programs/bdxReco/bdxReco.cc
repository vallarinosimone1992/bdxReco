// bdx reconstruction software

// JANA headers
#include <JANA/JApplication.h>
using namespace jana;

// bdx headers

#include <system/JEventSourceEVIOGenerator.h>
#include <system/BDXEventProcessor.h>

//factory generators
#include <system/JFactoryGenerator_system.h>
#include <DAQ/JFactoryGenerator_DAQ.h>
#include <MC/JFactoryGenerator_MC.h>
#include <TT/JFactoryGenerator_TT.h>
#include <IntVeto/JFactoryGenerator_IntVeto.h>
#include <ExtVeto/JFactoryGenerator_ExtVeto.h>
#include <Calorimeter/JFactoryGenerator_Calorimeter.h>
#include <Paddles/JFactoryGenerator_Paddles.h>
#include <EventBuilder/JFactoryGenerator_EventBuilder.h>
#include "TH1D.h"
#include "TMinuitMinimizer.h"
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
	//goptions bdxOpt;
	//bdxOpt.setGoptions();
	//bdxOpt.setOptMap(narg, argv);
	TH1::AddDirectory(kFALSE);

	JApplication app(narg, argv);

	if(narg==1){
		app.Usage();
		return 0;
	}



	app.AddEventSourceGenerator(new JEventSourceEvioGenerator());
	jout<<"JEventSourceEvioGenerator DONE"<<endl;

	app.AddFactoryGenerator(new JFactoryGenerator_system());
	jout<<"JFactoryGenerator_system DONE"<<endl;
	app.AddFactoryGenerator(new JFactoryGenerator_DAQ());
	jout<<"JFactoryGenerator_DAQ DONE"<<endl;
	app.AddFactoryGenerator(new JFactoryGenerator_MC());
	jout<<"JFactoryGenerator_MC DONE"<<endl;
	app.AddFactoryGenerator(new JFactoryGenerator_TT());
	jout<<"JFactoryGenerator_TT DONE"<<endl;
	app.AddFactoryGenerator(new JFactoryGenerator_ExtVeto());
	jout<<"JFactoryGenerator_ExtVeto DONE"<<endl;
	app.AddFactoryGenerator(new JFactoryGenerator_IntVeto());
	jout<<"JFactoryGenerator_IntVeto DONE"<<endl;
	app.AddFactoryGenerator(new JFactoryGenerator_Calorimeter());
	jout<<"JFactoryGenerator_Calorimeter DONE"<<endl;
	app.AddFactoryGenerator(new JFactoryGenerator_Paddles());
	jout<<"JFactoryGenerator_Paddles DONE"<<endl;
	app.AddFactoryGenerator(new JFactoryGenerator_EventBuilder());
	jout<<"JFactoryGenerator_EventBuilder DONE"<<endl;
	app.AddProcessor(new BDXEventProcessor());
	jout<<"BDXEventProcessor DONE"<<endl;
	app.Run();

	return 1;
}


