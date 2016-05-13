// $Id$
//
//    File: JEventProcessor_MCtest.h
// Created: Mon Mar  7 11:23:10 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _MCtest_
#define _MCtest_

#include <JANA/JEventProcessor.h>
#include "TH1D.h"
class TH1D;
class TH2D;
class TTree;
class JROOTOutput;


class MCtest:public jana::JEventProcessor{
	public:
		MCtest();
		~MCtest();
		const char* className(void){return "MCtest";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		int m_isFirstCallToBrun;

      double phe1_tot, phe2_tot, E_tot, multi_cal, multi_ev, multi_iv;

      double multi_sect0, multi_sect1, multi_sect2, multi_sect3, multi_sect4, multi_sect5, multi_sect6, multi_sect7;

      double E1[800],E2[800];
      int sector_cal[800], x_cal[800],y_cal[800];

      double multi_cl, Eseed, Ecluster;

      int sector_seed, x_seed, y_seed;

      int sector_ev[800], channel_ev[800];
      int sector_iv[800], channel_iv[800];


	  int component,eventN, Tw, sector,x,y;


        TTree *t;
        		JROOTOutput *m_ROOTOutput;

	    //	TH1D* h_wave=new TH1D("h_wave","h_wave",100,-0.5,99.5);


};




#endif // _MCtest_

