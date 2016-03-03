#ifndef _JEVENT_SOURCEEVIODAQ_H_
#define _JEVENT_SOURCEEVIODAQ_H_

// #include <vector>
// #include <string>
// #include <fstream>
// using namespace std;

// JANA headers
#include <JANA/JEventSource.h>
#include <JANA/jerror.h>

using namespace jana;

// bdx headers


// EVIO headers
#include <evioUtil.hxx>
#include <evioFileChannel.hxx>
//EVIO reader header
#include <evioReader/EvioCompositeDecoder.h>

using namespace evio;

// banks header
#include "banks.h"


class JEventSourceEvioDAQ:public JEventSource
{
	public:
		JEventSourceEvioDAQ(const char* source_name);
		virtual ~JEventSourceEvioDAQ();

		virtual const char* className(void)      {return static_className();}
		static const char* static_className(void){return "JEventSourceEvioDAQ";}

		jerror_t GetEvent(JEvent &event);
		void FreeEvent(JEvent &event);
		jerror_t GetObjects(JEvent &event, JFactory_base *factory);
	

		map<string, gBank> banksMap;
	
	private:
		evioFileChannel *chan;   // EVIO input channel
		evioDOMTree *EDT;        // single-event evio-DOM-tree
		int vme_mother_tag;    //tag of the bank containing banks with data from VME
		int child_mode1_tag;
		int child_mode7_tag;
		int child_trigger_tag;
		int eventHeader_tag;
		int eventHeader_CODA_tag;
		int prestart_tag;

		int curRunNumber,curEventNumber;
		int overwriteRunNumber;
};

#endif
