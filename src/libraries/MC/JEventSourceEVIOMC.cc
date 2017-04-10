// JANA headers
#include "JEventSourceEVIOMC.h"

#include <JANA/JEvent.h>
#include <JANA/JParameterManager.h>
#include <system/JEventSourceEVIOGenerator.h>
// bdx headers
#include "CLHEP/Units/PhysicalConstants.h"
using namespace CLHEP;

// C++ headers
#include <iostream>
using namespace std;

//MC
#include "GenParticle.h"
#include "CalorimeterMCHit.h"
#include "IntVetoMCHit.h"
#include "ExtVetoMCHit.h"
#include "PaddlesMCHit.h"
#include "VetoMCHit.h"
//GEMC stuff to read simulation
#include <MC/options.h>
#include <gbank.h>
#include <outputFactory.h>
// Constructor
JEventSourceEvioMC::JEventSourceEvioMC(const char* source_name) :
		JEventSource(source_name), chan(0), EDT(0), curRunNumber(0), curEventNumber(0) {
	overwriteRunNumber = -1;
	gPARMS->SetDefaultParameter("MC:RUN_NUMBER", overwriteRunNumber);

	// open EVIO file - buffer is hardcoded at 3M... that right?
	jout << " Opening MC input file " << source_name << "." << endl;

	try {
		chan = new evioFileChannel(source_name, "r", 300000);
		//chan = new evioFileChannel(source_name, "r");
		chan->open();
	} catch (evioException *e) {
		chan = 0;
		jerr << e->toString() << endl;
	}

	/*Read the bank map*/
	map<string, string> allSystems;
	allSystems["bdx"] = "TEXT";   //from gbank.cc src file, it will look for a file "name__bank.txt"
	goptions bdxOpt;
	banksMap = read_banks(bdxOpt, allSystems);

}

// Destructor
JEventSourceEvioMC::~JEventSourceEvioMC() {
	cout << "JEventSourceEvioMC closing input file " << source_name << "." << endl;
	chan->close();
	delete chan;
}

// GetEvent
jerror_t JEventSourceEvioMC::GetEvent(JEvent &event) {

	event.SetRef(NULL);

	vector<string> hitTypes;
	hitTypes.push_back("crs");
	hitTypes.push_back("veto");
/*
	map<string, gBank>::iterator it;
	for (it = banksMap.begin(); it != banksMap.end(); it++) {
		jout << "!!! " << it->first << " " << it->second.bankName << endl;
		for (int a = 0; a < it->second.name.size(); a++) {
			jout << it->second.name[a] << " " << it->second.type[a] << " " << endl;
		}
	}*/

	if (chan->read()) {
		EDT = new evioDOMTree(chan);
		evt = new Mevent(*EDT, hitTypes, &banksMap, 0);
		MCEVIOreference *the_reference = new MCEVIOreference;
		the_reference->EDT = EDT;
		the_reference->event = evt;
		event.SetJEventSource(this);
		event.SetRef(the_reference);

		event.SetEventNumber(evt->headerBank["evn"]);
		//read here the run number from MC
		curRunNumber = evt->headerBank["runNo"];
		if (overwriteRunNumber != -1)
			event.SetRunNumber(overwriteRunNumber);
		else
			event.SetRunNumber(curRunNumber);

		return NOERROR;
	} else {
		jout << "Source done" << endl;
		return NO_MORE_EVENTS_IN_SOURCE;
	}
}

// FreeEvent
void JEventSourceEvioMC::FreeEvent(JEvent &event) {
	MCEVIOreference *ref = (MCEVIOreference *) event.GetRef();
	if (ref != 0) {
		if (ref->event) delete ref->event;
		if (ref->EDT) delete ref->EDT;
		delete ref;
	}

}

