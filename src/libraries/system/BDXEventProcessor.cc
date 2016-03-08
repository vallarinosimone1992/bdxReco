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

// TT
#include <TT/TranslationTable.h>
//#include "ctofHitR.h"
//#include "marcoCluster.h"


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
	//string hd_msg = bdxOpt.optMap["LOG_MSG"].args + " Event: >> " ;


}

// Destructor
BDXEventProcessor::~BDXEventProcessor()
{

}

// init
jerror_t BDXEventProcessor::init(void)
{

	jout<<"BDXEventProcessor::init is called"<<endl;





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

	return NOERROR;
}

// brun
jerror_t BDXEventProcessor::brun(JEventLoop *eventLoop, int32_t runnumber)
{
	if (isMC==0) {
		eventLoop->GetSingle(m_tt);
	}
	return NOERROR;
}

// evnt
jerror_t BDXEventProcessor::evnt(JEventLoop *loop, uint64_t eventnumber)
{


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
	if (m_output){
		m_output->CloseOutput();
	}


	return NOERROR;
}

