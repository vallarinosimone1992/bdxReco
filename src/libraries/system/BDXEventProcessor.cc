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

#include "TTree.h"


#include "JOutput.h"
#include "JROOTOutput.h"

// Constructor
BDXEventProcessor::BDXEventProcessor():
								m_output(0),m_tt(0),isMC(0)
{
	/* Opens output file if specified
	the string should be of the form
	type filename
	with type: TXT EVIO ROOT
	and filename anything
	 */

	optf="";


	startTime=99999999;
	stopTime=-99999999;
}

// Destructor
BDXEventProcessor::~BDXEventProcessor()
{

}

// init
jerror_t BDXEventProcessor::init(void)
{

	jout<<"BDXEventProcessor::init"<<endl;





	gPARMS->SetDefaultParameter("SYSTEM:OUTPUT",optf,
			"Set OUTPUT file type and name, using the form \"TYPE,FILENAME\". Type can be ROOT, EVIO, TXT.\n"
			"Example: -PSYSTEM:OUTPUT=\"ROOT,out.root\" ");

	outType.assign(optf, 0, optf.find(",")) ;
	outFile.assign(optf,    optf.find(",") + 1, optf.size()) ;


	std::transform(outType.begin(),outType.end(), outType.begin(),::tolower);


	if(optf!= "none")
	{
		jout<<"Out file type is: "<<outType<<endl;
		if(outType == "root"){
			m_output=new JROOTOutput(outFile);
		}
		else if (outType == "evio"){
			jerr<<"evio not yet implemented"<<endl;
		}
		else if (outType == "txt"){
			jerr<<"txt not yet implemented"<<endl;
		}
		else{
			jerr<<"file type not recognized"<<endl;
		}

	}
	gPARMS->GetParameter("MC", isMC);

	japp->RootWriteLock();

	t=new TTree("EventHeader","EventHeader");
	t->Branch("eventN",&eventN);
	t->Branch("runN",&runN);
	t->Branch("T",&eventT);
	t->Branch("tword",&tword);
	if (m_output){
		if (m_output->className()=="JRootOutput"){
			JROOTOutput* m_ROOTOutput=(JROOTOutput*)m_output;
			(m_ROOTOutput)->AddObject(t);
		}
	}
	return NOERROR;
}

// brun
jerror_t BDXEventProcessor::brun(JEventLoop *eventLoop, int32_t runnumber)
{

	jout<<"BDXEventProcessor::brun "<<runnumber<<endl;
	if (isMC==0) {
		eventLoop->GetSingle(m_tt);
	}
	/*This is the part where we load ALL the calibrations,
	 * provided they were associated with this processor!
	 */
	/*jout<<"Loading calibrations "<<endl;
	jout<<"There are: "<<m_calibrations.size()<<" calibrations to load"<<endl;
	string table;
	CalibrationHandlerBase* calib;
	vector<vector < double> > m_rawcalib;
	for(m_calibrations_it= m_calibrations.begin(); m_calibrations_it!= m_calibrations.end(); m_calibrations_it++){

		table=(*m_calibrations_it).second;
		calib=(*m_calibrations_it).first;
		jout<<"Loading table: "<<table<<endl;
		m_rawcalib.clear();
		eventLoop->GetCalib(table, m_rawcalib);
		calib->fillCalib(m_rawcalib);
	}
	 */
	return NOERROR;
}

// evnt
jerror_t BDXEventProcessor::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	const eventData* tData;
	if (isMC){
		return NOERROR;
	}
	else{
		try{
			loop->GetSingle(tData);
		}
		catch(unsigned long e){
			jout<<"No eventData bank this event"<<endl;
			return 	OBJECT_NOT_AVAILABLE;
		}
		japp->RootWriteLock();

		eventT=tData->time;
		eventN=eventnumber;
		tword=tData->triggerWords[0];
		runN=tData->runN;
		t->Fill();


		/*Time*/
		if (eventT<startTime) startTime=eventT;
		if (eventT>stopTime)  stopTime=eventT;
		japp->RootUnLock();

		return NOERROR;
	}
}

// erun
jerror_t BDXEventProcessor::erun(void)
{
	deltaTime=stopTime-startTime;
	jout<<"BDXEventProcessor::erun "<<endl;
	jout<<"Run start: "<<startTime<<" stop: "<<stopTime<<" diff: "<<deltaTime<<endl;

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
		jerr<<"BDXEventProcessor::updateCalibration error, calibration handler associated with table "<<name<<" was not registered!"<<endl;
		return;
	}

	vector<CalibrationHandlerBase*> calibrations=m_calibrations[name];
	vector<CalibrationHandlerBase*>::iterator calibrations_it;

	/*Another check*/
	calibrations_it=find(calibrations.begin(),calibrations.end(),cal);
	if (calibrations_it==calibrations.end()){
		jerr<<"BDXEventProcessor::updateCalibration, key: "<<name<<" was found but not this specific calibrationHandler!"<<endl;
		return;
	}
	/*Verify if all the calibrators already have been set for this run*/
	bool flag=true;
	for (calibrations_it=calibrations.begin();calibrations_it!=calibrations.end();calibrations_it++){
		if ((*calibrations_it)->hasLoadedCurrentRun()==false) flag=false;
	}
	if (flag){ /*it means all the calibrations have been already loaded!*/
		return;
	}
	else{
		/*Get the data*/
		vector< vector < double > > m_data;
		eventLoop->GetCalib(name, m_data);
		cout<<"Going to fill the CalibrationHandlers for table: "<<name<<" there are: "<<calibrations.size()<<endl;
		for (calibrations_it=calibrations.begin();calibrations_it!=calibrations.end();calibrations_it++){
			(*calibrations_it)->fillCalib(m_data);
		}
	}
}

void BDXEventProcessor::clearCalibration(CalibrationHandlerBase* cal) {
	/*First, check the table name for cal*/
	string name=cal->getTable();
	/*Verify the key is in the map*/
	m_calibrations_it=m_calibrations.find(name);
	if (m_calibrations_it==m_calibrations.end()){
		jerr<<"BDXEventProcessor::clearCalibration error, calibration handler associated with table "<<name<<" was not registered!"<<endl;
		return;
	}

	vector<CalibrationHandlerBase*> calibrations=m_calibrations[name];
	vector<CalibrationHandlerBase*>::iterator calibrations_it;

	/*Another check*/
	calibrations_it=find(calibrations.begin(),calibrations.end(),cal);
	if (calibrations_it==calibrations.end()){
		jerr<<"BDXEventProcessor::clearCalibration, key: "<<name<<" was found but not this specific calibrationHandler!"<<endl;
		return;
	}
	for (calibrations_it=calibrations.begin();calibrations_it!=calibrations.end();calibrations_it++){
		(*calibrations_it)->setLoadedCurrentRun(false);
	}
}
