#include <dlfcn.h>
#include <iostream>
#include <string>
using namespace std;
static string macro_data=""
"// hnamepath: /Trigger/hTriggerBits\n"
"// hnamepath: /Trigger/hTriggerMultiplicity\n"
"// hnamepath: /Trigger/hTriggerTimes\n"
"// hnamepath: /Trigger/hTriggerTimes2D\n"
"\n"
"{\n"
"\n"
"	TDirectory *dir = (TDirectory*)gDirectory->FindObjectAny(\"TriggerTimes\");\n"
"	if(dir) dir->cd();\n"
"\n"
"	TH1D *hTriggerBits=0;\n"
"	TH1D *hTriggerTimes=0;\n"
"	TH1D *hTriggerMultiplicity=0;\n"
"	TH2D *hTriggerTimes2D=0;\n"
"\n"
"	hTriggerBits=(TH1D*)gDirectory->FindObjectAny(\"hTriggerBits\");\n"
"	hTriggerMultiplicity=(TH1D*)gDirectory->FindObjectAny(\"hTriggerMultiplicity\");\n"
"	hTriggerTimes=(TH1D*)gDirectory->FindObjectAny(\"hTriggerTimes\");\n"
"	hTriggerTimes2D=(TH2D*)gDirectory->FindObjectAny(\"hTriggerTimes2D\");\n"
"\n"
"\n"
"	if(gPad == NULL) {\n"
"		TCanvas *c1 = new TCanvas(\"cTrigger\");\n"
"		c1->cd(0);\n"
"		c1->Draw();\n"
"		c1->Update();\n"
"	}\n"
"\n"
"	if(!gPad) return;\n"
"	TCanvas *c1 = gPad->GetCanvas();\n"
"	c1->Divide(2, 2);\n"
"\n"
"	c1->cd(1);\n"
"	if (hTriggerMultiplicity) hTriggerMultiplicity->Draw(\"HIST\");\n"
"	c1->cd(2);\n"
"	if (hTriggerBits) hTriggerBits->Draw(\"HIST\");\n"
"	c1->cd(3)->SetLogy();\n"
"	if (hTriggerTimes) hTriggerTimes->Draw(\"HIST\");\n"
"	c1->cd(4)->SetLogz();\n"
"	if (hTriggerTimes2D) hTriggerTimes2D->Draw(\"colz\");\n"
"}\n"
;
class Trigger_JLabFlux_rootspy_class{
   public:
   typedef void rmfcn(string, string, string);
   Trigger_JLabFlux_rootspy_class(){
      rmfcn* fcn = (rmfcn*)dlsym(RTLD_DEFAULT, "REGISTER_ROOTSPY_MACRO");
      if(fcn) (*fcn)("Trigger_JLabFlux","/", macro_data);
   }
};
static Trigger_JLabFlux_rootspy_class tmp;