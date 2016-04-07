// $Id$
//
//    File: JEventProcessor_muon_eff.h
// Created: Wed Apr  6 09:45:37 CEST 2016
// Creator: lmarsicano (on Linux apcx14 2.6.32-573.el6.x86_64 x86_64)
//

#ifndef _JEventProcessor_muon_eff_
#define _JEventProcessor_muon_eff_

#include <JANA/JEventProcessor.h>
class TH1D;
class TH2D;
class TF1;
class TTree;
class JROOTOutput;
class IntVetoDigiHit;
class JEventProcessor_muon_eff:public jana::JEventProcessor{
	public:
		JEventProcessor_muon_eff();
		~JEventProcessor_muon_eff();
		const char* className(void){return "JEventProcessor_muon_eff";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.


		int m_isFirstCallToBrun;
		TH1D *hp1,*hp2,*hc1,*hc2,*hctot;
		TH2D *hp1_2,*hc1_2;
		TTree *t;
		JROOTOutput *m_ROOTOutput;

		int isMC;

		//data variables
		double Qc1,Qc2,Ep1,Ep2,Qctot,Tp1,Tp2,Tpdiff,Tc1,Tc2;

		double  IntVetoTopDigiHitQ;
		const IntVetoDigiHit* IntVetoTopDigiHit;
		//MC variables
		double Ec_MC;

		int eventN;
};

#endif // _JEventProcessor_muon_eff_

