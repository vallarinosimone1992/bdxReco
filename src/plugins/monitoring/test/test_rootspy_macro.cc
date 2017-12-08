#include <dlfcn.h>
#include <iostream>
#include <string>
using namespace std;
static string macro_data=""
"// hnamepath: /test/h1\n"
"// hnamepath: /test/h2\n"
"\n"
"{\n"
"\n"
"	TDirectory *dir = (TDirectory*)gDirectory->FindObjectAny(\"test\");\n"
"	if(dir) dir->cd();\n"
"\n"
"	TH1D *h1 = (TH1D*)gDirectory->FindObjectAny(\"h1\");\n"
"	TH1D *h2 = (TH1D*)gDirectory->FindObjectAny(\"h2\");\n"
"\n"
"\n"
"	// Just for testing\n"
"	if(gPad == NULL){\n"
"		TCanvas *c1 = new TCanvas(\"c1\");\n"
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
"	gPad->SetTicks();\n"
"	gPad->SetGrid();\n"
"	if(h1) h1->Draw();\n"
"\n"
"	c1->cd(2);\n"
"	gPad->SetTicks();\n"
"	gPad->SetGrid();\n"
"	if(h1) h1->Draw();\n"
"\n"
"	c1->cd(3);\n"
"	gPad->SetTicks();\n"
"	gPad->SetGrid();\n"
"	if(h2) h2->Draw();\n"
"\n"
"	c1->cd(4);\n"
"	gPad->SetTicks();\n"
"	gPad->SetGrid();\n"
"	if(h1) h1->Draw();\n"
"}\n"
"\n"
"\n"
"\n"
;
class test_rootspy_class{
   public:
   typedef void rmfcn(string, string, string);
   test_rootspy_class(){
      rmfcn* fcn = (rmfcn*)dlsym(RTLD_DEFAULT, "REGISTER_ROOTSPY_MACRO");
      if(fcn) (*fcn)("test","/", macro_data);
   }
};
static test_rootspy_class tmp;