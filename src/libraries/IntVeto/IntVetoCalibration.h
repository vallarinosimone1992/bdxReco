/*
 * IntVetoCalibration.hh
 *
 *  Created on: Feb 2, 2016
 *      Author: celentan
 */

#ifndef INTVETOCALIBRATION_HH_
#define INTVETOCALIBRATION_HH_

#include <TT/TranslationTable.h>
#include <vector>

#include "jerror.h"
class IntVetoCalibration {
public:
	IntVetoCalibration();
	virtual ~IntVetoCalibration();

	jerror_t fillCalib(const std::vector<std::vector<double> > &calib_data);
	jerror_t getCalib(const TranslationTable::INT_VETO_Index_t &index,vector<double> &data) const;

	std::vector < double > operator[](const TranslationTable::INT_VETO_Index_t &index) const;


private:
	std::map < TranslationTable::INT_VETO_Index_t, std::vector < double > > m_calib;
	std::pair<std::map < TranslationTable::INT_VETO_Index_t, std::vector < double > >::iterator,bool> m_insert_check;





};

#endif /* INTVETOCALIBRATION_HH_ */
