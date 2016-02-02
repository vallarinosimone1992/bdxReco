// $Id$
//
//    File: JEventProcessor_sipm_calib.h
// Created: Tue Feb  2 18:52:32 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _JEventProcessor_sipm_calib_
#define _JEventProcessor_sipm_calib_

#include <JANA/JEventProcessor.h>
#include <TT/TranslationTable.h>
class TTree;
class JROOTOutput;
class JEventProcessor_sipm_calib:public jana::JEventProcessor{
	public:
		JEventProcessor_sipm_calib();
		~JEventProcessor_sipm_calib();
		const char* className(void){return "JEventProcessor_sipm_calib";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		TTree *t;

		int eventNumber;
		int m_isFirstCallToBrun;

		int m_sector,m_layer,m_component,m_readout;
		double Q;


		JROOTOutput *m_ROOTOutput;


};

#endif // _JEventProcessor_sipm_calib_

