// $Id$
//
//    File: JFactoryGenerator_EventBuilder.h
// Created: Tue Apr 26 00:28:27 CEST 2016
// Creator: celentan (on Linux dhcp-visitor-enr-116-196.slac.stanford.edu 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#ifndef _JFactoryGenerator_EventBuilder_
#define _JFactoryGenerator_EventBuilder_

#include <JANA/jerror.h>
#include <JANA/JFactoryGenerator.h>

#include "CataniaEvent_factory.h"
#include "MCEvent_factory.h"


#include "TEvent_factory_CataniaProto2.h"

class JFactoryGenerator_EventBuilder: public jana::JFactoryGenerator{
	public:
		JFactoryGenerator_EventBuilder(){}
		virtual ~JFactoryGenerator_EventBuilder(){}
		virtual const char* className(void){return static_className();}
		static const char* static_className(void){return "JFactoryGenerator_EventBuilder";}
		
		jerror_t GenerateFactories(jana::JEventLoop *loop){
			loop->AddFactory(new CataniaEvent_factory());
			loop->AddFactory(new MCEvent_factory());
			loop->AddFactory(new TEvent_factory_CataniaProto2());
			return NOERROR;
		}

};

#endif // _JFactoryGenerator_EventBuilder_

