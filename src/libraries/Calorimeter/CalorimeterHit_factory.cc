// $Id$
//
//    File: CalorimeterHit_factory.cc
// Created: Wed Jan 27 16:40:00 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "CalorimeterHit_factory.h"

//objects we need from the framework
#include <TT/TranslationTable.h>
#include <Calorimeter/CalorimeterDigiHit.h>
//objects we put in the framework
#include <Calorimeter/CalorimeterHit.h>


using namespace jana;

CalorimeterHit_factory::CalorimeterHit_factory():m_ene(0),m_tt(0){
	VERBOSE=0;
	isMC=0;
	m_THR_singleReadout=5;
	m_THR_multipleReadout=3;
	m_N_multipleReadout=2;

	gPARMS->SetDefaultParameter("CALORIMETER:HIT_THR_SINGLE",m_THR_singleReadout,"Threshold in phe (charge) for a detector with single readout");
	gPARMS->SetDefaultParameter("CALORIMETER:HIT_THR_MULTI",m_THR_multipleReadout,"Threshold in phe (charge) for a detector with multi readout");
	gPARMS->SetDefaultParameter("CALORIMETER:HIT_N_MULTI",m_N_multipleReadout,"Multiplicity for a detector with multi readout");
	gPARMS->GetParameter("CALORIMETER:VERBOSE",VERBOSE);

}

