// C++ headers
#include <cstdio>
#include <iostream>
using namespace std;

// bdx headers
#include "BDXEventProcessor.h"
#include "string_utilities.h"
//DAQ
#include "fa250Mode1Hit_factory.h"
// CTOF
//#include "ctofHitR.h"
//#include "marcoCluster.h"



// Constructor
BDXEventProcessor::BDXEventProcessor(goptions bdxOpt)
{
	/* Opens output file if specified
	the string should be of the form
	type filename
	with type: TXT EVIO ROOT
	and filename anything
	 */
	string type,name;
	optf  = bdxOpt.optMap["OUTPUT"].args;


	outType.assign(optf, 0, optf.find(",")) ;
	outFile.assign(optf,    optf.find(",") + 1, optf.size()) ;




	if(optf!= "none")
	{
		cout<<"Out file type is: "<<outType<<endl;
		if(outType == "txt")  return;
		else if(outType == "evio")
		{
			outFileEvio = new evioFileChannel(TrimSpaces(outFile).c_str(), "w", 3000000);
			outFileEvio->open();
		}
		else if(outType == "root"){
			outFileRoot = new TFile(TrimSpaces(outFile).c_str(), "RECREATE");

		}


	}

	//string hd_msg = bdxOpt.optMap["LOG_MSG"].args + " Event: >> " ;


}

// Destructor
BDXEventProcessor::~BDXEventProcessor()
{

	if(optf != "none")
	{
		if(outType == "txt")  return;
			else if(outType == "evio")
			{
				cout << " Closing evio file "<<endl;
				outFileEvio -> close();
			}
			else if(outType == "root"){
				cout << " Closing output file root " << endl;
				outFileRoot -> Close();
			}
	}
}

// init
jerror_t BDXEventProcessor::init(void)
{

	// outf = new TFile("raffa.root", "RECREATE");

	jout<<"BDXEventProcessor::init is called"<<endl;

	app->RootWriteLock();

	vector<JEventProcessor*>::iterator it;
	it=app->GetProcessors().begin();
	jout<<" AAA "<<app->GetProcessors().size()<<" "<<(*it)->static_className()<<std::endl;

	for (it=app->GetProcessors().begin();it<app->GetProcessors().end();it++){
		printf("%p %p \n",*it,this);
		if ((*it)!= NULL) jout<<"hereAA: "<<(*it)->className()<<std::endl;
	}
	app->RootUnLock();
	// Create histograms here
	return NOERROR;
}

// brun
jerror_t BDXEventProcessor::brun(JEventLoop *eventLoop, int runnumber)
{
	return NOERROR;
}

// evnt
jerror_t BDXEventProcessor::evnt(JEventLoop *loop, int eventnumber)
{
	//
	//	vector<const marcoCluster*> marcoC;
	//	loop->Get(marcoC);

	//vector<const ctofHitR*> marcoC;
	//loop->Get(marcoC);

	vector<const fa250Mode1Hit*> faHit;
	loop->Get(faHit);




	return NOERROR;
}

// erun
jerror_t BDXEventProcessor::erun(void)
{
	// Any final calculations on histograms (like dividing them)
	// should be done here. This may get called more than once.
	return NOERROR;
}

// fini
jerror_t BDXEventProcessor::fini(void)
{
	// If another DEventProcessor is in the list ahead of this one, then
	// it will have finished before this is called. e.g. closed the
	// ROOT file!
	return NOERROR;
}

