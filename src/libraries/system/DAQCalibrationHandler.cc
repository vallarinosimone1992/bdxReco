#include "DAQCalibrationHandler.h"


jerror_t DAQCalibrationHandler::fillCalib(const std::vector<std::vector<double> > &calib_data){
	TranslationTable::csc_t index;
	vector<double> data;
	int nData=0;
	int prevNdata=0;
	m_calib.clear();

	for (int irow=0;irow<calib_data.size();irow++){
		data.clear();
		if (irow>0) prevNdata=nData;
		nData=calib_data.at(irow).size()-3;   //crate, slot channel
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
		/*Fill the indexes*/
		index.rocid=calib_data.at(irow).at(0);
		index.slot=calib_data.at(irow).at(1);
		index.channel=calib_data.at(irow).at(2);

		/*Fill the data*/
		for (int idata=0;idata<nData;idata++){
			data.push_back(calib_data.at(irow).at(idata+3));
		}
		m_insert_check=m_calib.insert(std::make_pair(index,data));
		if (m_insert_check.second==false){
			jout<<"Element already exists in the map. Doing nothing"<<endl;
		}
	}
	return NOERROR;
}

std::vector <double> DAQCalibrationHandler::operator[](const TranslationTable::csc_t &index) const{
	vector < double > data;
	jerror_t ret;
	ret=this->getCalib(index,data);
	return data;
}

jerror_t DAQCalibrationHandler::getCalib(const TranslationTable::csc_t &index,vector<double> &data)const{
	std::map<TranslationTable::csc_t , vector < double > >::const_iterator it;
	it=m_calib.find(index);

	if (it==m_calib.end()){
		jerr<<"DAQCalibrationHandler:getCalib element not found"<<endl;
		return RESOURCE_UNAVAILABLE;
	}
	else{
		data=it->second;
		return NOERROR;
	}
}

jerror_t DAQCalibrationHandler::getCalibSingle(const TranslationTable::csc_t &index,double &data)const{
	std::map<TranslationTable::csc_t, vector < double > >::const_iterator it;
	it=m_calib.find(index);

	if (it==m_calib.end()){
		jerr<<"DAQCalibrationHandler:getCalibSingle element not found"<<endl;
		return RESOURCE_UNAVAILABLE;
	}
	else{
		if (it->second.size()!=1){
			jerr<<"DAQCalibrationHandler::getCalibSingle >1 cal constants: "<<it->second.size()<<endl;
			return RESOURCE_UNAVAILABLE;
		}
		else{
			data=it->second.at(0);
			return NOERROR;
		}
	}
}

