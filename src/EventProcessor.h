#ifndef _EventProcessor_
#define _EventProcessor_

// JANA headers
#include <JANA/JEventProcessor.h>
using namespace jana;

// bdx headers
#include "options.h"

// bank headers
#include "gbank.h"

// EVIO headers
#include "evioUtil.hxx"
#include "evioFileChannel.hxx"
using namespace evio;

class EventProcessor:public JEventProcessor
{
	public:
		 EventProcessor(goptions bdxOpt);
		~EventProcessor();
		const char* className(void){return "EventProcessor";}

	private:
		jerror_t init();                                 // Called once at program start.
		jerror_t brun(JEventLoop*, int runnumber);       // Called everytime a new run number is detected.
		jerror_t evnt(JEventLoop*, int eventnumber);     // Called every event.
		jerror_t erun();                                 // Called everytime run number changes, provided brun has been called.
		jerror_t fini();                                 // Called after last event of last event source has been processed.

		string           ofile;    // Output file name
		evioFileChannel *ochan;    // Output evio chan

	
	//		TFile *outf;

	
};

#endif

