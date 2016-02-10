/*
 * IntVetoCalibration.hh
 *
 *  Created on: Feb 2, 2016
 *      Author: celentan
 */

#ifndef CALORIMETERCALIBRATION_HH_
#define CALORIMETERCALIBRATION_HH_

#include <TT/TranslationTable.h>
#include <vector>

#include <JANA/jerror.h>

class CalorimeterCalibration {
public:
	CalorimeterCalibration();
	virtual ~CalorimeterCalibration();

	jerror_t fillCalib(const std::vector<std::vector<double> > &calib_data);
	jerror_t getCalib(const TranslationTable::CALO_Index_t &index,vector<double> &data) const;

	std::map  < TranslationTable::CALO_Index_t, std::vector < double > > getCalibMap() const{return m_calib;}
	std::vector < double > operator[](const TranslationTable::CALO_Index_t &index) const;


private:
	std::map < TranslationTable::CALO_Index_t, std::vector < double > > m_calib;
	std::pair<std::map < TranslationTable::CALO_Index_t, std::vector < double > >::iterator,bool> m_insert_check;





};

#endif
