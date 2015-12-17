// JANA headers
#include <JANA/JEvent.h>

// bdx headers
#include "options.h"
#include "JEventSourceEVIO.h"
#include "JEventSourceEVIOGenerator.h"

// CLHEP headers
#include "CLHEP/Units/PhysicalConstants.h"
using namespace CLHEP;

// C++ headers
#include <iostream>
using namespace std;

// CTOF
#include "CTOFhit.h"


// Constructor
JEventSourceEvio::JEventSourceEvio(const char* source_name, goptions Opt):JEventSource(source_name)
{
	bdxOpt = Opt;

	string hd_msg = bdxOpt.optMap["LOG_MSG"].args + " Event: >> " ;
	string lbank  = bdxOpt.optMap["BANK_LIST"].args;      // list of banks to be saved in the output
	
	// Setting CLAS12 constants
	// Hardcoded for now but will be from database later on
	int nsect      = 6;    // CLAS12 sectors	
	gPARMS->SetDefaultParameter("CLAS12:NSECTORS", nsect);
	
	// Drift Chambers
	int dc_nreg    = 3;    // DC: Number or regions
	int dc_nslay_r = 2;    // DC: Number of superlayers per region
	int dc_nslay   = 6;    // DC: superlayers, 2 per region = 6 total
	int dc_nlayr   = 6;    // DC: layers within the superlayer
	int dc_nwire   = 113;  // DC: number of wires
	gPARMS->SetDefaultParameter("DC:NREGIONS",     dc_nreg);
	gPARMS->SetDefaultParameter("DC:NSLAYERS_R",   dc_nslay_r);
	gPARMS->SetDefaultParameter("DC:NSLAYERS",     dc_nslay);
	gPARMS->SetDefaultParameter("DC:NLAYERS",      dc_nlayr);	
	gPARMS->SetDefaultParameter("DC:NWIRES",       dc_nwire);
	

	// open EVIO file - buffer is hardcoded at 3M... that right?
	cout << hd_msg << " Opening input file " << source_name << "." << endl;
	chan = new evioFileChannel(source_name, "r", 300000);
	chan->open();
}


// Destructor
JEventSourceEvio::~JEventSourceEvio()
{
	cout << " Closing input file " << source_name << "." << endl;
	chan->close();
	delete chan;
}

// GetEvent
jerror_t JEventSourceEvio::GetEvent(JEvent &event)
{
	event.SetRef(NULL);

	vector<string> hitTypes;
 	hitTypes.push_back("ctof");
	
	map<string, string> allSystems;
	allSystems["ctof"] = "TEXT";
	
	banksMap = read_banks(bdxOpt, allSystems);

	if(chan->read())
	{
		
		evioDOMTree EDT(chan);
		Mevent *this_evt = new Mevent(EDT, hitTypes, &banksMap, 0);

		
		event.SetJEventSource(this);
		event.SetEventNumber(1);
		event.SetRef(this_evt);

		return NOERROR;
	}
	else
		return NO_MORE_EVENTS_IN_SOURCE;
}

// FreeEvent
void JEventSourceEvio::FreeEvent(JEvent &event)
{
	//	delete (Mevent*)event.GetRef();
}

// GetObjects
jerror_t JEventSourceEvio::GetObjects(JEvent &event, JFactory_base *factory)
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

	Mevent* this_evt = (Mevent*)event.GetRef();
	
	
	
	if(dataClassName == "CTOFhit")
	{
		// getting EVIO bank
		vector<hitOutput> bankDgt = this_evt->dgtBanks["ctof"];
		
		
		vector<CTOFhit*> ctofhits;
		for(unsigned int ih=0; ih<bankDgt.size(); ih++)
		{
			CTOFhit *ctofhit = new CTOFhit;
			ctofhit->paddle = bankDgt[ih].getIntDgtVar("paddle");
			ctofhit->ADCL   = bankDgt[ih].getIntDgtVar("ADCL");
			ctofhit->ADCR   = bankDgt[ih].getIntDgtVar("ADCR");
			ctofhits.push_back(ctofhit);
		}
		
		// publish the CTOFhit
		JFactory<CTOFhit> *fac = dynamic_cast<JFactory<CTOFhit>*>(factory);
		fac->CopyTo(ctofhits);
		return NOERROR;
		
	}
	
	
	// Just return. The _data vector should already be reset to have zero objects
	return OBJECT_NOT_AVAILABLE;
}







