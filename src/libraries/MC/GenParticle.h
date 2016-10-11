// $Id$
//
//    File: genParticle.h
// Created: Tue Mar  1 00:59:14 CET 2016
// Creator: celentan (on Linux localhost.localdomain 2.6.32-573.12.1.el6.x86_64 x86_64)
//

#ifndef _GenParticle_
#define _GenParticle_

#include <JANA/JObject.h>
#include <JANA/JFactory.h>
#include "outputFactory.h"
class GenParticle:public jana::JObject,generatedParticle{
	public:

		GenParticle(const generatedParticle base):generatedParticle(base){

		}
		JOBJECT_PUBLIC(GenParticle);
		
		// Add data members here. For example:
		// int id;
		// double E;
		
		// This method is used primarily for pretty printing
		// the second argument to AddString is printf style format
		void toStrings(vector<pair<string,string> > &items)const{
			 AddString(items, "id", "%4d", id);
			 AddString(items, "pid", "%4d", PID);
			 AddString(items, "px", "%4f", momentum.x());
			 AddString(items, "py", "%4f", momentum.y());
			 AddString(items, "pz", "%4f", momentum.z());
			 AddString(items, "vx", "%4f", vertex.x());
			 AddString(items, "vy", "%4f", vertex.y());
			 AddString(items, "vz", "%4f", vertex.z());
		}


		void Print()const{
			jout<<"(((((((((((((((((((((((Gen particle)))))))))))))))))))))))))"<<std::endl;
			jout<<"pid: "<<PID<<endl;
			jout<<momentum.x()<<" "<<momentum.y()<<" "<<momentum.z()<<std::endl;
			jout<<vertex.x()<<" "<<vertex.y()<<" "<<vertex.z()<<std::endl;
			jout<<"Ene true: "<<sqrt(momentum.x()*momentum.x()+momentum.y()*momentum.y()+momentum.z()*momentum.z())<<" MeV"<<std::endl;   // rest massignored
			jout<<"(((((((((((((((((((((((((((((()))))))))))))))))))))))))))))))"<<std::endl;

		}



		double px;
		double py;
		double pz;
        int pid;
        double vx;
        double vy;
        double vz;
        double E;



};

#endif // _genParticle_

