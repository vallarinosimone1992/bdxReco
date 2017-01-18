// $Id$
//
//    File: JEventProcessor_Calorimeter_SipmStudies.h
// Created: Mon Apr  4 15:27:17 CEST 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _JEventProcessor_Calorimeter_SipmStudies_
#define _JEventProcessor_Calorimeter_SipmStudies_

#include <JANA/JEventProcessor.h>
class TTree;
class JROOTOutput;
class CalorimeterSiPMHit;

class JEventProcessor_Calorimeter_SipmStudies:public jana::JEventProcessor{
public:
	JEventProcessor_Calorimeter_SipmStudies();
	~JEventProcessor_Calorimeter_SipmStudies();
	const char* className(void){return "JEventProcessor_Calorimeter_SipmStudies";}

private:
	jerror_t init(void);						///< Called once at program start.
	jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
	jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
	jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
	jerror_t fini(void);						///< Called after last event of last event source has been processed.

	TTree *t;

	int eventNumber;
	int m_isFirstCallToBrun;

/*	int m_type1,m_type2;
	double m_Q1,m_Q2;
	double m_A1,m_A2;
	double m_Q1s,m_Q2s;
	double m_T1,m_T2;
	double m_ped1,m_ped2;
	double m_pedSigma1,m_pedSigma2;*/

	double Ec1,Ec2,Ec;

	vector <const CalorimeterSiPMHit*> *vhit;

	JROOTOutput *m_ROOTOutput;

};

#endif // _JEventProcessor_Calorimeter_SipmStudies_

