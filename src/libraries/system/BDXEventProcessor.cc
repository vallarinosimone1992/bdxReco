// C++ headers
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
using namespace std;

// bdx headers
#include "BDXEventProcessor.h"
#include "string_utilities.h"
//DAQ
#include <DAQ/eventData.h>
// TT
#include <TT/TranslationTable.h>

//DST
#include <EventBuilder/TEvent.h>

#include "TTree.h"


#include "JOutput.h"
#include "JROOTOutput.h"

// Constructor
BDXEventProcessor::BDXEventProcessor():
										m_output(0),m_tt(0),m_isMC(0),m_buildDST(0),m_eventDST(0),m_runInfo(0),m_event(0)
{
	optf="";


	startTime=99999999;
	stopTime=-99999999;

	bout.SetTag("BDXEventProcessor >>");
	berr.SetTag("BDXEventProcessorError >>");
}

// Destructor
BDXEventProcessor::~BDXEventProcessor()
{

}

// init
jerror_t BDXEventProcessor::init(void)
{

	bout<<"BDXEventProcessor::init"<<endl;
	gPARMS->GetParameter("MC",m_isMC);
	gPARMS->SetDefaultParameter("SYSTEM:BUILD_DST",m_buildDST,"Build the DST output");
	bout<<"Building DST is: "<<m_buildDST<<endl;





	gPARMS->SetDefaultParameter("SYSTEM:OUTPUT",optf,"Set OUTPUT file type and name, using the form \"TYPE,FILENAME\". Type can be ROOT, EVIO, TXT. Example: -PSYSTEM:OUTPUT=\"ROOT,out.root\" ");

	outType.assign(optf, 0, optf.find(",")) ;
	outFile.assign(optf,    optf.find(",") + 1, optf.size()) ;


	std::transform(outType.begin(),outType.end(), outType.begin(),::tolower);


	if(optf!= "none")
	{
		bout<<"Out file type is: "<<outType<<endl;
		if(outType == "root"){
			m_output=new JROOTOutput(outFile);
		}
		else if (outType == "evio"){
			berr<<"evio not yet implemented"<<endl;
		}
		else if (outType == "txt"){
			berr<<"txt not yet implemented"<<endl;
		}
		else{
			berr<<"file type not recognized"<<endl;
		}

	}


	japp->RootWriteLock();
	m_eventHeader=new TTree("EventHeader","EventHeader");
	m_eventHeader->Branch("eventN",&eventN);
	m_eventHeader->Branch("runN",&runN);
	m_eventHeader->Branch("T",&eventT);
	m_eventHeader->Branch("tword",&tword);

	m_runInfo=new TTree("RunInfo","RunInfo");
	m_runInfo->Branch("runN",&runN);
	m_runInfo->Branch("dT",&deltaTime);
	if (m_output){
		if (m_output->className()=="JRootOutput"){
			JROOTOutput* m_ROOTOutput=(JROOTOutput*)m_output;
			(m_ROOTOutput)->AddObject(m_eventHeader);
			(m_ROOTOutput)->AddObject(m_runInfo);
		}
	}

	if (m_buildDST){
		m_eventDST=new TTree("EventDST","EventDST");
		m_eventDST->Branch("CataniaEventProto2",&m_event);
		m_eventDST->Branch("Event",&m_event);
		if (m_isMC==0) m_eventDST->AddFriend(m_eventHeader);
	}



	japp->RootUnLock();

	return NOERROR;
}

// brun
jerror_t BDXEventProcessor::brun(JEventLoop *eventLoop, int32_t runnumber)
{

	bout<<"BDXEventProcessor::brun "<<runnumber<<endl;
	if (m_isMC==0) {
		eventLoop->GetSingle(m_tt);
	}



	return NOERROR;
}

// evnt
jerror_t BDXEventProcessor::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	const eventData* tData;
	vector<const TEvent*> events;
	if (m_buildDST){
		loop->Get(events,"CataniaProto2");
		if (events.size()!=1){
			bout<<"Not a single TEvent object in this event but: "<<events.size()<<endl;
			return RESOURCE_UNAVAILABLE;
		}
		japp->RootWriteLock();
		m_event=events[0];
		m_eventDST->Fill();
		japp->RootUnLock();
	}

	if (m_isMC==0){
		try{
			loop->GetSingle(tData);
		}
		catch(unsigned long e){
			bout<<"No eventData bank this event"<<endl;
			return 	OBJECT_NOT_AVAILABLE;
		}
		japp->RootWriteLock();
		eventT=tData->time;
		eventN=eventnumber;
		tword=tData->triggerWords[0];
		runN=tData->runN;
		m_eventHeader->Fill();
		//Time
		if (eventT<startTime) startTime=eventT;
		if (eventT>stopTime)  stopTime=eventT;
		japp->RootUnLock();
	}
	return NOERROR;
}

