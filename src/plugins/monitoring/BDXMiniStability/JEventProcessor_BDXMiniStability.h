// $Id$
//
//    File: JEventProcessor_BDXMiniStability.h
// Created: Fri May 24 19:02:10 CEST 2019
// Creator: celentan (on Linux apcx4 3.10.0-957.el7.x86_64 x86_64)
//

#ifndef _JEventProcessor_BDXMiniStability_
#define _JEventProcessor_BDXMiniStability_

#include <JANA/JEventProcessor.h>
class JROOTOutput;




class JEventProcessor_BDXMiniStability: public jana::JEventProcessor {
public:
	JEventProcessor_BDXMiniStability();
	~JEventProcessor_BDXMiniStability();
	const char* className(void) {
		return "JEventProcessor_BDXMiniStability";
	}

private:
	jerror_t init(void);						///< Called once at program start.
	jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
	jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
	jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
	jerror_t fini(void);						///< Called after last event of last event source has been processed.

	int m_isMC;
	int m_isFirstCallToBrun;
	JROOTOutput *m_ROOTOutput;


	double m_thrVetoCaps;
	double m_thrVetoL;

	double m_thrEneTot,m_thrEneSeed;

	unsigned long int m_dT, m_dT2;
	unsigned long int    m_T0,m_T;
	int m_isT0Set;
	unsigned long int index, index2;

	int m_nbins;

	int m_nbinsL0[10];
	int m_nbinsL1[10];

	static const int m_nchargestep =80;
	static constexpr double m_chargemin = 0;
	static constexpr double m_chargemax = 10;


	/*This algorithm is also supposed to run online.
	 * Hence, we don't know a priori the run length. We use maps to accumulate statistics.
	 */
	map<unsigned long int,unsigned int>::iterator map_it;
	map<unsigned long int,unsigned int[m_nchargestep]>::iterator rateL0_it[10];
	map<unsigned long int,unsigned int[m_nchargestep]>::iterator rateL1_it[10];
	//Rate: Layer 0 - Layer 1
	map<unsigned long int,unsigned int[m_nchargestep]> rateL0[10];
	map<unsigned long int,unsigned int[m_nchargestep]> rateL1[10];

	map<unsigned long int,unsigned int> allEvents;
	vector< map<unsigned long int,unsigned int> > trgs;

	//Large energy events - any event with at least 1 cluster above m_thrEneTot and m_thrEneSeed
	map<unsigned long int,unsigned int> highE;

	//Large energy events - veto anticoincidence
	map<unsigned long int,unsigned int> highE_antiVeto;




};

#endif // _JEventProcessor_BDXMiniStability_

