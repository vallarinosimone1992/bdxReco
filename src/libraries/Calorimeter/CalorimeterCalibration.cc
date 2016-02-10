/*
 * CalorimeterCalibration.cc
 *
 *  Created on: Feb 2, 2016
 *      Author: celentan
 */

#include "CalorimeterCalibration.h"

using namespace std;
CalorimeterCalibration::CalorimeterCalibration() {
	// TODO Auto-generated constructor stub

}

CalorimeterCalibration::~CalorimeterCalibration() {
	// TODO Auto-generated destructor stub
}

/*From the calibration source (db - file) we get something like
sector x y readout x x x x x

Where there can be any number of "x" (while the fact we have 4 identifiers is related to TT!
 */
jerror_t CalorimeterCalibration::fillCalib(const vector<vector<double> > &calib_data){
	TranslationTable::CALO_Index_t index;
	vector<double> data;
	int nData=0;
	int prevNdata=0;
	m_calib.clear();

	for (int irow=0;irow<calib_data.size();irow++){
		data.clear();
		if (irow>0) prevNdata=nData;
		nData=calib_data.at(irow).size()-TranslationTable::CALO_Index_t::nIDs();
		if (nData<=0) {
			jerr<<"Error in IntvetoCalibration::fillCalib. No data?"<<endl;
			m_calib.clear();
			return 	VALUE_OUT_OF_RANGE;
		}
		if ((nData!=prevNdata)&&(irow!=0)){
			jerr<<"Error in CalorimeterCalibration::fillCalib. Error on number of datas?"<<endl;
			m_calib.clear();
			return 	VALUE_OUT_OF_RANGE;
		}
		index.sector=calib_data.at(irow).at(0);
		index.x=calib_data.at(irow).at(1);
		index.y=calib_data.at(irow).at(2);
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


jerror_t CalorimeterCalibration::getCalib(const TranslationTable::CALO_Index_t &index,vector<double> &calib) const{
	std::map< TranslationTable::CALO_Index_t, vector < double > >::const_iterator it;
	it=m_calib.find(index);

	if (it==m_calib.end()){
		jerr<<"CalorimeterCalibration:getCalib element not found sector= "<<index.sector<<" x="<<index.x<<" y="<<index.y<<" readout= "<<index.readout<<endl;
		return RESOURCE_UNAVAILABLE;
	}
	else{
		calib=it->second;
		return NOERROR;
	}
}


vector < double > CalorimeterCalibration::operator[](const TranslationTable::CALO_Index_t &index) const{
	vector < double > data;
	jerror_t ret;
	ret=this->getCalib(index,data);
	return data;
}
