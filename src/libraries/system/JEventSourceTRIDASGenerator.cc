// C++ headers
#include <string>
using namespace std;

// bdx headers
#include <system/JEventSourceTRIDASGenerator.h>
#include <DAQ/JEventSourceTRIDASDAQ.h>

#include <JANA/JParameterManager.h>
using namespace jana;

//---------------------------------
// Description
//---------------------------------
const char* JEventSourceTRIDASGenerator::Description(void) {
	return "TRIDAS";
}

//---------------------------------
// CheckOpenable
//---------------------------------
double JEventSourceTRIDASGenerator::CheckOpenable(string source) {
	// This should return a value between 0 and 1 inclusive
	// with 1 indicating it definitely can read events from
	// the specified source and 0 meaning it definitely can't.
	// Typically, this will just check the file suffix.

	if (source.find("pt") != std::string::npos) {
		std::cout << "JEventSourceTRIDASGenerator: source name " << source << " contains \"pt\" substring. Open it" << endl;
		return 1.0;
	} else {
		std::cout << "JEventSourceTRIDASGenerator failed on source " << source << endl;
		return 0.0;
	}
}

//---------------------------------
// MakeJEventSource
//---------------------------------
JEventSource* JEventSourceTRIDASGenerator::MakeJEventSource(string source) {

	int m_isMC, m_verbose;
	m_isMC = 0;
	m_verbose = 0;
	gPARMS->GetParameter("MC", m_isMC);
	gPARMS->GetParameter("SYSTEM:VERBOSE", m_verbose);

	if (m_isMC == 0) {
		if (m_verbose > 2) jout << "JEventSourceTRIDASGenerator::MakeJEventSource for DATA " << endl;
		return new JEventSourceTRIDASDAQ(source.c_str());
	} else if (m_isMC >= 1) {
		jerr << "MC - TRIDAS format not yet supported!!" << endl;
		throw JException("Failed to open MC TRIDAS file - no MC support yet");
	}
}

