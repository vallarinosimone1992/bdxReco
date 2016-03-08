#ifndef _EventProcessor_
#define _EventProcessor_

// JANA headers
#include <JANA/JEventProcessor.h>
#include <JANA/JApplication.h>
using namespace jana;



// bank headers
//#include "gbank.h"

// EVIO headers
#include "evioUtil.hxx"
#include "evioFileChannel.hxx"
using namespace evio;

//ROOT headers
#include "TFile.h"

class TranslationTable;
class JOutput;

/*This class is the "main" event processor, that gets called in any case,
 * also if we use users plugins
 *
 * Here I want to handle the output in a proper way.
 */
class BDXEventProcessor:public JEventProcessor
{
	public:
		 BDXEventProcessor();
		~BDXEventProcessor();
		const char* className(void){return "BDXEventProcessor";}  /*A.C. DO NOT TOUCH THIS!*/

		JOutput* getOutput(){return m_output;}

	private:
		jerror_t init();                                 // Called once at program start.
		jerror_t brun(JEventLoop*, int32_t runnumber);       // Called everytime a new run number is detected.
		jerror_t evnt(JEventLoop*, uint64_t eventnumber);     // Called every event.
		jerror_t erun();                                 // Called everytime run number changes, provided brun has been called.
		jerror_t fini();                                 // Called after last event of last event source has been processed.

		string           optf;     // Output file
		string			 outType,outFile;

		JOutput		*m_output;
		const TranslationTable *m_tt;
		int isMC;
	//		TFile *outf;



};

#endif