//------------------
// init
//------------------
jerror_t CalorimeterHit_factory::init(void)
{

	m_ene=new CalibrationHandler<TranslationTable::CALO_Index_t>("/Calorimeter/Ene");
	this->mapCalibrationHandler(m_ene);

	gPARMS->GetParameter("MC", isMC);
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t CalorimeterHit_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{


	this->updateCalibrationHandler(m_ene,eventLoop);



	if (VERBOSE>3){
		std::map  < TranslationTable::CALO_Index_t, std::vector < double > > gainCalibMap;
		std::map  < TranslationTable::CALO_Index_t, std::vector < double > >::iterator gainCalibMap_it;
		gainCalibMap=m_ene->getCalibMap();
		jout<<"Got following ene for run number: "<<runnumber<<endl;
		jout<<"Rows: "<<gainCalibMap.size()<<endl;
		for (gainCalibMap_it=gainCalibMap.begin();gainCalibMap_it!=gainCalibMap.end();gainCalibMap_it++){
			jout<<gainCalibMap_it->first.sector<<" "<<gainCalibMap_it->first.x<<" "<<gainCalibMap_it->first.y<<" "<<gainCalibMap_it->first.readout<<" "<<gainCalibMap_it->second[0]<<" "<<gainCalibMap_it->second[1]<<endl;
		}
	}





	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t CalorimeterHit_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{


	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector <const CalorimeterDigiHit*> m_CalorimeterDigiHits;
	vector <const CalorimeterDigiHit*>::const_iterator it;

	const CalorimeterDigiHit *m_CalorimeterDigiHit;
	CalorimeterHit *m_CalorimeterHit=0;

	TranslationTable::CALO_Index_t m_index;

	int readout;
	double Q,T,Qtot,Qmax,Tmax;
	int flagOk;

	double gain,ped;

	//1b: retrieve CalorimeterSiPMHit objects

	/*This is very important!! Select - or not - the MC case*/
	if (isMC){
		loop->Get(m_CalorimeterDigiHits,"MC");
	}
	else{
		loop->Get(m_CalorimeterDigiHits);
	}

	for (it=m_CalorimeterDigiHits.begin();it!=m_CalorimeterDigiHits.end();it++){
		m_CalorimeterDigiHit=(*it);

		flagOk=0;
		Qtot=0;
		Qmax=-9999;
		if ((*it)->m_data.size()==0){
			continue;
		}
		else if ((*it)->m_data.size()==1){   /*Single readout object*/
			Q=m_CalorimeterDigiHit->m_data[0].Q;
			T=m_CalorimeterDigiHit->m_data[0].T;

			if (Q	> m_THR_singleReadout){
				m_CalorimeterHit=new CalorimeterHit();
				m_CalorimeterHit->E=0;
				m_CalorimeterHit->m_channel=m_CalorimeterDigiHit->m_channel;
				m_CalorimeterHit->AddAssociatedObject(m_CalorimeterDigiHit);
				m_CalorimeterHit->Q=Q;
				m_CalorimeterHit->T=T;
				CalorimeterHit::CalorimeterComponentHit hit;
				hit.readout=m_CalorimeterDigiHit->m_data[0].readout;
				hit.Q=Q;
				hit.T=T;
				/*Try to calibrate in energy and ped-sub*/
				m_index=m_CalorimeterHit->m_channel;
				m_index.readout=m_CalorimeterDigiHit->m_data[0].readout;
				gain=m_ene->getCalib(m_index)[0];
				ped=m_ene->getCalib(m_index)[1];
				hit.E=(Q-ped);
				if (gain!=0){
					hit.E/=gain;
				}
				m_CalorimeterHit->E=hit.E;
				m_CalorimeterHit->m_data.push_back(hit);
				m_CalorimeterHit->N=1;
				_data.push_back(m_CalorimeterHit);
			}
		}
		else if ((*it)->m_data.size()>=2){   /*Multiple readout object*/

			for (int idigi=0;idigi<m_CalorimeterDigiHit->m_data.size();idigi++){
				Q=m_CalorimeterDigiHit->m_data[idigi].Q;
				T=m_CalorimeterDigiHit->m_data[idigi].T;

				if (Q>m_THR_multipleReadout){
					flagOk++;
					Qtot+=Q;
				}
				if (Q>Qmax){
					Qmax=Q;
					Tmax=T;
				}
			}

			/*At the end of this loop, flagOK is the number of counters above thr*/
			if (flagOk>=m_N_multipleReadout){
				m_CalorimeterHit=new CalorimeterHit();
				m_CalorimeterHit->E=0;
				m_CalorimeterHit->m_channel=m_CalorimeterDigiHit->m_channel;
				m_CalorimeterHit->AddAssociatedObject(m_CalorimeterDigiHit);
				m_CalorimeterHit->Q=Qtot;
				m_CalorimeterHit->T=Tmax;
				m_CalorimeterHit->N=flagOk;

				/*Loop again*/
				for (int idigi=0;idigi<m_CalorimeterDigiHit->m_data.size();idigi++){
					Q=m_CalorimeterDigiHit->m_data[idigi].Q;
					T=m_CalorimeterDigiHit->m_data[idigi].T;
					if (Q>m_THR_multipleReadout){						/*Clearly this is now enough as condition!*/
						CalorimeterHit::CalorimeterComponentHit hit;
						hit.readout=m_CalorimeterDigiHit->m_data[idigi].readout;
						hit.Q=Q;
						hit.T=T;
						hit.good_ped_RMS=m_CalorimeterDigiHit->m_data[idigi].good_ped_RMS;
						hit.type=m_CalorimeterDigiHit->m_data[idigi].type;
						hit.E=0;

						/*Try to calibrate in energy and ped-sub*/
						m_index=m_CalorimeterHit->m_channel;
						m_index.readout=m_CalorimeterDigiHit->m_data[idigi].readout;
						gain=m_ene->getCalib(m_index)[0];
						ped=m_ene->getCalib(m_index)[1];
						hit.E=(Q-ped);
						if (gain!=0){
							hit.E/=gain;
						}

						m_CalorimeterHit->E+=hit.E;
						m_CalorimeterHit->m_data.push_back(hit);
					}
				}
				m_CalorimeterHit->E/=flagOk;

				_data.push_back(m_CalorimeterHit);

			}

		}
		else {
			jerr<<"CalorimeterHit_factory::evnt error, a CalorimeterDigiHit has more than 2 entries in the data vector??"<<endl;
		}

	}

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t CalorimeterHit_factory::erun(void)
{

	this->clearCalibrationHandler(m_ene);

	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t CalorimeterHit_factory::fini(void)
{
	return NOERROR;
}

