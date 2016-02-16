/*
 * PaddlesCalibration.cc
 *
 *  Created on: Feb 2, 2016
 *      Author: celentan
 */

#include <Paddles/PaddlesCalibration.h>

using namespace std;
PaddlesCalibration::PaddlesCalibration() {
	// TODO Auto-generated constructor stub

}

PaddlesCalibration::~PaddlesCalibration() {
	// TODO Auto-generated destructor stub
}

/*From the calibration source (db - file) we get something like
sector id readout x  		// x= gain value

Where there can be any number of "x" (while the fact we have 3 identifiers is related to TT!
 */
jerror_t PaddlesCalibration::fillCalib(const vector<vector<double> > &calib_data){
	TranslationTable::PADDLES_Index_t index;
	vector<double> data;
	int nData=0;
	int prevNdata=0;
	m_calib.clear();
//			jout<<"CALIB DATA SIZE "<<calib_data.size()<<endl;
	for (int irow=0;irow<calib_data.size();irow++){
		data.clear();
		if (irow>0) prevNdata=nData;
		nData=calib_data.at(irow).size()-TranslationTable::PADDLES_Index_t::nIDs();
//		jout<<calib_data.at(irow).at(0)<<" "<<calib_data.at(irow).at(1)<<" "<<calib_data.at(irow).at(2)<<" "<<calib_data.at(irow).at(3)<<endl;
		if (nData<=0) {
			jerr<<"Error in PaddlesCalibration::fillCalib. No data?"<<endl;
			m_calib.clear();
			return 	VALUE_OUT_OF_RANGE;
		}
		if ((nData!=prevNdata)&&(irow!=0)){
			jerr<<"Error in PaddlesCalibration::fillCalib. Error on number of datas?"<<endl;
			m_calib.clear();
			return 	VALUE_OUT_OF_RANGE;
		}
		index.sector=calib_data.at(irow).at(0);
		index.id=calib_data.at(irow).at(1);
		index.readout=calib_data.at(irow).at(2);
		for (int idata=0;idata<nData;idata++){
			data.push_back(calib_data.at(irow).at(3+idata));
//			jout<<"PUSH BACK "<<calib_data.at(irow).at(3+idata)<<endl;
		}
		m_insert_check=m_calib.insert(std::make_pair(index,data));
		if (m_insert_check.second==false){
			jout<<"Element already exists in the map. Doing nothing"<<endl;
		}
	}
	return NOERROR;
}


jerror_t PaddlesCalibration::getCalib(const TranslationTable::PADDLES_Index_t &index,vector<double> &calib) const{
	std::map< TranslationTable::PADDLES_Index_t, vector < double > >::const_iterator it;
	it=m_calib.find(index);

	if (it==m_calib.end()){
		jerr<<"PaddlesCalibration:getCalib element not found"<<endl;
		return RESOURCE_UNAVAILABLE;
	}
	else{
		calib=it->second;
		return NOERROR;
	}
}


vector < double > PaddlesCalibration::operator[](const TranslationTable::PADDLES_Index_t &index) const{
	vector < double > data;
	jerror_t ret;
	ret=this->getCalib(index,data);
	return data;
}
