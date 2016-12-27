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
	CataniaProto1MC=10,
	CataniaProto2MC=20,
	FullMC=30
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

private:
	int m_eventType;
	int m_runNumber;
	int m_eventNumber;
	int m_eventTime; //Unix Timestamp
	std::vector<uint32_t> m_triggerWords;



#if (!defined(__APPLE__))
	ClassDef(TEventHeader,1);
#endif
};

#endif /* TEVENTHEADER_H_ */
