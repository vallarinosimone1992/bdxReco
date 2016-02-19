
#ifndef EXTVETOCALIBRATION_HH_
#define EXTVETOCALIBRATION_HH_

#include <TT/TranslationTable.h>
#include <vector>

#include <JANA/jerror.h>

class ExtVetoCalibration {
public:
	ExtVetoCalibration();
	virtual ~ExtVetoCalibration();

	jerror_t fillCalib(const std::vector<std::vector<double> > &calib_data);
	jerror_t getCalib(const TranslationTable::EXT_VETO_Index_t &index,vector<double> &data) const;

	std::map  < TranslationTable::EXT_VETO_Index_t, std::vector < double > > getCalibMap() const{return m_calib;}
	std::vector < double > operator[](const TranslationTable::EXT_VETO_Index_t &index) const;


private:
	std::map < TranslationTable::EXT_VETO_Index_t, std::vector < double > > m_calib;
	std::pair<std::map < TranslationTable::EXT_VETO_Index_t, std::vector < double > >::iterator,bool> m_insert_check;


};

#endif
