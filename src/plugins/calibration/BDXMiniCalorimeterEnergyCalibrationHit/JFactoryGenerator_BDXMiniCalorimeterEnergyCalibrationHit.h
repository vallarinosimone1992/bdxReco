// $Id$
//
//    File: JFactoryGenerator_BDXMiniCalorimeterEnergyCalibrationHit.h
// Created: Wed Apr  3 17:08:28 CEST 2019
// Creator: celentan (on Linux apcx4 2.6.32-696.13.2.el6.x86_64 x86_64)
//

#ifndef _JFactoryGenerator_BDXMiniCalorimeterEnergyCalibrationHit_
#define _JFactoryGenerator_BDXMiniCalorimeterEnergyCalibrationHit_

#include <JANA/jerror.h>
#include <JANA/JFactoryGenerator.h>

#include "BDXMiniCalorimeterEnergyCalibrationHit_factory.h"

class JFactoryGenerator_BDXMiniCalorimeterEnergyCalibrationHit: public jana::JFactoryGenerator{
	public:
		JFactoryGenerator_BDXMiniCalorimeterEnergyCalibrationHit(){}
		virtual ~JFactoryGenerator_BDXMiniCalorimeterEnergyCalibrationHit(){}
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "JFactoryGenerator_BDXMiniCalorimeterEnergyCalibrationHit";}
		
		jerror_t GenerateFactories(jana::JEventLoop *loop){
			loop->AddFactory(new BDXMiniCalorimeterEnergyCalibrationHit_factory());
			return NOERROR;
		}

};

#endif // _JFactoryGenerator_BDXMiniCalorimeterEnergyCalibrationHit_

