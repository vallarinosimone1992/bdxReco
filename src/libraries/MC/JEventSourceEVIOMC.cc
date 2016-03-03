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

//GEMC stuff to read simulation
#include <MC/options.h>
#include <gbank.h>
#include <outputFactory.h>
// Constructor
JEventSourceEvioMC::JEventSourceEvioMC(const char* source_name):JEventSource(source_name),
		chan(0),EDT(0),
		curRunNumber(0),curEventNumber(0)
{



	overwriteRunNumber=-1;


	// open EVIO file - buffer is hardcoded at 3M... that right?
	jout << " Opening MC input file " << source_name << "." << endl;

	try{
		//		chan = new evioFileChannel(source_name, "r", 300000);
		chan = new evioFileChannel(source_name, "r", 10804);
		chan->open();
	}
	catch(evioException *e){
		chan = 0;
		jerr<<e->toString()<<endl;
	}
}


// Destructor
JEventSourceEvioMC::~JEventSourceEvioMC()
{
	cout << " Closing input file " << source_name << "." << endl;
	chan->close();
	delete chan;
}

// GetEvent
jerror_t JEventSourceEvioMC::GetEvent(JEvent &event)
{

	event.SetRef(NULL);

	vector<string> hitTypes;
	hitTypes.push_back("crs");
	hitTypes.push_back("veto");

	map<string, string> allSystems;
	allSystems["bdx"] = "TEXT";   //from gbank.cc src file, it will look for a file "name__bank.txt"
	goptions bdxOpt;
	banksMap = read_banks(bdxOpt, allSystems);
	/*	map<string,gBank>::iterator it;
	for (it=banksMap.begin();it!=banksMap.end();it++){
		jout<<it->first<<" "<<it->second.bankName<<endl;
		for (int a=0;a<it->second.name.size();a++){
			jout<<it->second.name[a]<<" "<<it->second.type[a]<<" "<<endl;
		}
	}
	 */
	if(chan->read())
	{
		EDT=new evioDOMTree(chan);

		Mevent *this_evt = new Mevent(*EDT, hitTypes, &banksMap, 0);

		reference *the_reference=new reference;
		the_reference->EDT=EDT;
		the_reference->event=this_evt;
		event.SetJEventSource(this);
		event.SetRef(the_reference);

		event.SetEventNumber(this_evt->headerBank["evn"]);
		//read here the run number from MC
		curRunNumber=this_evt->headerBank["runNo"];
		if (overwriteRunNumber!=-1) event.SetRunNumber(overwriteRunNumber);
		else event.SetRunNumber(curRunNumber);

		delete this_evt;

		return NOERROR;
	}
	else{
		jout<<"Source done"<<endl;
		return NO_MORE_EVENTS_IN_SOURCE;
	}
}

// FreeEvent
void JEventSourceEvioMC::FreeEvent(JEvent &event)
{
	delete (evioDOMTree*)event.GetRef();
	//	delete (Mevent*)event.GetRef();
}

// GetObjects
jerror_t JEventSourceEvioMC::GetObjects(JEvent &event, JFactory_base *factory)
{
	/// This gets called through the virtual method of the
	/// JEventSource base class. It creates the objects of the type
	/// which factory is based.

	/// Example: DCsegment needs DCHit. If DCHit doesn't exist already, then
	/// it will be read here.

	// We must have a factory to hold the data
	if(!factory)throw RESOURCE_UNAVAILABLE;

	// Get name of data class we're trying to extract
	string dataClassName = factory->GetDataClassName();

	reference   *the_reference = (reference*)event.GetRef();
	evioDOMTree *this_edt = the_reference->EDT;
	Mevent 		*this_event = the_reference->event;



	if(dataClassName == "GenParticle"){
		vector<generatedParticle> particles; //has to be here since CopyTo requires 1 vector
		particles =	getGenerated(*this_edt,banksMap["generated"],0);

		if (particles.size()!=1){
			jerr<<"Something strange, more than 1 gen particle: "<<particles.size()<<endl;
			return VALUE_OUT_OF_RANGE;
		}
		else{

			vector<GenParticle*> jparticles;
			GenParticle *particle=new GenParticle(particles[0]);
			jparticles.push_back(particle);

			JFactory<GenParticle> *fac= dynamic_cast<JFactory<GenParticle>*>(factory);
			fac->CopyTo(jparticles);
			return NOERROR;
		}

	}
	else if (dataClassName == "CalorimeterDigiHitMC"){




		return NOERROR;
	}




	// Just return. The _data vector should already be reset to have zero objects
	return OBJECT_NOT_AVAILABLE;
}









