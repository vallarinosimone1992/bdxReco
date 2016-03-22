// $Id$
//
//    File: CalorimeterCluster_factory.cc
// Created: Tue Mar 22 11:19:57 CET 2016
// Creator: celentan (on Linux apcx4 2.6.32-504.30.3.el6.x86_64 x86_64)
//


#include <iostream>
#include <iomanip>

#include <cmath>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>      // std::pair, std::make_pair

using namespace std;

#include "CalorimeterCluster_factory.h"

#include "CalorimeterHit.h"

using namespace jana;


CalorimeterCluster_factory::CalorimeterCluster_factory(){
	m_CLUSTER_HIT_THR=20;
	m_CLUSTER_SEED_THR=20;
	m_CLUSTER_POS_W0=3.1;
	gPARMS->SetDefaultParameter("CALORIMETER:CLUSTER_HIT_THR",m_CLUSTER_HIT_THR,"Hit threshold in MeV for clustering");
	gPARMS->SetDefaultParameter("CALORIMETER:CLUSTER_SEED_THR",m_CLUSTER_SEED_THR,"Seed threshold in MeV for clustering");
	gPARMS->SetDefaultParameter("CALORIMETER:CLUSTER_POS_W0",m_CLUSTER_POS_W0,"Cluster position algorithm w0 factor");
}


