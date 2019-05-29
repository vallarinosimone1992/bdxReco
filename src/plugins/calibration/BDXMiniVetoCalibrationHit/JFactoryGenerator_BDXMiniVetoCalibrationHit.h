// $Id$
//
//    File: JFactoryGenerator_BDXMiniVetoCalibrationHit.h
// Created: Fri May 17 11:56:57 CEST 2019
// Creator: celentan (on Linux apcx4 3.10.0-957.el7.x86_64 x86_64)
//

#ifndef _JFactoryGenerator_BDXMiniVetoCalibrationHit_
#define _JFactoryGenerator_BDXMiniVetoCalibrationHit_

#include <JANA/jerror.h>
#include <JANA/JFactoryGenerator.h>

#include "BDXMiniVetoCalibrationHit_factory.h"

class JFactoryGenerator_BDXMiniVetoCalibrationHit: public jana::JFactoryGenerator{
	public:
		JFactoryGenerator_BDXMiniVetoCalibrationHit(){}
		virtual ~JFactoryGenerator_BDXMiniVetoCalibrationHit(){}
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "JFactoryGenerator_BDXMiniVetoCalibrationHit";}
		
		jerror_t GenerateFactories(jana::JEventLoop *loop){
			loop->AddFactory(new BDXMiniVetoCalibrationHit_factory());
			return NOERROR;
		}

};

#endif // _JFactoryGenerator_BDXMiniVetoCalibrationHit_

