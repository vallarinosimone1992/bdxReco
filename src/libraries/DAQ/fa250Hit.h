/*
 * fa250Hit.h
 *
 *  Created on: Jan 25, 2016
 *      Author: celentan
 */

#ifndef FA250HIT_H_
#define FA250HIT_H_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include <TT/TranslationTable.h>
#include <system/BDXObject.h>
class fa250Hit : public BDXObject{
public:


	TranslationTable::csc_t m_channel;
	int trigger;
	uint64_t timestamp;

	ClassDef(fa250Hit,1)
};

#endif /* FA250HIT_H_ */
