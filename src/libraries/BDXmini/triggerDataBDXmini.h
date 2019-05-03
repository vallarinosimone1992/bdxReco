// $Id$
//
//    File: triggerData.h
// Created: Fri May  3 11:44:17 CEST 2019
// Creator: celentan (on Linux apcx4 2.6.32-696.13.2.el6.x86_64 x86_64)
//

#ifndef _triggerData_
#define _triggerData_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>

class triggerDataBDXmini:public jana::JObject{
	public:
		JOBJECT_PUBLIC(triggerDataBDXmini);
		
		// Add data members here. For example:
		// int id;
		// double E;
		
		// This method is used primarily for pretty printing
		// the second argument to AddString is printf style format
		void toStrings(vector<pair<string,string> > &items)const{
			// AddString(items, "id", "%4d", id);
			// AddString(items, "E", "%f", E);
		}
		

		triggerDataBDXmini();

		vector<unsigned int> rawWords;

		static const int nTriggersMAX = 32;
		static const int nChansMAX = 64;

		unsigned int nTriggers;
		vector<int> trgTimes[nTriggersMAX];

		unsigned int getNTriggers() const;
		void setNTriggers(unsigned int n);
		unsigned int getNtriggers_single(int itrg) const;


		void addTriggerTime_single(int itrg,int data);
		int getTriggerTime_single(int itrg,int n) const;


		bool channels[nChansMAX];
		bool channelsTRG[nChansMAX];

		bool hasChannel(int ich) const;
		bool hasChannelTRG(int ich) const;

		void setChannel(int ich);
		void setChannelTRG(int ich);
		void unsetChannel(int ich);
		void unsetChannelTRG(int ich);
};

#endif // _triggerData_

