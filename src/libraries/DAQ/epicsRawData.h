#ifndef _epicsRawData_
#define _epicsRawData_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>

#include "TString.h"
//#include <vector>

/*This is basically a light-weight container for EPICS data from EVIO*/
class epicsRawData:public jana::JObject{
	public:
		JOBJECT_PUBLIC(epicsRawData);


		string rawData;

		// This method is used primarily for pretty printing
		// the second argument to AddString is printf style format
		void toStrings(vector<pair<string,string> > &items)const{


		}

};

#endif

