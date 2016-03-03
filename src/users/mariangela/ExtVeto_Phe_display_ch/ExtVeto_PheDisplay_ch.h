/*
 * ExtVeto_PheDisplay.h
 *
 *  Created on: 18 feb 2016
 *      Author: Mariangela
 */

#ifndef _ExtVeto_PheDisplay_ch_
#define _ExtVeto_PheDisplay_ch_

#include <JANA/JEventProcessor.h>
#include "TH1D.h"
class TH1D;
class TH2D;
class TTree;
class JROOTOutput;

class ExtVeto_PheDisplay_ch:public jana::JEventProcessor{
	public:
		ExtVeto_PheDisplay_ch();
		~ExtVeto_PheDisplay_ch();
		const char* className(void){return "ExtVeto_PheDisplay_ch";}

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
       double Q;
		double Q0, Q1, Q2, Q3, Q4, Q5, Q6, Q7, Q8, Q9, Q10, Q11;
		 double T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11;
	//   double Q,pedestal, T, phe;

			int component,eventN, Tw;
	    double A[100], B[100];
        int time[100];

	    //	TH1D* h_wave=new TH1D("h_wave","h_wave",100,-0.5,99.5);


};

#endif // _ExtVeto_SigDisplay_ch

