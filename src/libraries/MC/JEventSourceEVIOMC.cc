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
		evt = new Mevent(*EDT, hitTypes, &banksMap, 0);
		MCEVIOreference *the_reference=new MCEVIOreference;
		the_reference->EDT=EDT;
		the_reference->event=evt;
		event.SetJEventSource(this);
		event.SetRef(the_reference);

		event.SetEventNumber(evt->headerBank["evn"]);
		//read here the run number from MC
		curRunNumber=evt->headerBank["runNo"];
		if (overwriteRunNumber!=-1) event.SetRunNumber(overwriteRunNumber);
		else event.SetRunNumber(curRunNumber);


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
	MCEVIOreference *ref=(MCEVIOreference *)event.GetRef();
	if (ref!=0){
		if (ref->event) delete ref->event;
		if (ref->EDT) delete ref->EDT;
		delete ref;
	}

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

	MCEVIOreference   *the_reference = (MCEVIOreference*)event.GetRef();
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
	else if (dataClassName == "CalorimeterMCHit"){
		// getting EVIO bank
		vector<hitOutput> bankDgt = this_event->dgtBanks["crs"];
		vector<hitOutput> bankRaw = this_event->rawBanks["crs"];

		/*	map<string,double> dgt;
		map<string,double>::iterator dgt_it;
		if (bankDgt.size()==0) return NOERROR;
		if (bankRaw.size()==0) return NOERROR;
		for(unsigned int ih=0; ih<bankDgt.size(); ih++){

			dgt=bankDgt[ih].getDgtz();
			jout<<"has "<<dgt.size()<<endl;
			for (dgt_it=dgt.begin();dgt_it!=dgt.end();dgt_it++){
				jout<<"Dgt :"<<ih<<" "<<dgt_it->first<<" "<<dgt_it->second<<endl;
			}
		}
		for(unsigned int ih=0; ih<bankRaw.size(); ih++){
			dgt=bankRaw[ih].getRaws();
			jout<<"has "<<dgt.size()<<endl;
			for (dgt_it=dgt.begin();dgt_it!=dgt.end();dgt_it++){
				jout<<"Raw :"<<ih<<" "<<dgt_it->first<<" "<<dgt_it->second<<endl;
			}
		}
		 */

		if (bankDgt.size()!=bankRaw.size()){
			jerr<<"Calorimeter MC banks raw and dgtz different size"<<endl;
			return VALUE_OUT_OF_RANGE;
		}
		vector<CalorimeterMCHit*> caloMChits;
		for(unsigned int ih=0; ih<bankDgt.size(); ih++)
		{

			CalorimeterMCHit *hit = new CalorimeterMCHit;

			hit->sector=bankDgt[ih].getIntDgtVar("sector");
			hit->x=bankDgt[ih].getIntDgtVar("x");
			hit->y=bankDgt[ih].getIntDgtVar("y");


			/*dgtz banks*/
			hit->adcl=bankDgt[ih].getIntDgtVar("adcl");
			hit->adcr=bankDgt[ih].getIntDgtVar("adcr");
			hit->tdcl=bankDgt[ih].getIntDgtVar("tdcl");
			hit->tdcr=bankDgt[ih].getIntDgtVar("tdcr");
			hit->adcb=bankDgt[ih].getIntDgtVar("adcb");
			hit->adcf=bankDgt[ih].getIntDgtVar("adcf");
			hit->tdcb=bankDgt[ih].getIntDgtVar("tdcb");
			hit->tdcf=bankDgt[ih].getIntDgtVar("tdcf");


			/*raw banks*/
			for(unsigned int ir=0; ir<bankRaw.size(); ir++)
			{
				if (bankRaw[ir].getIntRawVar("hitn")==bankDgt[ih].getIntDgtVar("hitn")){
					hit->totEdep=bankRaw[ir].getIntRawVar("totEdep");
					break;
				}
			}


			caloMChits.push_back(hit);
		}

		// publish the hit
		JFactory<CalorimeterMCHit> *fac = dynamic_cast<JFactory<CalorimeterMCHit>*>(factory);
		fac->CopyTo(caloMChits);
		return NOERROR;
	}

	else if (dataClassName == "IntVetoMCHit"){
		// getting EVIO bank
		vector<hitOutput> bankDgt = this_event->dgtBanks["veto"];
		vector<hitOutput> bankRaw = this_event->rawBanks["veto"];

		if (bankDgt.size()!=bankRaw.size()){
			jerr<<"Veto MC banks raw and dgtz different size"<<endl;
			jerr<<"At this level the check is only on the veto overall"<<endl;
			return VALUE_OUT_OF_RANGE;
		}
		vector<IntVetoMCHit*> intVetoMChits;
		for(unsigned int ih=0; ih<bankDgt.size(); ih++)
		{
			if (bankDgt[ih].getIntDgtVar("veto")!=1) continue;  //since Marco used the same bank for all the vetos.. blah~!!


			IntVetoMCHit *hit = new IntVetoMCHit;

			hit->sector=bankDgt[ih].getIntDgtVar("sector");
			hit->channel=bankDgt[ih].getIntDgtVar("channel");


			/*dgtz banks*/
			hit->adc1=bankDgt[ih].getIntDgtVar("adc1");
			hit->adc2=bankDgt[ih].getIntDgtVar("adc2");
			hit->adc3=bankDgt[ih].getIntDgtVar("adc3");
			hit->adc4=bankDgt[ih].getIntDgtVar("adc4");
			hit->tdc1=bankDgt[ih].getIntDgtVar("tdc1");
			hit->tdc2=bankDgt[ih].getIntDgtVar("tdc2");
			hit->tdc3=bankDgt[ih].getIntDgtVar("tdc3");
			hit->tdc4=bankDgt[ih].getIntDgtVar("tdc4");

			/*raw banks*/
			for(unsigned int ir=0; ir<bankRaw.size(); ir++)
			{
				if (bankRaw[ir].getIntRawVar("hitn")==bankDgt[ih].getIntDgtVar("hitn")){
					hit->totEdep=bankRaw[ir].getIntRawVar("totEdep");
					break;
				}
			}

			intVetoMChits.push_back(hit);
		}
		// publish the hit
		JFactory<IntVetoMCHit> *fac = dynamic_cast<JFactory<IntVetoMCHit>*>(factory);
		fac->CopyTo(intVetoMChits);
		return NOERROR;

	}


	else if (dataClassName == "ExtVetoMCHit"){
			// getting EVIO bank
			vector<hitOutput> bankDgt = this_event->dgtBanks["veto"];
			vector<hitOutput> bankRaw = this_event->rawBanks["veto"];

			if (bankDgt.size()!=bankRaw.size()){
				jerr<<"Veto MC banks raw and dgtz different size"<<endl;
				jerr<<"At this level the check is only on the veto overall"<<endl;
				return VALUE_OUT_OF_RANGE;
			}
			vector<ExtVetoMCHit*> extVetoMChits;
			for(unsigned int ih=0; ih<bankDgt.size(); ih++)
			{
				if (bankDgt[ih].getIntDgtVar("veto")!=2) continue;  //since Marco used the same bank for all the vetos.. blah~!!


				ExtVetoMCHit *hit = new ExtVetoMCHit;

				hit->sector=bankDgt[ih].getIntDgtVar("sector");
				hit->channel=bankDgt[ih].getIntDgtVar("channel");


				/*dgtz banks*/
				hit->adc=bankDgt[ih].getIntDgtVar("adc1");
				hit->tdc=bankDgt[ih].getIntDgtVar("tdc1");

				/*raw banks*/
				for(unsigned int ir=0; ir<bankRaw.size(); ir++)
				{
					if (bankRaw[ir].getIntRawVar("hitn")==bankDgt[ih].getIntDgtVar("hitn")){
						hit->totEdep=bankRaw[ir].getIntRawVar("totEdep");
						break;
					}
				}

				extVetoMChits.push_back(hit);
			}
			// publish the hit
			JFactory<ExtVetoMCHit> *fac = dynamic_cast<JFactory<ExtVetoMCHit>*>(factory);
			fac->CopyTo(extVetoMChits);
			return NOERROR;

		}

	else if (dataClassName == "PaddlesMCHit"){
				// getting EVIO bank
				vector<hitOutput> bankDgt = this_event->dgtBanks["veto"];
				vector<hitOutput> bankRaw = this_event->rawBanks["veto"];

				if (bankDgt.size()!=bankRaw.size()){
					jerr<<"Veto MC banks raw and dgtz different size"<<endl;
					jerr<<"At this level the check is only on the veto overall"<<endl;
					return VALUE_OUT_OF_RANGE;
				}
				vector<PaddlesMCHit*> paddlesMChits;
				for(unsigned int ih=0; ih<bankDgt.size(); ih++)
				{
					if (bankDgt[ih].getIntDgtVar("veto")!=3) continue;  //since Marco used the same bank for all the vetos.. blah~!!


					PaddlesMCHit *hit = new PaddlesMCHit;

					hit->sector=bankDgt[ih].getIntDgtVar("sector");
					hit->channel=bankDgt[ih].getIntDgtVar("channel");


					/*dgtz banks*/
					hit->adc=bankDgt[ih].getIntDgtVar("adc1");
					hit->tdc=bankDgt[ih].getIntDgtVar("tdc1");

					/*raw banks*/
					for(unsigned int ir=0; ir<bankRaw.size(); ir++)
					{
						if (bankRaw[ir].getIntRawVar("hitn")==bankDgt[ih].getIntDgtVar("hitn")){
							hit->totEdep=bankRaw[ir].getIntRawVar("totEdep");
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








