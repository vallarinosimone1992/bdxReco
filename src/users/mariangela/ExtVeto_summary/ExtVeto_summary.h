

#ifndef _ExtVeto_summary_
#define _ExtVeto_summary_

#include <JANA/JEventProcessor.h>
#include "TH1D.h"
#include <TT/TranslationTable.h>
class TH1D;
class TH2D;
class TTree;
class JROOTOutput;

class ExtVeto_summary:public jana::JEventProcessor{
	public:
		ExtVeto_summary();
		~ExtVeto_summary();
		const char* className(void){return "ExtVeto_Hit";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		int m_isFirstCallToBrun;
		TH1D *h;
		TTree *t;
		JROOTOutput *m_ROOTOutput;
       double E,T;

	//   double Q,pedestal, T, phe;

		int component,eventN, Tw;
	    double A[100];
        int time[100];
        int sum;
    //    TranslationTable::EXT_VETO_Index_t sum_chan[12];
        int sum_chan[12];

	    //	TH1D* h_wave=new TH1D("h_wave","h_wave",100,-0.5,99.5);


};

#endif

