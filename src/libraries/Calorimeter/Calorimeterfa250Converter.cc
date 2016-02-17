#include "Calorimeterfa250Converter.h"
#include "CalorimeterCalibration.h"

#include "TGraphErrors.h"
#include "TF1.h"

#include <DAQ/fa250Mode1CalibHit.h>
#include <DAQ/fa250Mode7Hit.h>


double Calorimeterfa250Converter::fSinglePhe(double *x,double *par){
	double ret=0;
	double t=x[0]-par[0];
	if (t<0) return 0;
	else{
		ret=par[1]/(2.*par[2]*par[2]*par[2])*t*t*exp(-t/par[2]); // the overall integral is par[1]. Max val is 2*par[1]*par[2]*exp(-2)
		ret+=par[3]; //pedestal - eventually still there
		return ret;
	}
}



CalorimeterSiPMHit* Calorimeterfa250Converter::convertHit(const fa250Hit *hit,const TranslationTable::ChannelInfo &m_channel) const{
	CalorimeterSiPMHit *m_CalorimeterSiPMHit=new CalorimeterSiPMHit;
	m_CalorimeterSiPMHit->m_channel=m_channel;

	if (strcmp(hit->className(),"fa250Mode1CalibHit")==0){
		this->convertMode1Hit(m_CalorimeterSiPMHit,(const fa250Mode1CalibHit*)hit);
	}
	else if (strcmp(hit->className(),"fa250Mode7Hit")==0){
		this->convertMode7Hit(m_CalorimeterSiPMHit,(const fa250Mode7Hit*)hit);
	}
	else{
		jerr<<"Calorimeterfa250Converter::convertHit unsupported class name: "<<hit->className()<<std::endl;
		return 0;
	}
	return m_CalorimeterSiPMHit;
}

jerror_t Calorimeterfa250Converter::convertMode1Hit(CalorimeterSiPMHit* output,const fa250Mode1CalibHit *input) const{
	int size=input->samples.size();

	output->Q=0;
	output->T=0;
	output->A=0;
	/*This part is to read pedestal from dB*/
	vector<double> pedV;
	vector<double> corrected_samples;



	std::vector<std::pair<int,int> > m_crossingTimes;
	std::vector<int> m_crossingTimesDelta;
	std::pair<int,int> m_thisCrossingTime;

	int nSingles=0;
	int nSignals=0;


	pedestal->getCalib(output->m_channel.calorimeter,pedV);
	if (pedV.size()==1){
		output->ped=pedV.at(0);
	}
	else{
		jerr<<"Calorimeterfa250Converter::convertMode1Hit error on pedestal db entries. Got: "<<pedV.size()<<std::endl;
		output->ped=0;  //A.C. for now ok, in the future need to revert-back to the non-db case!*/
	}

	//1: get the pedestal-corrected samples
	for (int ii=0;ii<size;ii++){
		corrected_samples.push_back(input->samples.at(ii)-output->ped);
	}


	///TODO!
	int thr=5;

	//2: find thr crossings
	if (corrected_samples[0]>thr) m_thisCrossingTime.first=0;
	for (int ii=1;ii<size;ii++){
		if ((corrected_samples[ii]>thr)&&(corrected_samples[ii-1]<thr)) m_thisCrossingTime.first=ii;
		else if ((corrected_samples[ii]<thr)&&(corrected_samples[ii-1]>thr)) {
			m_thisCrossingTime.second=ii;
			m_crossingTimes.push_back(m_thisCrossingTime);
		}
	}
	//It may happen that the last sample is still over thr!
	if (corrected_samples[size-1]>thr){
		m_thisCrossingTime.second=size-1;
		m_crossingTimes.push_back(m_thisCrossingTime);
	}


	//3: deltas
	//3a: if no crossing times are found
	if (m_crossingTimes.size()==0){
		output->m_type=noise;
		output->T=0;
		output->Q=this->sumSamples(corrected_samples);
	}

	/*Compute ToT */
	for (int itime=0;itime<m_crossingTimes.size();itime++){
		m_crossingTimesDelta.push_back(m_crossingTimes.at(itime).second-m_crossingTimes.at(itime).first);
	}

	/*Verify the ToT for each pulse*/
	for (int itime=0;itime<m_crossingTimes.size();itime++){
			if (m_crossingTimesDelta.at(itime)<0){
				jerr<<"Calorimeterfa20Converter::convertMode1Hit error, negative ToT?"<<std::endl;
			}
			else if (m_crossingTimesDelta.at(itime)<SINGLE_SIGNAL_TOT){
				nSingles++;
			}
			else{
				nSignals++;
			}
	}

	/*If there is one pulse only: this is a "single phe" signal*/
	if ((nSignals==0)&&(nSignals==1)){
		output->m_type=one_phe;
		/*So fit it*/
		double tau=3;
		double max=10;
		double I=max/(2*tau*exp(-2));
		output->fit_function.fSinglePhe=new TF1("fSinglePhe",const_cast<Calorimeterfa250Converter*>(this),&Calorimeterfa250Converter::fSinglePhe,0,size,4);
		output->fit_function.fSinglePhe->SetParameters(m_crossingTimes.at(0).first,I,tau,0);

		/*For now, very terrible*/
		TGraphErrors *gTmp=new TGraphErrors(size,const_cast<double*>(this->m_fitIndex),&(corrected_samples.at(0)),0,const_cast<double*>(this->m_fitError));
		gTmp->Fit(output->fit_function.fSinglePhe,"","",m_crossingTimes.at(0).first-2,m_crossingTimes.at(0).second+2);

		output->Q=output->fit_function.fSinglePhe->GetParameter(1);
	}


	return NOERROR;
}

jerror_t Calorimeterfa250Converter::convertMode7Hit(CalorimeterSiPMHit* output,const fa250Mode7Hit *input) const{


	return NOERROR;
}




