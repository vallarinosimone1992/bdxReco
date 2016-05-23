/*
 * ExtVeto_check.h
 *
 *  Created on: 18 feb 2016
 *      Author: Mariangela
 */

#ifndef _ExtVeto_check_
#define _ExtVeto_check_

#include <JANA/JEventProcessor.h>
#include "TH1D.h"
class TH1D;
class TH2D;
class TTree;
class JROOTOutput;

class ExtVeto_check:public jana::JEventProcessor{
	public:
		ExtVeto_check();
		~ExtVeto_check();
		const char* className(void){return "ExtVeto_ check";}

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

	//	double Q, pedestal, T;
	   double Q,pedestal, T, T_B, Threshold_B, rise_TB, rise_T;
		int component,eventN, Tw;
	    double A[500], B[500];
        int time[500];

	    //	TH1D* h_wave=new TH1D("h_wave","h_wave",100,-0.5,99.5);


};

#endif // _ExtVeto_SigDisplay_

