// $Id$
//
//    File: IntVetoDigiHit_factory.cc
// Created: Wed Jan 20 16:42:38 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>
#include <map>
using namespace std;

#include "IntVetoDigiHit_factory.h"

#include <IntVeto/IntVetoSiPMHit.h>
#include <TT/TranslationTable.h>

using namespace jana;

//------------------
// init
//------------------
jerror_t IntVetoDigiHit_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t IntVetoDigiHit_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{

	jout<<"IntVetoDigiHit_factory::brun new run number: "<<runnumber<<endl;
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
jerror_t IntVetoDigiHit_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	TranslationTable::INT_VETO_Index_t m_channel;
	IntVetoDigiHit *m_IntVetoDigiHit=0;

	//1: Here, we get from the framework the objects we need to process
	//1a: create vectors
	vector <const IntVetoSiPMHit*> m_IntVetoSiPMHit;
	vector <const IntVetoSiPMHit*>::const_iterator it;

	//1b: retrieve IntVetoSiPMHit objects
	loop->Get(m_IntVetoSiPMHit);


	/*Do the matching
	 *Proceed in this way:
	 * loop over the hits
	 * get the hit index, but put the readout to 0 (active detector element!)
	 * check if, in the map, a key with this detector index already exists.
	 * if not exist, add it, and create a new IntVetoDigiHit
	 * if exist, get it, and add the SiPM hit to the list of sipm hits of the IntVetoDigiHit
	 */
	m_map.clear();
	for (it=m_IntVetoSiPMHit.begin(); it != m_IntVetoSiPMHit.end() ; it++){
		m_channel = *((*it)->m_channel.int_veto);
		m_channel.readout = 0;
 		m_map_it=m_map.find(m_channel);
 		if (m_map_it == m_map.end()){ //not here. Create a new VetoIntDigiHit object
 			m_IntVetoDigiHit=new IntVetoDigiHit;
 			m_IntVetoDigiHit->m_channel=m_channel;
 			m_IntVetoDigiHit->AddAssociatedObject((*it));
 			m_map.insert(std::make_pair(m_channel,m_IntVetoDigiHit));
		}
 		else{ //element already exists. Get the VetoIntDigiHit
 			m_IntVetoDigiHit=m_map[m_channel];
 			m_IntVetoDigiHit->AddAssociatedObject((*it));
 		}
	}

	/*Now the map is full of all the hits in different active elements of active veto, i.e. with different identifiers, that maps the sipm hits.
	 * Each hit has a reference to the SiPM hits that made it
	 */
	vector <const IntVetoSiPMHit*> m_IntVetoSiPMHit_tmp;
	IntVetoDigiHit* m_IntVetoDigiHit_tmp;
	double Qmax=-99999;
	for (m_map_it=m_map.begin();m_map_it!=m_map.end();m_map_it++){
		//do here further elaborations!
		//Compute the charge as the sum of the charges
		//Compute the hit-time as time of the sipm-hit with largest charge
		m_IntVetoDigiHit_tmp=m_map_it->second;
		m_IntVetoDigiHit_tmp->Get(m_IntVetoSiPMHit_tmp,"",0);  //0 means "associated only with this object

		m_IntVetoDigiHit_tmp->Qtot=0;

		for (int ihit=0;ihit<m_IntVetoSiPMHit_tmp.size();ihit++){
			IntVetoDigiHit::IntVetoSiPMDigiHit hit;
			hit.Q=m_IntVetoSiPMHit_tmp.at(ihit)->Qphe;
			hit.T=m_IntVetoSiPMHit_tmp.at(ihit)->T;
			hit.readout=m_IntVetoSiPMHit_tmp.at(ihit)->m_channel.int_veto->readout;
			m_IntVetoDigiHit_tmp->m_data.push_back(hit);
			m_IntVetoDigiHit_tmp->Qtot+=hit.Q;
		}
		_data.push_back(m_IntVetoDigiHit_tmp); //publish it
	}

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t IntVetoDigiHit_factory::erun(void)
{
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t IntVetoDigiHit_factory::fini(void)
{
	return NOERROR;
}

