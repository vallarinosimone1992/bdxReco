/*
 * TEventHeader.h
 *
 *  Created on: Dec 23, 2016
 *      Author: celentan
 */

#ifndef TEVENTHEADER_H_
#define TEVENTHEADER_H_

#include "TObject.h"

typedef enum{
	CataniaProto1Event=1,
	CataniaProto2Event=2,
	JLabFluxEvent=4,
	CataniaProto1MC=10,
	CataniaProto2MC=20,
	FullMC=30,
	JLabFluxEventMC=40,
}eventType;

class TEventHeader : public TObject{
public:
	TEventHeader();
	virtual ~TEventHeader();

	int getEventNumber() const {
		return m_eventNumber;
	}

	void setEventNumber(int eventNumber) {
		m_eventNumber = eventNumber;
	}

	int getRunNumber() const {
		return m_runNumber;
	}

	void setRunNumber(int runNumber) {
		m_runNumber = runNumber;
	}

	std::vector<uint32_t> getTriggerWords() const{
		return m_triggerWords;
	}

	void setTriggerWords(std::vector<uint32_t> triggerWords){
		m_triggerWords=triggerWords;
	}

	int getEventTime() const {
		return m_eventTime;
	}

	void setEventTime(int eventTime) {
		m_eventTime = eventTime;
	}

	int getEventType() const {
		return m_eventType;
	}

	void setEventType(int eventType) {
		m_eventType = eventType;
	}

	uint32_t getEventFineTime() const {
		return m_eventFineTime;
	}

	void setEventFineTime(uint32_t eventFineTime) {
		m_eventFineTime = eventFineTime;
	}

private:
	uint8_t m_eventType;
	int m_runNumber;
	int m_eventNumber;
	int m_eventTime; //Unix Timestamp

	uint32_t m_eventFineTime; //this is the timestamp as reported by FADC boards, in particular I save here time from fadc in slot4, reading the calorimeter (there should be always a calo hit for events of interest). Units are: 4ns*/



	std::vector<uint32_t> m_triggerWords;




	ClassDef(TEventHeader,1);

};

#endif /* TEVENTHEADER_H_ */
