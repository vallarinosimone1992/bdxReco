// $Id$
//
//    File: CalibrationHandler.h
// Created: Tue Feb 23 17:49:00 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#ifndef _CalibrationHandler_
#define _CalibrationHandler_

#include <JANA/JObject.h>
#include <JANA/jerror.h>
#include <TT/TranslationTable.h>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

template <class T> class CalibrationHandler{  //T is supposed to be an index in the TT library
public:
	jerror_t fillCalib(const std::vector<std::vector<double> > &calib_data);
	jerror_t getCalib(const T &index,vector<double> &data) const;
	jerror_t getCalibSingle(const T &index,double &data) const;



	std::map  <T, std::vector < double > > getCalibMap() const{return m_calib;}
	std::vector < double > operator[](const T &index) const;

	//	virtual ~CalibrationHandler();
private:

	std::map <T, std::vector < double > > m_calib;
	std::pair<typename std::map <T, std::vector < double > >::iterator,bool> m_insert_check;


};

template <class T> jerror_t CalibrationHandler<T>::fillCalib(const std::vector<std::vector<double> > &calib_data){
	T index;
	vector<double> data;
	int nData=0;
	int prevNdata=0;
	m_calib.clear();

	for (int irow=0;irow<calib_data.size();irow++){
		data.clear();
		if (irow>0) prevNdata=nData;
		nData=calib_data.at(irow).size()-index.nIDs();
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
		for (int iindex=0;iindex<index.nIDs();iindex++){
			index.ID(iindex)=calib_data.at(irow).at(iindex);
		}
		/*Fill the data*/
		for (int idata=0;idata<nData;idata++){
			data.push_back(calib_data.at(irow).at(index.nIDs()+idata));
		}
		m_insert_check=m_calib.insert(std::make_pair(index,data));
		if (m_insert_check.second==false){
			jout<<"Element already exists in the map. Doing nothing"<<endl;
		}
	}
	return NOERROR;
}

template <class T> std::vector <double> CalibrationHandler<T>::operator[](const T &index) const{
	vector < double > data;
	jerror_t ret;
	ret=this->getCalib(index,data);
	return data;
}

template <class T> jerror_t CalibrationHandler<T>::getCalib(const T &index,vector<double> &data)const{
	typename std::map< T, vector < double > >::const_iterator it;
	it=m_calib.find(index);

	if (it==m_calib.end()){
		jerr<<"CalorimeterCalibration:getCalib element not found"<<endl;
		return RESOURCE_UNAVAILABLE;
	}
	else{
		data=it->second;
		return NOERROR;
	}
}

template <class T> jerror_t CalibrationHandler<T>::getCalibSingle(const T &index,double &data)const{
	typename std::map<T, vector < double > >::const_iterator it;
	it=m_calib.find(index);

	if (it==m_calib.end()){
		jerr<<"CalibrationHandler:getCalibSingle element not found"<<endl;
		return RESOURCE_UNAVAILABLE;
	}
	else{
		if (it->second.size()!=1){
			jerr<<"CalibrationHandler::getCalibSingle >1 cal constants: "<<it->second.size()<<endl;
			return RESOURCE_UNAVAILABLE;
		}
		else{
			data=it->second.at(0);
			return NOERROR;
		}
	}
}


#endif // _CalibrationHandler_

