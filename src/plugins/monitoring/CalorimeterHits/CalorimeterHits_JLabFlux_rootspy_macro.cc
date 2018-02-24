#include <dlfcn.h>
#include <iostream>
#include <string>
using namespace std;
static string macro_data=""
"\n"
"// hnamepath: /CalorimeterHits/hCaloHitE_allTrg_s0_x0_y0\n"
"// hnamepath: /CalorimeterHits/hCaloHitE_allTrg_s0_x1_y0\n"
"\n"
"// hnamepath: /CalorimeterHits/hCaloHitE_rndmTrg_s0_x0_y0\n"
"// hnamepath: /CalorimeterHits/hCaloHitE_rndmTrg_s0_x1_y0\n"
"\n"
"{\n"
"\n"
"	TDirectory *dir = (TDirectory*) gDirectory->FindObjectAny(\"CalorimeterHits\");\n"
"	if (dir) dir->cd();\n"
"\n"
"	TH1D *hCaloHitE_allTrg_s0_x0_y0;\n"
"	TH1D *hCaloHitE_allTrg_s0_x1_y0;\n"
"	TH1D *hCaloHitE_rndmTrg_s0_x0_y0;\n"
"	TH1D *hCaloHitE_rndmTrg_s0_x1_y0;\n"
"\n"
"	hCaloHitE_allTrg_s0_x0_y0 = (TH1D*) gDirectory->FindObjectAny(\"hCaloHitE_allTrg_s0_x0_y0\");\n"
"	hCaloHitE_rndmTrg_s0_x1_y0 = (TH1D*) gDirectory->FindObjectAny(\"hCaloHitE_allTrg_s0_x1_y0\");\n"
"	hCaloHitE_allTrg_s0_x0_y0 = (TH1D*) gDirectory->FindObjectAny(\"hCaloHitE_rndmTrg_s0_x0_y0\");\n"
"	hCaloHitE_rndmTrg_s0_x1_y0 = (TH1D*) gDirectory->FindObjectAny(\"hCaloHitE_rndmTrg_s0_x1_y0\");\n"
"\n"
"	if (gPad == NULL) {\n"
"		TCanvas *c1 = new TCanvas(\"cCaloHits\");\n"
"		c1->cd(0);\n"
"		c1->Draw();\n"
"		c1->Update();\n"
"	}\n"
"\n"
"	if (!gPad) return;\n"
"	TCanvas *c1 = gPad->GetCanvas();\n"
"	c1->Divide(2, 2);\n"
"	c1->cd(1)->SetLogy();\n"
"	hCaloHitE_allTrg_s0_x0_y0->Draw();\n"
"	hCaloHitE_rndmTrg_s0_x0_y0->Draw(\"sames\");\n"
"\n"
"	c1->cd(2)->SetLogy();\n"
"	hCaloHitE_allTrg_s0_x1_y0->Draw();\n"
"	hCaloHitE_rndmTrg_s0_x1_y0->Draw(\"sames\");\n"
"\n"
"}\n"
;
class CalorimeterHits_JLabFlux_rootspy_class{
   public:
   typedef void rmfcn(string, string, string);
   CalorimeterHits_JLabFlux_rootspy_class(){
      rmfcn* fcn = (rmfcn*)dlsym(RTLD_DEFAULT, "REGISTER_ROOTSPY_MACRO");
      if(fcn) (*fcn)("CalorimeterHits_JLabFlux","/", macro_data);
   }
};
static CalorimeterHits_JLabFlux_rootspy_class tmp;