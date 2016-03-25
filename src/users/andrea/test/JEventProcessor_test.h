// $Id$
//
//    File: JEventProcessor_test.h
// Created: Mon Dec 28 15:48:21 CET 2015
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _JEventProcessor_test_
#define _JEventProcessor_test_

#include <JANA/JEventProcessor.h>
#include <system/CalibrationHandler.h>

class TH1D;
class TH2D;
class TF1;
class TTree;
class JROOTOutput;

class JEventProcessor_test:public jana::JEventProcessor{
	public:
		JEventProcessor_test();
		~JEventProcessor_test();
		const char* className(void){return "JEventProcessor_test";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		double HH(double x);
		double fun(double t,double t0);
		double integrand(double *x,double *par);
		double integrated(double t);

		int m_isFirstCallToBrun;
		TH1D *h,*h1;
		TTree *t;
		JROOTOutput *m_ROOTOutput;

		double Q1,Q2,Q;
		int component,eventN;

		CalibrationHandler<TranslationTable::CALO_Index_t> m_pedestal;

		TH1D *hGlob;
		TF1  *fGlob;

		double tau;
		double T0;

};

#endif // _JEventProcessor_test_

