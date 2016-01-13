// $Id$
//
//    File: JFactoryGenerator_VETO_INT.h
// Created: Tue Jan 12 11:52:51 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//

#ifndef _JFactoryGenerator_VETO_INT_
#define _JFactoryGenerator_VETO_INT_

#include <JANA/jerror.h>
#include <JANA/JFactoryGenerator.h>

#include "VetoIntDigiHit_factory.h"

class JFactoryGenerator_VETO_INT: public jana::JFactoryGenerator{
	public:
		JFactoryGenerator_VETO_INT(){}
		virtual ~JFactoryGenerator_VETO_INT(){}
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "JFactoryGenerator_VETO_INT";}
		
		jerror_t GenerateFactories(jana::JEventLoop *loop){
			loop->AddFactory(new VetoIntDigiHit_factory());
			return NOERROR;
		}

};

#endif // _JFactoryGenerator_VETO_INT_

