// $Id$
//
//    File: JEventProcessor_CalorimeterSipm_calib.h
// Created: Thu Feb 18 11:55:18 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _JEventProcessor_Calorimeter_SipmCalib_
#define _JEventProcessor_Calorimeter_SipmCalib_

#include <JANA/JEventProcessor.h>
class TTree;
class JROOTOutput;
class TH1D;
class JEventProcessor_Calorimeter_SipmCalib:public jana::JEventProcessor{
	public:
		JEventProcessor_Calorimeter_SipmCalib();
		~JEventProcessor_Calorimeter_SipmCalib();
		const char* className(void){return "JEventProcessor_Calorimeter_SipmCalib";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.


		TTree *t;
		TH1D *h;

		int eventNumber;
		int m_isFirstCallToBrun;
		int m_type;
		int m_sector,m_x,m_y,m_readout;

		int m_singles,m_signals;
		double Qphe,Qraw,A,T,average,ped,pedSigma;

		JROOTOutput *m_ROOTOutput;
};

#endif
