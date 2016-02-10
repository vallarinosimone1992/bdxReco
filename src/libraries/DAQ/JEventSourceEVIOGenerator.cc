// C++ headers
#include <string>
using namespace std;

// bdx headers
#include "JEventSourceEVIOGenerator.h"
#include "JEventSourceEVIO.h"

// Describes the file format
const char* JEventSourceEvioGenerator::Description(void)
{
	return "Evio";
}

// Check if it's a valid file
double JEventSourceEvioGenerator::CheckOpenable(string source)
{
	/// This always returns 100% probability of 
	/// opening the source
	return 1.0;
}

// Make the file a JEventSource
JEventSource* JEventSourceEvioGenerator::MakeJEventSource(string source)
{
	return new JEventSourceEvio(source.c_str());
}
		

