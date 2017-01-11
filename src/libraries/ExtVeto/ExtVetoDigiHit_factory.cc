// $Id$
//
//    File: ExtVetoDigiHit_factory.cc
// Created: Wed Jan 20 18:22:52 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
using namespace std;

#include "ExtVetoDigiHit_factory.h"
#include <ExtVeto/ExtVetoPMTHit.h>

#include <TT/TranslationTable.h>

using namespace jana;

//------------------
// init
//------------------
jerror_t ExtVetoDigiHit_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t ExtVetoDigiHit_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	jout<<"ExtVetoDigiHit_factory::brun new run number: "<<runnumber<<endl;
	m_tt=0;
	eventLoop->GetSingle(m_tt);
	if (m_tt==0){
		jerr<<" unable to get the TranslationTable from this run!"<<endl;
		return OBJECT_NOT_AVAILABLE;
	}
	return NOERROR;

}

//------------------
// evnt
//------------------
jerror_t ExtVetoDigiHit_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{


	TranslationTable::EXT_VETO_Index_t m_channel;
	ExtVetoDigiHit *m_ExtVetoDigiHit=0;

	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector <const ExtVetoPMTHit*> m_ExtVetoPMTHit;
	vector <const ExtVetoPMTHit*>::const_iterator it;

	//1b: retrieve ExtVetoPMTHit objects
	loop->Get(m_ExtVetoPMTHit);


	/*Do the matching
	 *Proceed in this way:
	 * loop over the hits
	 * get the hit index, but put the readout to 0 (active detector element!)
	 * check if, in the map, a key with this detector index already exists.
	 * if not exist, add it, and create a new ExtVetoDigiHit
	 * if exist, get it, and add the PMT hit to the list of PMT hits of the ExtVetoDigiHit
	 */
	m_map.clear();
	for (it=m_ExtVetoPMTHit.begin(); it != m_ExtVetoPMTHit.end() ; it++){
		m_channel = *((*it)->m_channel.ext_veto);
		m_channel.readout = 0;
		m_map_it=m_map.find(m_channel);
		if (m_map_it == m_map.end()){ //not here. Create a new ExtVetoDigiHit object
			m_ExtVetoDigiHit=new ExtVetoDigiHit;
			m_ExtVetoDigiHit->m_channel=m_channel;
			m_ExtVetoDigiHit->AddAssociatedObject((*it));
			m_map.insert(std::make_pair(m_channel,m_ExtVetoDigiHit));
		}
		else{ //element already exists. Get the ExtVetoDigiHit
			m_ExtVetoDigiHit=m_map[m_channel];
			m_ExtVetoDigiHit->AddAssociatedObject((*it));
		}
	}

	/*Now the map is full of all the hits in different active elements of active veto, i.e. with different identifiers, that maps the PMT hits.
	 * Each hit has a reference to the PMT hits that made it */
	vector <const ExtVetoPMTHit*> m_ExtVetoPMTHit_tmp;
	ExtVetoDigiHit* m_ExtVetoDigiHit_tmp;
	double Qmax=-99999;

	for (m_map_it=m_map.begin();m_map_it!=m_map.end();m_map_it++){
		//do here further elaborations!
		//Compute the charge as the sum of the charges
		//Compute the hit-time as time of the PMT-hit with largest charge
		m_ExtVetoDigiHit_tmp=m_map_it->second;
		m_ExtVetoDigiHit_tmp->Get(m_ExtVetoPMTHit_tmp,"",0);  //0 means "associated only with this object

		m_ExtVetoDigiHit_tmp->Q=0;
		Qmax=-9999;
		for (int ihit=0;ihit<m_ExtVetoPMTHit_tmp.size();ihit++){
			ExtVetoDigiHit::ExtVetoPMTDigiHit hit;
			hit.Q=m_ExtVetoPMTHit_tmp[ihit]->Q;
			hit.T=m_ExtVetoPMTHit_tmp[ihit]->T;
			hit.readout=m_ExtVetoPMTHit_tmp.at(ihit)->m_channel.ext_veto->readout;
			m_ExtVetoDigiHit_tmp->m_data.push_back(hit);
			m_ExtVetoDigiHit_tmp->Q+=hit.Q;
			if (hit.Q>Qmax){
				Qmax=hit.Q;
				m_ExtVetoDigiHit_tmp->T=hit.T;
			}
		}
		_data.push_back(m_ExtVetoDigiHit_tmp); //publish it
	}

	return NOERROR;
}




//------------------
// erun
//------------------
jerror_t ExtVetoDigiHit_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t ExtVetoDigiHit_factory::fini(void)
{
	return NOERROR;
}

