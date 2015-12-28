#ifndef _JEVENT_SOURCEEVIO_H_
#define _JEVENT_SOURCEEVIO_H_

// #include <vector>
// #include <string>
// #include <fstream>
// using namespace std;

// JANA headers
#include <JANA/JEventSource.h>
#include <JANA/jerror.h>
using namespace jana;

// bdx headers
#include "options.h"


// EVIO headers
#include "evioUtil.hxx"
#include "evioFileChannel.hxx"
#include "EvioCompositeDecoder.h"

using namespace evio;

// banks header
#include "banks.h"


class JEventSourceEvio:public JEventSource
{
	public:
		JEventSourceEvio(const char* source_name, goptions Opt);
		virtual ~JEventSourceEvio();

		virtual const char* className(void)      {return static_className();}
		static const char* static_className(void){return "JEventSourceEvio";}

		jerror_t GetEvent(JEvent &event);
		void FreeEvent(JEvent &event);
		jerror_t GetObjects(JEvent &event, JFactory_base *factory);
	
		goptions bdxOpt;
		map<string, gBank> banksMap;
	
	private:
		evioFileChannel *chan;   // EVIO input channel
		evioDOMTree *EDT;        // single-event evio-DOM-tree
		int mother_tag;
		int child_mode1_tag;
		int child_mode7_tag;
};

#endif
