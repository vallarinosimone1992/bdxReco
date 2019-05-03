// $Id$
//
//    File: BDXMiniCalorimeterEnergyCalibrationHit_factory.h
// Created: Wed Apr  3 17:08:28 CEST 2019
// Creator: celentan (on Linux apcx4 2.6.32-696.13.2.el6.x86_64 x86_64)
//

#ifndef _BDXMiniCalorimeterEnergyCalibrationHit_factory_
#define _BDXMiniCalorimeterEnergyCalibrationHit_factory_

#include <JANA/JFactory.h>
#include "BDXMiniCalorimeterEnergyCalibrationHit.h"



class BDXMiniCalorimeterEnergyCalibrationHit_factory:public jana::JFactory<BDXMiniCalorimeterEnergyCalibrationHit>{
	public:
		BDXMiniCalorimeterEnergyCalibrationHit_factory(){};
		~BDXMiniCalorimeterEnergyCalibrationHit_factory(){};

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.

		int m_isMC;

		double m_thrIntVetoCaps;
		double m_thrIntVetoL;

		std::map<int,std::vector<std::pair<int,int>>> corrTOP; // The correspondance ID of IntVeto L1 max SiPM ->  (X,Y).
		std::map<int,std::vector<std::pair<int,int>>> corrBOTTOM; // The correspondance ID of IntVeto L1 max SiPM ->  (X,Y).

		std::map<std::pair<int,int>,int > bitTOP;     //Correspondance TOP -> bit
		std::map<std::pair<int,int>,int > bitBOTTOM;  //Correspondance BOTTOM -> bit

		static const int nChansMax=64;
		int chanTimeMin,chanTimeMax;



		int getCrystalChanBit(int sector,int ix,int iy);


};

#endif // _BDXMiniCalorimeterEnergyCalibrationHit_factory_

