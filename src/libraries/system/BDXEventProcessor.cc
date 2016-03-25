// C++ headers
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
using namespace std;

// bdx headers
#include "BDXEventProcessor.h"
#include "string_utilities.h"
//DAQ
#include <DAQ/eventData.h>

// TT
#include <TT/TranslationTable.h>

#include "TTree.h"


#include "JOutput.h"
#include "JROOTOutput.h"

// Constructor
BDXEventProcessor::BDXEventProcessor():
				m_output(0),m_tt(0),isMC(0)
{
	/* Opens output file if specified
	the string should be of the form
	type filename
	with type: TXT EVIO ROOT
	and filename anything
	 */

	optf="";


	startTime=99999999;
	stopTime=-99999999;
}

// Destructor
BDXEventProcessor::~BDXEventProcessor()
{

}

// init
jerror_t BDXEventProcessor::init(void)
{

	jout<<"BDXEventProcessor::init"<<endl;





	gPARMS->SetDefaultParameter("SYSTEM:OUTPUT",optf,
			"Set OUTPUT file type and name, using the form \"TYPE,FILENAME\". Type can be ROOT, EVIO, TXT.\n"
			"Example: -PSYSTEM:OUTPUT=\"ROOT,out.root\" ");

	outType.assign(optf, 0, optf.find(",")) ;
	outFile.assign(optf,    optf.find(",") + 1, optf.size()) ;


	std::transform(outType.begin(),outType.end(), outType.begin(),::tolower);


	if(optf!= "none")
	{
		jout<<"Out file type is: "<<outType<<endl;
		if(outType == "root"){
			m_output=new JROOTOutput(outFile);
		}
		else if (outType == "evio"){
			jerr<<"evio not yet implemented"<<endl;
		}
		else if (outType == "txt"){
			jerr<<"txt not yet implemented"<<endl;
		}
		else{
			jerr<<"file type not recognized"<<endl;
		}

	}
	gPARMS->GetParameter("MC", isMC);

	japp->RootWriteLock();

	t=new TTree("EventHeader","EventHeader");
	t->Branch("eventN",&eventN);
	t->Branch("runN",&runN);
	t->Branch("T",&eventT);
	t->Branch("tword",&tword);
	if (m_output){
		if (m_output->className()=="JRootOutput"){
			JROOTOutput* m_ROOTOutput=(JROOTOutput*)m_output;
			(m_ROOTOutput)->AddObject(t);
		}
	}
	return NOERROR;
}

// brun
jerror_t BDXEventProcessor::brun(JEventLoop *eventLoop, int32_t runnumber)
{

	jout<<"BDXEventProcessor::brun "<<runnumber<<endl;
	if (isMC==0) {
		eventLoop->GetSingle(m_tt);
	}
	return NOERROR;
}

// evnt
jerror_t BDXEventProcessor::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	const eventData* tData;
	if (isMC){
		return NOERROR;
	}
	else{
		try{
			loop->GetSingle(tData);
		}
		catch(unsigned long e){
			jout<<"No eventData bank this event"<<endl;
			return 	OBJECT_NOT_AVAILABLE;
		}
		japp->RootWriteLock();

		eventT=tData->time;
		eventN=eventnumber;
		tword=tData->triggerWords[0];
		runN=tData->runN;
		t->Fill();


		/*Time*/
		if (eventT<startTime) startTime=eventT;
		if (eventT>stopTime)  stopTime=eventT;
		japp->RootUnLock();

		return NOERROR;
	}
}

// erun
jerror_t BDXEventProcessor::erun(void)
{
	deltaTime=stopTime-startTime;
	jout<<"BDXEventProcessor::erun "<<endl;
	jout<<"Run start: "<<startTime<<" stop: "<<stopTime<<" diff: "<<deltaTime<<endl;

	return NOERROR;
}

// fini
jerror_t BDXEventProcessor::fini(void)
{
	// If another DEventProcessor is in the list ahead of this one, then
	// it will have finished before this is called. e.g. closed the
	// ROOT file!
	if (m_output){
		m_output->CloseOutput();
	}


	return NOERROR;
}

