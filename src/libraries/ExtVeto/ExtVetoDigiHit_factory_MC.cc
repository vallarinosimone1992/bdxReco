// $Id$
//
//    File: ExtVetoDigiHit_factory_MC.cc
// Created: Tue Mar  8 12:19:00 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;


#include <MC/ExtVetoMCHit.h>
#include <MC/MCType.h>
#include <ExtVeto/ExtVetoDigiHit.h>


#include "ExtVetoDigiHit_factory_MC.h"
using namespace jana;

//------------------
// init
//------------------
jerror_t ExtVetoDigiHit_factory_MC::init(void)
{
	gPARMS->GetParameter("MC",m_isMC);
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t ExtVetoDigiHit_factory_MC::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t ExtVetoDigiHit_factory_MC::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	ExtVetoDigiHit *m_ExtVetoDigiHit=0;
	ExtVetoDigiHit::ExtVetoPMTDigiHit digi_hit;

	const ExtVetoMCHit *m_ExtVetoMCHit=0;

	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector <const ExtVetoMCHit*> m_ExtVetoMCHits;
	vector <const ExtVetoMCHit*>::const_iterator it;

	//1b: retrieve ExtVetoSiPMHit objects
	loop->Get(m_ExtVetoMCHits);

	m_map.clear();
	for (it=m_ExtVetoMCHits.begin();it!=m_ExtVetoMCHits.end();it++){
		m_ExtVetoMCHit = (*it);
		m_map_it=m_map.find(std::make_pair(m_ExtVetoMCHit->sector,m_ExtVetoMCHit->channel));
		if (m_map_it==m_map.end()){ //ExtVetoDigiHit was not created yet
			m_ExtVetoDigiHit=new ExtVetoDigiHit;
			m_ExtVetoDigiHit->AddAssociatedObject(m_ExtVetoMCHit);
			m_ExtVetoDigiHit->m_channel.sector=m_ExtVetoMCHit->sector;
			m_ExtVetoDigiHit->m_channel.layer=0;
			m_ExtVetoDigiHit->m_channel.readout=0;


			if (m_isMC==MCType::CATANIA_V1){
				m_ExtVetoDigiHit->m_channel.component=this->getCataniaV1Component(m_ExtVetoMCHit->channel);
				digi_hit.readout=1;
				digi_hit.Q=m_ExtVetoMCHit->adc1;
				digi_hit.T=m_ExtVetoMCHit->tdc1/1000.;  //MC is in ps
				m_ExtVetoDigiHit->m_data.push_back(digi_hit);
			}
			else if (m_isMC==MCType::CATANIA_V2){
				m_ExtVetoDigiHit->m_channel.component=this->getCataniaV2Component(m_ExtVetoMCHit->channel);
				/*create the component PMT-hits*/

				digi_hit.readout=1;
				digi_hit.Q=m_ExtVetoMCHit->adc1;
				digi_hit.T=m_ExtVetoMCHit->tdc1/1000.;  //MC is in ps
				m_ExtVetoDigiHit->m_data.push_back(digi_hit);
				if (m_ExtVetoMCHit->channel==1){
					digi_hit.readout=2;
					digi_hit.Q=m_ExtVetoMCHit->adc2;
					digi_hit.T=m_ExtVetoMCHit->tdc2/1000.;  //MC is in ps
					m_ExtVetoDigiHit->m_data.push_back(digi_hit);
				}
			}
			else if (m_isMC==MCType::FULL_V1){
				m_ExtVetoDigiHit->m_channel.component=this->getFullV1Component(m_ExtVetoMCHit->channel); /*Keep this as in MC. There's no data to compare with!*/
				digi_hit.readout=1;
				digi_hit.Q=m_ExtVetoMCHit->adc1;
				digi_hit.T=m_ExtVetoMCHit->tdc1/1000.;  //MC is in ps
				m_ExtVetoDigiHit->m_data.push_back(digi_hit);
			}


			m_map[std::make_pair(m_ExtVetoMCHit->sector,m_ExtVetoMCHit->channel)]=m_ExtVetoDigiHit;
		}
		else{ /*object already there*/
			m_ExtVetoDigiHit=m_map_it->second;
			m_ExtVetoDigiHit->m_data[0].Q+=m_ExtVetoMCHit->adc1;

			if ((m_isMC==MCType::CATANIA_V2)&&(m_ExtVetoMCHit->channel==1)){
				m_ExtVetoDigiHit->m_data[1].Q+=m_ExtVetoMCHit->adc2;
			}




			m_ExtVetoDigiHit->AddAssociatedObject(m_ExtVetoMCHit);
		}
	}

	for (m_map_it=m_map.begin();m_map_it!=m_map.end();m_map_it++){
		m_ExtVetoDigiHit=m_map_it->second;

		m_ExtVetoDigiHit->Q=0;
		for (int ii=0;ii<m_ExtVetoDigiHit->m_data.size();ii++){
			m_ExtVetoDigiHit->Q+=m_ExtVetoDigiHit->m_data[ii].Q;
		}



		_data.push_back(m_ExtVetoDigiHit);
	}



	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t ExtVetoDigiHit_factory_MC::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t ExtVetoDigiHit_factory_MC::fini(void)
{
	return NOERROR;
}

int ExtVetoDigiHit_factory_MC::getFullV1Component(int channel){
	int component;
	switch (channel){
	case(1)://top
																		component=0;
	break;
	case(2)://bottom
																		component=3;
	break;
	case(3)://right
																		component=2;
	break;
	case(4)://left
																		component=1;
	break;
	case(5)://downstream
																		component=5;
	break;
	case(6)://upstream
																		component=4;
	break;
	default:
		break;
	}
	return component;
}



int ExtVetoDigiHit_factory_MC::getCataniaV1Component(int channel){
	int component;
	switch (channel){
	case(1):
																		component=7;
	break;
	case(2):
																		component=6;
	break;
	case(3):
																		component=8;
	break;
	case(4):
																		component=9;
	break;
	case(5):
																		component=11;
	break;
	case(6):
																		component=10;
	break;
	case(7):
																		component=2;
	break;
	case(8):
																		component=1;
	break;
	case(9):
																		component=0;
	break;
	case(10):
																		component=3;
	break;
	case(11):
																		component=4;
	break;
	case(12):
																		component=5;
	break;
	default:
		break;
	}
	return component;
}

int ExtVetoDigiHit_factory_MC::getCataniaV2Component(int channel){
	int component;
	switch (channel){
	case(1):
					component=6;
	break;
	case(2):
			break; //there is NO channel 2 in simulation
	case(3):
					component=8;
	break;
	case(4):
					component=7;
	break;
	case(5):
					component=9;
	break;
	case(6):
					component=10;
	break;
	case(7):
					component=12;
	break;
	case(8):
					component=11;
	break;
	case(9):
					component=5;
	break;
	case(10):
					component=4;
	break;
	case(11):
					component=0;
	break;
	case(12):
					component=1;
	break;
	case(13):
					component=2;
	break;
	case(14):
					component=3;
	break;
	default:
		break;
	}
	return component;
}
