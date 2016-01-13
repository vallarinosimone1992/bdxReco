// $Id$
//
//    File: TranslationTable_factory.cc
// Created: Tue Jan 12 09:29:14 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "TranslationTable_factory.h"
using namespace jana;

//------------------
// init
//------------------
jerror_t TranslationTable_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t TranslationTable_factory::brun(jana::JEventLoop *eventLoop, int runnumber)
{
	// Grab run-dependent translation table from CCDB
	tt = new TranslationTable(eventLoop);

	// Keep this translation table around and reuse it for
	// susequent events
	_data.push_back(tt);
	SetFactoryFlag(PERSISTANT);  /*This is very, very important: the Reset Method is not called at every event, so we do not clear data, and we
	can gate the TranslationTable also in evnt method of other factories / event processors*/

	// If restricting parsing, make sure it is set for this source
//	tt->SetSystemsToParse(loop->GetJEvent().GetJEventSource());


	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t TranslationTable_factory::evnt(JEventLoop *loop, int eventnumber)
{

	// Code to generate factory data goes here. Add it like:
	//
	// TranslationTable *myTranslationTable = new TranslationTable;
	// myTranslationTable->x = x;
	// myTranslationTable->y = y;
	// ...
	// _data.push_back(myTranslationTable);
	//
	// Note that the objects you create here will be deleted later
	// by the system and the _data vector will be cleared automatically.

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t TranslationTable_factory::erun(void)
{

	// If we have a translation table already the delete it
	if(tt){
		delete tt;
	}
	_data.clear();
	return NOERROR;

}

//------------------
// fini
//------------------
jerror_t TranslationTable_factory::fini(void)
{
	return NOERROR;
}

