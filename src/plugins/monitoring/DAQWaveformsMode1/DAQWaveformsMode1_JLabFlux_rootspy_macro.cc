#include <dlfcn.h>
#include <iostream>
#include <string>
using namespace std;
static string macro_data=""
"// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c0\n"
"// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c1\n"
"// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c2\n"
"// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c3\n"
"// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c4\n"
"// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c5\n"
"// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c6\n"
"// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c7\n"
"// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c8\n"
"// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c9\n"
"// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c10\n"
"// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c11\n"
"// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c12\n"
"// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c13\n"
"// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c14\n"
"// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c15\n"
"{\n"
"\n"
"	TDirectory *dir = (TDirectory*)gDirectory->FindObjectAny(\"DAQWaveformsMode1\");\n"
"	if(dir) dir->cd();\n"
"\n"
"	const int nChannels=16;\n"
"\n"
"	TH1D *h[nChannels];\n"
"	for (int ii=0;ii<nChannels;ii++){\n"
"		h[ii]= (TH1D*)gDirectory->FindObjectAny(Form(\"hDAQWaveform_c%i_s%i_c%i\",0,1,ii));\n"
"	}\n"
"\n"
"\n"
"	// Just for testing\n"
"	if(gPad == NULL){\n"
"		TCanvas *c1 = new TCanvas(\"cDAQWaveforms\");\n"
"		c1->cd(0);\n"
"		c1->Draw();\n"
"		c1->Update();\n"
"	}\n"
"\n"
"	if(!gPad) return;\n"
"	TCanvas *c1 = gPad->GetCanvas();\n"
"	c1->Divide(4, 4);\n"
"\n"
"	for (int ii=0;ii<nChannels;ii++){\n"
"		c1->cd(ii+1);\n"
"		gPad->SetTicks();\n"
"		gPad->SetGrid();\n"
"		if (h[ii]) h[ii]->Draw();\n"
"	}\n"
"}\n"
"\n"
"\n"
"\n"
;
class DAQWaveformsMode1_JLabFlux_rootspy_class{
   public:
   typedef void rmfcn(string, string, string);
   DAQWaveformsMode1_JLabFlux_rootspy_class(){
      rmfcn* fcn = (rmfcn*)dlsym(RTLD_DEFAULT, "REGISTER_ROOTSPY_MACRO");
      if(fcn) (*fcn)("DAQWaveformsMode1_JLabFlux","/", macro_data);
   }
};
static DAQWaveformsMode1_JLabFlux_rootspy_class tmp;