//------------------
// init
//------------------
jerror_t CalorimeterCluster_factory::init(void)
{
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t CalorimeterCluster_factory::brun(jana::JEventLoop *eventLoop, int32_t runnumber)
{
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t CalorimeterCluster_factory::evnt(JEventLoop *loop, uint64_t eventnumber)
{

	/*Variables required*/
	map<int,vector<const CalorimeterHit*> > sector_hits;  /*key: sector, data: all hits in that sector*/
	map<int,vector<const CalorimeterHit*> >::const_iterator sector_hits_it;  /*key: sector, data: all hits in that sector*/


	vector<const CalorimeterHit*> all_hits;
	vector<const CalorimeterHit*>::const_iterator all_hits_it;
	loop->Get(all_hits);

	/*1: divide hits in different sectors. At this stage, do not apply yet any cut*/
	for (all_hits_it=all_hits.begin();all_hits_it!=all_hits.end();all_hits_it++){
		sector_hits[(*all_hits_it)->m_channel.sector].push_back(*all_hits_it);  //the [] operator in a map create a new data if key does not exists.
	}
	/*2: start the loop on the sectors*/
	for (sector_hits_it=sector_hits.begin();sector_hits_it!=sector_hits.end();sector_hits_it++){
		int this_sector;
		vector<const CalorimeterHit*> this_sector_hits;
		vector<const CalorimeterHit*>::iterator this_sector_hits_it;
		vector<const CalorimeterHit*>::iterator this_sector_hits_it2;

		vector<const CalorimeterHit*> this_sector_seeds;
		vector<const CalorimeterHit*>::iterator this_sector_seeds_it;

		map<const CalorimeterHit*,vector<const CalorimeterHit*> > this_sector_hit_to_seeds;   /*key: a hit. data: vector of all the seeds this is associated with*/
		map<const CalorimeterHit*,vector<const CalorimeterHit*> > this_sector_hit_to_seeds_clone;
		map<const CalorimeterHit*,vector<const CalorimeterHit*> >::iterator this_sector_hit_to_seeds_it;
		map<const CalorimeterHit*,vector<const CalorimeterHit*> >::iterator this_sector_hit_to_seeds_it2;
		vector<const CalorimeterHit*> this_hit_seeds;
		vector<const CalorimeterHit*>::iterator this_hit_seeds_it;

		map<const CalorimeterHit*,vector<const CalorimeterHit*> > this_sector_seed_to_hits;   /*key: a seed. data: vector of all the hits this is associated with*/
		map<const CalorimeterHit*,vector<const CalorimeterHit*> >::iterator this_sector_seed_to_hits_it;	 /*Note that a hit can be in different seeds!*/

		map<const CalorimeterHit*,CalorimeterCluster*> this_sector_seed_to_cluster;
		map<const CalorimeterHit*,CalorimeterCluster*>::iterator this_sector_seed_to_cluster_it;


		this_sector=(sector_hits_it)->first;
		this_sector_hits=(sector_hits_it)->second;

		/*3A: apply cuts*/
		for (this_sector_hits_it=this_sector_hits.begin();this_sector_hits_it!=this_sector_hits.end();this_sector_hits_it++){
			if ((*this_sector_hits_it)->E <= m_CLUSTER_HIT_THR){
				this_sector_hits.erase(this_sector_hits_it);
			}
		}//end 3A loop

		/*3B: search for seeds.
		 * -For each hit, check if the neighborhoods are there and fill a map with neighborhoods.
		 * -Check if the hit has the larger energy wrt the neighborhoods AND that the energy is higher than the seed thr.
		 * -If so, this is a seed hit
		 * */
		for (this_sector_hits_it=this_sector_hits.begin();this_sector_hits_it!=this_sector_hits.end();this_sector_hits_it++){
			vector<const CalorimeterHit*> this_hit_neighbors;
			vector<const CalorimeterHit*>::iterator this_hit_neighbors_it;
			bool isSeed=true;

			//first check of the seed energy cut. If it is not passed, then simply go to next hit
			if ((*this_sector_hits_it)->E<m_CLUSTER_SEED_THR){
				isSeed=false;
				continue;
			}
			//Make a vector of neighbors, excluding itself
			for (this_sector_hits_it2=this_sector_hits.begin();this_sector_hits_it2!=this_sector_hits.end();this_sector_hits_it2++){
				if (areNeighbors((*this_sector_hits_it),(*this_sector_hits_it2))){
					this_hit_neighbors.push_back(*this_sector_hits_it2);
				}
			}
			//Check if the energy of ANY neighbor is higher than the hit energy. If this happens, it is not a seed! If it does not happen, it is a seed
			for (this_hit_neighbors_it=this_hit_neighbors.begin();this_hit_neighbors_it!=this_hit_neighbors.end();this_hit_neighbors_it++){
				if ((*this_sector_hits_it)->E<(*this_hit_neighbors_it)->E){
					isSeed=false;
					break;
				}
			}
			if (isSeed){ /*Add the hit to the list of hits with associated seeds. The seed is clearly the hit itself!*/
				this_sector_hit_to_seeds[(*this_sector_hits_it)].push_back(*this_sector_hits_it);
				this_sector_seeds.push_back(*this_sector_hits_it);
			}
		}//end 3B loop

		/*3C: at this point, all seeds have been found.
		 * -Loop again, and on each hit that is not already a seed search for a neighboring seed.
		 * -If it exists, associate to the hit that seed
		 */
		for (this_sector_hits_it=this_sector_hits.begin();this_sector_hits_it!=this_sector_hits.end();this_sector_hits_it++){
			/*Skip it the hit if it is already a seed*/
			if (find(this_sector_seeds.begin(),this_sector_seeds.end(),*this_sector_hits_it)!=this_sector_seeds.end()){
				continue;
			}
			vector<const CalorimeterHit*> this_hit_neighbors;
			vector<const CalorimeterHit*>::iterator this_hit_neighbors_it;
			//Make a vector of neighbors, excluding itself
			for (this_sector_hits_it2=this_sector_hits.begin();this_sector_hits_it2!=this_sector_hits.end();this_sector_hits_it2++){
				if (areNeighbors((*this_sector_hits_it),(*this_sector_hits_it2))){
					this_hit_neighbors.push_back(*this_sector_hits_it2);
				}
			}
			/*Check if any neighbor is a seed. If so, add this hit to that seed. NOTE THERE MAY BE MORE THAN 1 SEED!!*/
			for (this_hit_neighbors_it=this_hit_neighbors.begin();this_hit_neighbors_it!=this_hit_neighbors.end();this_hit_neighbors_it++){
				if (find(this_sector_seeds.begin(),this_sector_seeds.end(),*this_hit_neighbors_it)!=this_sector_seeds.end()){
					this_sector_hit_to_seeds[(*this_sector_hits_it)].push_back(*this_hit_neighbors_it);
				}
			}
		}
		/*3D: there may still be some hits over thr that are NOT near a seed, but near a hit that is, by itself, near to a seed.
		 * So need to:
		 * -loop over the hits that are ALREADY associated with at least one seed
		 * -check for neighbors that are not associated with a seed, and that have lower energy
		 * -add these to the seed of the hit
		 */
		this_sector_hit_to_seeds_clone=this_sector_hit_to_seeds; //need to use a copy, since we modify the original map in the following loop!
		for (this_sector_hit_to_seeds_it=this_sector_hit_to_seeds_clone.begin();this_sector_hit_to_seeds_it!=this_sector_hit_to_seeds_clone.end();this_sector_hit_to_seeds_it++){
			vector<const CalorimeterHit*> this_hit_neighbors;
			vector<const CalorimeterHit*>::iterator this_hit_neighbors_it;
			/*Make a vector of all neighbors, excluding itself*/
			for (this_sector_hits_it=this_sector_hits.begin();this_sector_hits_it!=this_sector_hits.end();this_sector_hits_it++){
				if (areNeighbors((*this_sector_hit_to_seeds_it).first,(*this_sector_hits_it))){
					this_hit_neighbors.push_back(*this_sector_hits_it);
				}
			}
			/*Check if a neighbor is not associated with any seed and if it has less energy than this seeded hit*/
			/*A very rare case is when this seeded hit is associated with more different seeds*/
			/*Also need to check uniqueness!*/
			for (this_hit_neighbors_it=this_hit_neighbors.begin();this_hit_neighbors_it!=this_hit_neighbors.end();this_hit_neighbors_it++){
				if ((this_sector_hit_to_seeds_clone.find(*this_hit_neighbors_it)==this_sector_hit_to_seeds_clone.end()) &&  ((*this_hit_neighbors_it)->E <  (*this_sector_hit_to_seeds_it).first->E))        {
					this_hit_seeds=(*this_sector_hit_to_seeds_it).second; //these are the seeds associated with the seeded hit. This vector has at least 1 entry
					for (this_hit_seeds_it=	this_hit_seeds.begin();this_hit_seeds_it!=this_hit_seeds.end();this_hit_seeds_it++){//loop on the seeds 1 by 1
						if (find(this_sector_hit_to_seeds[(*this_hit_neighbors_it)].begin(),this_sector_hit_to_seeds[(*this_hit_neighbors_it)].end(),*this_hit_seeds_it)==this_sector_hit_to_seeds[(*this_hit_neighbors_it)].end()){
							this_sector_hit_to_seeds[(*this_hit_neighbors_it)].push_back(*this_hit_seeds_it);
						}
					}
				}
			}
		}

		/*4: now can create clusters*/
		/*4a: create the map seed -> all hits belonging to*/
		for (this_sector_hit_to_seeds_it=this_sector_hit_to_seeds.begin();this_sector_hit_to_seeds_it!=this_sector_hit_to_seeds.end();this_sector_hit_to_seeds_it++){
			for (this_hit_seeds_it=(*this_sector_hit_to_seeds_it).second.begin();this_hit_seeds_it!=(*this_sector_hit_to_seeds_it).second.end();this_hit_seeds_it++){
				this_sector_seed_to_hits[*this_hit_seeds_it].push_back((*this_sector_hit_to_seeds_it).first);
			}
		}

		/*4b create the clusters from the seeds
		 * -set the seed energy
		 * -set the time
		 * -add the cluster to the map of seeds->cluster
		 * - loop on the seeded hits, check one by one if it has this seed as seed, and also if it has OTHERS!
		 *
		 * */
		for (this_sector_seeds_it=this_sector_seeds.begin();this_sector_seeds_it!=this_sector_seeds.end();this_sector_seeds_it++){
			CalorimeterCluster *cluster = new CalorimeterCluster;
			double Etot=0;

			cluster->Eseed = (*this_sector_seeds_it)->E;
			cluster->E=0;
			cluster->T = (*this_sector_seeds_it)->T;
			cluster->AddAssociatedObject(*this_sector_seeds_it);
			this_sector_seed_to_cluster[*this_sector_seeds_it]=cluster;


			vector<pair<const CalorimeterHit*,double> > this_seed_weighted_hits;

			/*Loop on secondaries. Note that we must loop on this_sector_hit_to_seeds to check if a hit with this seed has ALSO other seeds!*/
			for (this_sector_hit_to_seeds_it=this_sector_hit_to_seeds.begin();this_sector_hit_to_seeds_it!=this_sector_hit_to_seeds.end();this_sector_hit_to_seeds_it++){
				double f=1;

				this_hit_seeds=(*this_sector_hit_to_seeds_it).second;
				/*Has this hit the above seed associated with? If no, continue*/
				if (find(this_hit_seeds.begin(),this_hit_seeds.end(),*this_sector_seeds_it)==this_hit_seeds.end()){
					continue;
				}
				/*Yes, the hit contains this seed (and maybe others!)*/
				/*Compute the relevant energy fraction if more than one seed*/
				/*Hit associated with this cluster only: f=1*/
				if (this_hit_seeds.size()==1){
					f=1;
				}
				/*Hit associated with other clusters too: f= this_cluster_seed_E / (sum of all_clusters_seed_E)*/
				else{
					f=0;
					for (this_hit_seeds_it=this_hit_seeds.begin();this_hit_seeds_it!=this_hit_seeds.end();this_hit_seeds_it++){
						f+=(*this_hit_seeds_it)->E;
					}
					f=(*this_sector_seeds_it)->E/f;
				}
				/*Fraction is now computed*/
				/*Add the hit energy to the cluster, for the relevant fraction*/
				this_seed_weighted_hits.push_back(make_pair((*this_sector_hit_to_seeds_it).first,f));
				cluster->E += f * ((*this_sector_hit_to_seeds_it).first->E);
			}
			/*Finally, compute position*/
			computePosition(cluster,this_seed_weighted_hits);

			_data.push_back(cluster); //publish it
		}
	}//end loop on sectors





	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t CalorimeterCluster_factory::erun(void)
{
	return NOERROR;
}


//------------------
// fini
//------------------
jerror_t CalorimeterCluster_factory::fini(void)
{
	return NOERROR;
}

void CalorimeterCluster_factory::computePosition(CalorimeterCluster* cluster,const std::vector<std::pair<const CalorimeterHit*, double> >& whits) const {
	std::vector<std::pair<const CalorimeterHit*, double> >::const_iterator whits_it;
	double Etot=cluster->E;
	double E;
	double pos_weight;

	double den,Xnum,Ynum;
	den=0;
	Xnum=0;
	Ynum=0;

	for (whits_it=whits.begin();whits_it!=whits.end();whits_it++){
		E=(*whits_it).first->E;
		pos_weight=std::max(0.,(m_CLUSTER_POS_W0+log(E/Etot)));
		den+=E*pos_weight;
		Xnum+=(*whits_it).first->m_channel.x*pos_weight;
		Ynum+=(*whits_it).first->m_channel.y*pos_weight;
	}
	cluster->x=Xnum/=den;
	cluster->y=Ynum/=den;


}









bool CalorimeterCluster_factory::areNeighbors(const CalorimeterHit *hit1,const CalorimeterHit *hit2)const{
	bool ret=false;
	if ((hit2->m_channel.x==hit1->m_channel.x-1)||(hit2->m_channel.x==hit1->m_channel.x+1)||(hit2->m_channel.y==hit1->m_channel.y-1)||(hit2->m_channel.y==hit1->m_channel.y+1)){
		ret =true;
	}

	return ret;
}



