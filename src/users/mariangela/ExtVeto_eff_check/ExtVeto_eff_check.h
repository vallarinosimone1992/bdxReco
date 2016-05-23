/*
 * ExtVeto_eff_check.h
 *
 *  Created on: 18 feb 2016
 *      Author: Mariangela
 */

#ifndef _ExtVeto_eff_check_
#define _ExtVeto_eff_check_

#include <JANA/JEventProcessor.h>
#include "TH1D.h"
class TH1D;
class TH2D;
class TTree;
class JROOTOutput;

class ExtVeto_eff_check:public jana::JEventProcessor{
	public:
		ExtVeto_eff_check();
		~ExtVeto_eff_check();
		const char* className(void){return "ExtVeto_eff_check";}

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
	  double multi_iv, multi_ev, eventN;
	  double E_cal;
	  double A[500], B[500];

	  double T_iv, T_ev,T_cal;


	  int multi_iv_0, multi_iv_1, multi_iv_2, multi_iv_3, multi_iv_4, multi_iv_5, multi_iv_6;
	  int multi_ev_0, multi_ev_1, multi_ev_2, multi_ev_3, multi_ev_4, multi_ev_5, multi_ev_6,multi_ev_7, multi_ev_8, multi_ev_9, multi_ev_10, multi_ev_11;
	  int time[500], Tw;

	  int multi_iv_c, multi_ev_c;
	  int good_ped;

		double E_up,E_down,E6,E9;
		double T_up,T_down,T6,T9;

	    //	TH1D* h_wave=new TH1D("h_wave","h_wave",100,-0.5,99.5);


};

#endif // _ExtVeto_SigDisplay_

