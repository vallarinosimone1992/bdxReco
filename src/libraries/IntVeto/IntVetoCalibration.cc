/*
 * IntVetoCalibration.cc
 *
 *  Created on: Feb 2, 2016
 *      Author: celentan
 */

#include "IntVetoCalibration.h"

using namespace std;
IntVetoCalibration::IntVetoCalibration() {
	// TODO Auto-generated constructor stub

}

IntVetoCalibration::~IntVetoCalibration() {
	// TODO Auto-generated destructor stub
}

/*From the calibration source (db - file) we get something like
sector layer component readout x x x x x

Where there can be any number of "x" (while the fact we have 4 identifiers is related to TT!
 */
jerror_t IntVetoCalibration::fillCalib(const vector<vector<double> > &calib_data){
	TranslationTable::INT_VETO_Index_t index;
	vector<double> data;
	int nData=0;
	int prevNdata=0;
	m_calib.clear();

	for (int irow=0;irow<calib_data.size();irow++){
		if (irow>0) prevNdata=nData;
		nData=calib_data.at(irow).size()-TranslationTable::INT_VETO_Index_t::nIDs();
		if (nData<=0) {
			jerr<<"Error in IntvetoCalibration::fillCalib. No data?"<<endl;
			m_calib.clear();
			return 	VALUE_OUT_OF_RANGE;
		}
		if ((nData!=prevNdata)&&(irow!=0)){
			jerr<<"Error in IntVetoCalibration::fillCalib. Error on number of datas?"<<endl;
			m_calib.clear();
			return 	VALUE_OUT_OF_RANGE;
		}
		index.sector=calib_data.at(irow).at(0);
		index.layer=calib_data.at(irow).at(1);
		index.component=calib_data.at(irow).at(2);
		index.readout=calib_data.at(irow).at(3);
		for (int idata=0;idata<nData;idata++){
			data.push_back(calib_data.at(irow).at(4+idata));
		}
		m_insert_check=m_calib.insert(std::make_pair(index,data));
		if (m_insert_check.second==false){
			jout<<"Element already exists in the map. Doing nothing"<<endl;
		}
	}
	return NOERROR;
}


jerror_t IntVetoCalibration::getCalib(const TranslationTable::INT_VETO_Index_t &index,vector<double> &calib) const{
	std::map< TranslationTable::INT_VETO_Index_t, vector < double > >::const_iterator it;
	it=m_calib.find(index);

	if (it==m_calib.end()){
		jerr<<"IntVetoCalibration:getCalib element not found"<<endl;
		return RESOURCE_UNAVAILABLE;
	}
	else{
		calib=it->second;
		return NOERROR;
	}
}


vector < double > IntVetoCalibration::operator[](const TranslationTable::INT_VETO_Index_t &index) const{
	vector < double > data;
	jerror_t ret;
	ret=this->getCalib(index,data);
	return data;
}