// erun
jerror_t BDXEventProcessor::erun(void)
{
	japp->RootWriteLock();
	deltaTime=stopTime-startTime;
	bout<<"BDXEventProcessor::erun "<<endl;
	bout<<"Run start: "<<startTime<<" stop: "<<stopTime<<" diff: "<<deltaTime<<endl;
	m_runInfo->Fill();
	japp->RootUnLock();


	return NOERROR;
}

// fini
jerror_t BDXEventProcessor::fini(void)
{
	// If another DEventProcessor is in the list ahead of this one, then
	// it will have finished before this is called. e.g. closed the
	// ROOT file!
	if (m_output){
		m_output->CloseOutput();
	}


	return NOERROR;
}

void BDXEventProcessor::updateCalibration(CalibrationHandlerBase* cal,JEventLoop* eventLoop) {
	/*First, check the table name for cal*/
	string name=cal->getTable();
	/*Verify the key is in the map*/
	m_calibrations_it=m_calibrations.find(name);
	if (m_calibrations_it==m_calibrations.end()){
		berr<<"BDXEventProcessor::updateCalibration error, calibration handler associated with table "<<name<<" was not registered!"<<endl;
		return;
	}

	vector<CalibrationHandlerBase*> calibrations=m_calibrations[name];
	vector<CalibrationHandlerBase*>::iterator calibrations_it;

	/*Another check*/
	calibrations_it=find(calibrations.begin(),calibrations.end(),cal);
	if (calibrations_it==calibrations.end()){
		berr<<"UpdateCalibration, key: "<<name<<" was found but not this specific calibrationHandler!"<<endl;
		return;
	}
	/*Verify if all the calibrators already have been set for this run*/
	/*Since we are multi-thread mode, it is possible this method is called by different factories at different times.
	 * Need to perform a clever check:
	 * 1) Loop over all the calibrations handlers for the given table.
	 * 1A) If ALL off them have been loaded, do nothing
	 * 1B) If NONE of them have been loaded, do that
	 * 1C) If ONE of them at least have been loaded, use it for all the non-calibrated ones
	 */
	bool flagAll=true;
	int calibratedOne=-1;
	for (calibrations_it=calibrations.begin();calibrations_it!=calibrations.end();calibrations_it++){
		if ((*calibrations_it)->hasLoadedCurrentRun()==false) flagAll=false;
		else calibratedOne=std::distance(calibrations.begin(),calibrations_it); //save the index of this calibrated object
	}
	if (flagAll){ /*flagAll is true if ALL off them have been loaded*/
		bout<<"Going to fill CalibrationHandlers for table: "<<name<<" there are: "<<calibrations.size()<<" ALREADY DONE "<<endl;
		return;
	}
	else if (calibratedOne != -1){ /*It means there is at least an already-calibrated object!*/
		bout<<"Going to fill CalibrationHandlers for table: "<<name<<" there are: "<<calibrations.size()<<" Load from data: "<<calibratedOne<<endl;
		for (int ical=0;ical<calibrations.size();ical++){
			if (ical==calibratedOne) continue;
			else if (calibrations[ical]->hasLoadedCurrentRun()==true) continue;
			else{
				calibrations[ical]->fillCalib(calibrations[calibratedOne]->getRawCalibData());
			}
		}
	}
	else{
		/*Get the data*/
		vector< vector < double > > m_data;
		eventLoop->GetCalib(name, m_data);
		bout<<"Going to fill CalibrationHandlers for table: "<<name<<" there are: "<<calibrations.size()<<" Load from DB "<<endl;
		for (calibrations_it=calibrations.begin();calibrations_it!=calibrations.end();calibrations_it++){
			(*calibrations_it)->fillCalib(m_data);
			(*calibrations_it)->setLoadedCurrentRun(true);
		}
	}
	bout<<"Done table"<<name<<endl;
}

void BDXEventProcessor::clearCalibration(CalibrationHandlerBase* cal) {
	/*First, check the table name for cal*/
	string name=cal->getTable();
	/*Verify the key is in the map*/
	m_calibrations_it=m_calibrations.find(name);
	if (m_calibrations_it==m_calibrations.end()){
		berr<<"BDXEventProcessor::clearCalibration error, calibration handler associated with table "<<name<<" was not registered!"<<endl;
		return;
	}

	vector<CalibrationHandlerBase*> calibrations=m_calibrations[name];
	vector<CalibrationHandlerBase*>::iterator calibrations_it;

	/*Another check*/
	calibrations_it=find(calibrations.begin(),calibrations.end(),cal);
	if (calibrations_it==calibrations.end()){
		berr<<"BDXEventProcessor::clearCalibration, key: "<<name<<" was found but not this specific calibrationHandler!"<<endl;
		return;
	}
	for (calibrations_it=calibrations.begin();calibrations_it!=calibrations.end();calibrations_it++){
		(*calibrations_it)->setLoadedCurrentRun(false);
	}
}

void BDXEventProcessor::addCalibration(CalibrationHandlerBase* cal) {
	this->m_calibrations[cal->getTable()].push_back(cal);
}
