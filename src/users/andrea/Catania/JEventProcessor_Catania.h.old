// $Id$
//
//    File: JEventProcessor_Calorimeter_rate.h
// Created: Fri Mar 25 18:38:03 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _JEventProcessor_Catania
#define _JEventProcessor_Catania

#include <JANA/JEventProcessor.h>

class TTree;
class JROOTOutput;
class TH1D;
class CalorimeterHit;
class CalorimeterSiPMHit;
class PaddlesHit;
class CataniaEvent;

class JEventProcessor_Catania:public jana::JEventProcessor{
	public:
		JEventProcessor_Catania();
		~JEventProcessor_Catania();
		const char* className(void){return "JEventProcessor_Catania";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		int eventNumber;
		int m_isFirstCallToBrun;
		int m_isMC;

		const CataniaEvent* event;
		const CalorimeterHit* caloHit;
		double EcMC;




		TTree *t;
		vector<TH1D *> hwavesCalo;
		vector<TH1D *> hwavesIntVeto;
		vector<TH1D *> hwavesExtVeto;

		TH1D*		   hwave;
		JROOTOutput *m_ROOTOutput;
};

#endif // _JEventProcessor_Calorimeter_rate_

