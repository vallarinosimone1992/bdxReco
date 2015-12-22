// C++ headers
#include <cstdio>
#include <iostream>
using namespace std;

// bdx headers
#include "EventProcessor.h"

// CTOF
#include "ctofHitR.h"
#include "marcoCluster.h"



// Constructor
EventProcessor::EventProcessor(goptions bdxOpt)
{
	// Opens output file if specified
	ofile  = bdxOpt.optMap["OUTPUT"].args;
	string hd_msg = bdxOpt.optMap["LOG_MSG"].args + " Event: >> " ;
	ochan = NULL;
	
	if(ofile != "none")
	{
		cout << hd_msg << " Opening output file " << ofile << "." << endl;
		ochan = new evioFileChannel(ofile, "w", 3000000);
		ochan->open();
	}
}

// Destructor
EventProcessor::~EventProcessor()
{

	if(ofile != "none")
	{
		cout << " Closing output file " << ofile << "." << endl;
		ochan->close();
		delete ochan;
	}
}

// init
jerror_t EventProcessor::init(void)
{
	
	outf = new TFile("raffa.root", "RECREATE");

	
	// Create histograms here
	return NOERROR;
}

// brun
jerror_t EventProcessor::brun(JEventLoop *eventLoop, int runnumber)
{
	return NOERROR;
}

// evnt
jerror_t EventProcessor::evnt(JEventLoop *loop, int eventnumber)
{
//
//	vector<const marcoCluster*> marcoC;
//	loop->Get(marcoC);

	vector<const ctofHitR*> marcoC;
	loop->Get(marcoC);
	
	
	
	return NOERROR;
}

// erun
jerror_t EventProcessor::erun(void)
{
	// Any final calculations on histograms (like dividing them)
	// should be done here. This may get called more than once.
	return NOERROR;
}

// fini
jerror_t EventProcessor::fini(void)
{
	// If another DEventProcessor is in the list ahead of this one, then
	// it will have finished before this is called. e.g. closed the
	// ROOT file!
	return NOERROR;
}

