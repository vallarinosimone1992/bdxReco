// $Id$
//
//    File: CalibrationHandler.h
// Created: Tue Feb 23 17:49:00 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#ifndef _DAQCalibrationHandler_
#define _DAQCalibrationHandler_

#include <JANA/JObject.h>
#include <JANA/jerror.h>
#include <TT/TranslationTable.h>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

class DAQCalibrationHandler{
public:
	jerror_t fillCalib(const std::vector<std::vector<double> > &calib_data);
	jerror_t getCalib(const TranslationTable::csc_t &index,vector<double> &data) const;
	jerror_t getCalibSingle(const TranslationTable::csc_t &index,double &data) const;



	std::map  <TranslationTable::csc_t, std::vector < double > > getCalibMap() const{return m_calib;}
	std::vector < double > operator[](const TranslationTable::csc_t &index) const;

	//	virtual ~CalibrationHandler();
private:

	std::map <TranslationTable::csc_t, std::vector < double > > m_calib;
	std::pair<std::map <TranslationTable::csc_t, std::vector < double > >::iterator,bool> m_insert_check;


};

#endif // _DAQCalibrationHandler_

