// $Id$
//
//    File: JFactoryGenerator_BDXMiniVetoSIPMCalibrationHit.h
// Created: Mon Aug 19 18:11:03 CEST 2019
// Creator: celentan (on Linux apcx4 3.10.0-957.el7.x86_64 x86_64)
//

#ifndef _JFactoryGenerator_BDXMiniVetoSIPMCalibrationHit_
#define _JFactoryGenerator_BDXMiniVetoSIPMCalibrationHit_

#include <JANA/jerror.h>
#include <JANA/JFactoryGenerator.h>

#include "BDXMiniVetoSIPMCalibrationHit_factory.h"

class JFactoryGenerator_BDXMiniVetoSIPMCalibrationHit: public jana::JFactoryGenerator{
	public:
		JFactoryGenerator_BDXMiniVetoSIPMCalibrationHit(){}
		virtual ~JFactoryGenerator_BDXMiniVetoSIPMCalibrationHit(){}
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "JFactoryGenerator_BDXMiniVetoSIPMCalibrationHit";}
		
		jerror_t GenerateFactories(jana::JEventLoop *loop){
			loop->AddFactory(new BDXMiniVetoSIPMCalibrationHit_factory());
			return NOERROR;
		}

};

#endif // _JFactoryGenerator_BDXMiniVetoSIPMCalibrationHit_