// GetObjects
jerror_t JEventSourceEvioMC::GetObjects(JEvent &event, JFactory_base *factory) {
	/// This gets called through the virtual method of the
	/// JEventSource base class. It creates the objects of the type
	/// which factory is based.

	/// Example: DCsegment needs DCHit. If DCHit doesn't exist already, then
	/// it will be read here.

	// We must have a factory to hold the data
	if (!factory) throw RESOURCE_UNAVAILABLE;

	// Get name of data class we're trying to extract
	string dataClassName = factory->GetDataClassName();

	MCEVIOreference *the_reference = (MCEVIOreference*) event.GetRef();
	evioDOMTree *this_edt = the_reference->EDT;
	Mevent *this_event = the_reference->event;

	if (dataClassName == "GenParticle") {
		vector<generatedParticle> particles; //has to be here since CopyTo requires 1 vector
		map<string, gBank>::iterator it;
		it = banksMap.find("generated");
		if (it == banksMap.end()) {
			jerr << "Something strange, no generated bank this event: " << event.GetEventNumber() << endl << fflush;
			return VALUE_OUT_OF_RANGE;
		}

		particles = getGenerated(*this_edt, it->second, 0);

		//jout << "There are: " << particles.size() << "primaries" << endl;

		vector<GenParticle*> jparticles;
		for (int ip = 0; ip < particles.size(); ip++) {
			GenParticle *particle = new GenParticle();

			particle->pid = particles[ip].PID;
			particle->px = particles[ip].momentum.x();
			particle->py = particles[ip].momentum.y();
			particle->pz = particles[ip].momentum.z();
			particle->vx = particles[ip].vertex.x();
			particle->vy = particles[ip].vertex.y();
			particle->vz = particles[ip].vertex.z();

			jparticles.push_back(particle);
		}

		JFactory<GenParticle> *fac = dynamic_cast<JFactory<GenParticle>*>(factory);
		fac->CopyTo(jparticles);
		return NOERROR;

	} else if (dataClassName == "CalorimeterMCHit") {
		// getting EVIO bank
		vector<hitOutput> bankDgt = this_event->dgtBanks["crs"];
		vector<hitOutput> bankRaw = this_event->rawBanks["crs"];


	//	vector<hitOutput> bankDgt = getDgtIntDataBank(*this_edt,"crs",&banksMap,2);
		//vector<hitOutput> bankRaw = getRawIntDataBank(*this_edt,"crs",&banksMap,2);





		if (bankDgt.size() != bankRaw.size()) {
			jerr << "Calorimeter MC banks raw and dgtz different size" << endl;
			return VALUE_OUT_OF_RANGE;
		}
		vector<CalorimeterMCHit*> caloMChits;
		for (unsigned int ih = 0; ih < bankDgt.size(); ih++) {

			CalorimeterMCHit *hit = new CalorimeterMCHit;

			/*raw banks -> This check is needed BECAUSE it is not guaranteed they're ordered the same way*/
			for (unsigned int ir = 0; ir < bankRaw.size(); ir++) {
				if (bankRaw[ir].getIntRawVar("hitn") == bankDgt[ih].getIntDgtVar("hitn")) {
					hit->totEdep = bankRaw[ir].getIntRawVar("totEdep");
					hit->pid = bankRaw[ir].getIntRawVar("pid");
					break;
				}
			}
			//A.C. this check is here for 0-edep hits from neutrinos.. don't want to have them
			if (hit->totEdep <= 0) {
				delete hit;
				hit = 0;
				continue;
			}

			hit->sector = bankDgt[ih].getIntDgtVar("sector");
			hit->x = bankDgt[ih].getIntDgtVar("xch");
			hit->y = bankDgt[ih].getIntDgtVar("ych");


			/*dgtz banks*/
			hit->adcl = bankDgt[ih].getIntDgtVar("adcl");
			hit->adcr = bankDgt[ih].getIntDgtVar("adcr");
			hit->tdcl = bankDgt[ih].getIntDgtVar("tdcl");
			hit->tdcr = bankDgt[ih].getIntDgtVar("tdcr");
			hit->adcb = bankDgt[ih].getIntDgtVar("adcb");
			hit->adcf = bankDgt[ih].getIntDgtVar("adcf");
			hit->tdcb = bankDgt[ih].getIntDgtVar("tdcb");
			hit->tdcf = bankDgt[ih].getIntDgtVar("tdcf");

			//				jout << hit->adcl << " "<< hit->tdcl << endl;
			//			jout << hit->adcr << " "<< hit->tdcr << endl;

			caloMChits.push_back(hit);
		}

		// publish the hit
		JFactory<CalorimeterMCHit> *fac = dynamic_cast<JFactory<CalorimeterMCHit>*>(factory);
		fac->CopyTo(caloMChits);
		return NOERROR;
	}

	else if (dataClassName == "IntVetoMCHit") {
		// getting EVIO bank
		vector<hitOutput> bankDgt = this_event->dgtBanks["veto"];
		vector<hitOutput> bankRaw = this_event->rawBanks["veto"];

		if (bankDgt.size() != bankRaw.size()) {
			jerr << "Veto MC banks raw and dgtz different size" << endl;
			jerr << "At this level the check is only on the veto overall" << endl;
			return VALUE_OUT_OF_RANGE;
		}
		vector<IntVetoMCHit*> intVetoMChits;
		for (unsigned int ih = 0; ih < bankDgt.size(); ih++) {
			if ((bankDgt[ih].getIntDgtVar("veto") != VetoMCHit::CATANIA_INTVETO) && (bankDgt[ih].getIntDgtVar("veto") != VetoMCHit::FULL_INTVETO)) continue;

			IntVetoMCHit *hit = new IntVetoMCHit;

			/*raw banks*/
			for (unsigned int ir = 0; ir < bankRaw.size(); ir++) {
				if (bankRaw[ir].getIntRawVar("hitn") == bankDgt[ih].getIntDgtVar("hitn")) {
					hit->totEdep = bankRaw[ir].getIntRawVar("totEdep");

					break;
				}
			}

			if (hit->totEdep <= 0) {
				delete hit;
				hit = 0;
				continue;
			}

			hit->sector = bankDgt[ih].getIntDgtVar("sector");
			hit->channel = bankDgt[ih].getIntDgtVar("channel");

			//    jout << "sector "<< hit->sector << " " << hit->channel<<endl;

			hit->system = bankDgt[ih].getIntDgtVar("veto");

			/*dgtz banks*/
			hit->adc1 = bankDgt[ih].getIntDgtVar("adc1");
			hit->adc2 = bankDgt[ih].getIntDgtVar("adc2");
			hit->adc3 = bankDgt[ih].getIntDgtVar("adc3");
			hit->adc4 = bankDgt[ih].getIntDgtVar("adc4");
			hit->tdc1 = bankDgt[ih].getIntDgtVar("tdc1");
			hit->tdc2 = bankDgt[ih].getIntDgtVar("tdc2");
			hit->tdc3 = bankDgt[ih].getIntDgtVar("tdc3");
			hit->tdc4 = bankDgt[ih].getIntDgtVar("tdc4");

			//		jout << hit->adc1 << " "<< hit->tdc1 << endl;
			//		jout << hit->adc2 << " "<< hit->tdc2 << endl;
			//		jout << hit->adc3 << " "<< hit->tdc3 << endl;
			//		jout << hit->adc4 << " "<< hit->tdc4 << endl;

			intVetoMChits.push_back(hit);
		}
		// publish the hit
		JFactory<IntVetoMCHit> *fac = dynamic_cast<JFactory<IntVetoMCHit>*>(factory);
		fac->CopyTo(intVetoMChits);
		return NOERROR;

	}

	else if (dataClassName == "ExtVetoMCHit") {
		// getting EVIO bank
		vector<hitOutput> bankDgt = this_event->dgtBanks["veto"];
		vector<hitOutput> bankRaw = this_event->rawBanks["veto"];

		if (bankDgt.size() != bankRaw.size()) {
			jerr << "Veto MC banks raw and dgtz different size" << endl;
			jerr << "At this level the check is only on the veto overall" << endl;
			return VALUE_OUT_OF_RANGE;
		}
		vector<ExtVetoMCHit*> extVetoMChits;
		for (unsigned int ih = 0; ih < bankDgt.size(); ih++) {
			if ((bankDgt[ih].getIntDgtVar("veto") != VetoMCHit::CATANIA_EXTVETO) && (bankDgt[ih].getIntDgtVar("veto") != VetoMCHit::FULL_EXTVETO)) continue; //since Marco used the same bank for all the vetos.. blah~!!

			ExtVetoMCHit *hit = new ExtVetoMCHit;

			/*raw banks*/
			for (unsigned int ir = 0; ir < bankRaw.size(); ir++) {
				if (bankRaw[ir].getIntRawVar("hitn") == bankDgt[ih].getIntDgtVar("hitn")) {
					hit->totEdep = bankRaw[ir].getIntRawVar("totEdep");

					break;
				}
			}
			if (hit->totEdep <= 0) {
				delete hit;
				hit = 0;
				continue;
			}

			hit->sector = bankDgt[ih].getIntDgtVar("sector");
			hit->channel = bankDgt[ih].getIntDgtVar("channel");
			hit->system = bankDgt[ih].getIntDgtVar("veto");

			/*dgtz banks*/
			/*Note that there's a difference between CataniaProto1 and CataniaProto2. But at this stage, we read all the DgtVar ADC1-2-3-4.
			 * It will be later, in the ExtVeto library, that this will be handled properly*/

			hit->adc1 = bankDgt[ih].getIntDgtVar("adc1");
			hit->tdc1 = bankDgt[ih].getIntDgtVar("tdc1");
			hit->adc2 = bankDgt[ih].getIntDgtVar("adc2");
			hit->tdc2 = bankDgt[ih].getIntDgtVar("tdc2");
			hit->adc3 = bankDgt[ih].getIntDgtVar("adc3");
			hit->tdc3 = bankDgt[ih].getIntDgtVar("tdc3");
			hit->adc4 = bankDgt[ih].getIntDgtVar("adc4");
			hit->tdc4 = bankDgt[ih].getIntDgtVar("tdc4");

			//		jout << hit->adc << " "<< hit->tdc << endl;

			extVetoMChits.push_back(hit);
		}
		// publish the hit
		JFactory<ExtVetoMCHit> *fac = dynamic_cast<JFactory<ExtVetoMCHit>*>(factory);
		fac->CopyTo(extVetoMChits);
		return NOERROR;

	}

	else if (dataClassName == "PaddlesMCHit") {
		// getting EVIO bank
		vector<hitOutput> bankDgt = this_event->dgtBanks["veto"];
		vector<hitOutput> bankRaw = this_event->rawBanks["veto"];

		if (bankDgt.size() != bankRaw.size()) {
			jerr << "Veto MC banks raw and dgtz different size" << endl;
			jerr << "At this level the check is only on the veto overall" << endl;
			return VALUE_OUT_OF_RANGE;
		}
		vector<PaddlesMCHit*> paddlesMChits;
		for (unsigned int ih = 0; ih < bankDgt.size(); ih++) {
			if (bankDgt[ih].getIntDgtVar("veto") != VetoMCHit::CATANIA_PADDLES) continue;  //since Marco used the same bank for all the vetos.. blah~!!

			PaddlesMCHit *hit = new PaddlesMCHit;

			hit->sector = bankDgt[ih].getIntDgtVar("sector");
			hit->channel = bankDgt[ih].getIntDgtVar("channel");

			/*dgtz banks*/
			hit->adc = bankDgt[ih].getIntDgtVar("adc1");
			hit->tdc = bankDgt[ih].getIntDgtVar("tdc1");

			/*raw banks*/
			for (unsigned int ir = 0; ir < bankRaw.size(); ir++) {
				if (bankRaw[ir].getIntRawVar("hitn") == bankDgt[ih].getIntDgtVar("hitn")) {
					hit->totEdep = bankRaw[ir].getIntRawVar("totEdep");

					break;
				}
			}

			paddlesMChits.push_back(hit);
		}
		// publish the hit
		JFactory<PaddlesMCHit> *fac = dynamic_cast<JFactory<PaddlesMCHit>*>(factory);
		fac->CopyTo(paddlesMChits);
		return NOERROR;

	}

	// Just return. The _data vector should already be reset to have zero objects
	return OBJECT_NOT_AVAILABLE;
}

