/*
 * ExtVeto_eff.h
 *
 *  Created on: 18 feb 2016
 *      Author: Mariangela
 */

#ifndef _ExtVeto_eff_
#define _ExtVeto_eff_

#include <JANA/JEventProcessor.h>
#include "TH1D.h"
class TH1D;
class TH2D;
class TTree;
class JROOTOutput;

class ExtVeto_eff:public jana::JEventProcessor{
	public:
		ExtVeto_eff();
		~ExtVeto_eff();
		const char* className(void){return "ExtVeto_eff";}

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
	   int multi_ev, multi_iv, multi_cal;
	   int multi_iv_c, multi_ev_c;
	   double E_ev, T_ev;
	   double E_iv, T_iv;
	   double E_cal, T_cal;
	   int Tw;
	   int good_ped;


	    //	TH1D* h_wave=new TH1D("h_wave","h_wave",100,-0.5,99.5);


};

#endif // _ExtVeto_SigDisplay_

