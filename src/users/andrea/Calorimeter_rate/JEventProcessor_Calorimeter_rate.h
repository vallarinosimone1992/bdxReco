// $Id$
//
//    File: JEventProcessor_Calorimeter_rate.h
// Created: Fri Mar 25 18:38:03 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _JEventProcessor_Calorimeter_rate_
#define _JEventProcessor_Calorimeter_rate_

#include <JANA/JEventProcessor.h>

class TTree;
class JROOTOutput;
class TH1D;
class CalorimeterHit;
class JEventProcessor_Calorimeter_rate:public jana::JEventProcessor{
	public:
		JEventProcessor_Calorimeter_rate();
		~JEventProcessor_Calorimeter_rate();
		const char* className(void){return "JEventProcessor_Calorimeter_rate";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		int eventNumber;
		int m_isFirstCallToBrun;

		const CalorimeterHit *caloHit;
		double Ec,Ec1,Ec2;
		int nHitsIntVeto,nHitsExtVeto;

		TTree *t;
		vector<TH1D *> hwavesCalo;
		vector<TH1D *> hwavesIntVeto;
		vector<TH1D *> hwavesExtVeto;

		TH1D*		   hwave;
		JROOTOutput *m_ROOTOutput;
};

#endif // _JEventProcessor_Calorimeter_rate_

