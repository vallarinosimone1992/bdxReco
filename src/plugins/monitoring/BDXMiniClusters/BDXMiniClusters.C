// hnamepath: /BDXMiniClusters/hBDXMiniClusterEtotTOP
// hnamepath: /BDXMiniClusters/hBDXMiniClusterEtotBOT
// hnamepath: /BDXMiniClusters/hBDXMiniClusterEseedTOP
// hnamepath: /BDXMiniClusters/hBDXMiniClusterEseedBOT
// hnamepath: /BDXMiniClusters/hBDXMiniClusterNTOP
// hnamepath: /BDXMiniClusters/hBDXMiniClusterNBOT

{

	TDirectory *dir = (TDirectory*)gDirectory->FindObjectAny("BDXMiniClusters");
	if(dir) dir->cd();


	TH1D *hBDXMiniClusterEtotTOP=0;
	TH1D *hBDXMiniClusterEseedTOP=0;
	TH1D *hBDXMiniClusterNTOP=0;

	TH1D *hBDXMiniClusterEtotBOT=0;
	TH1D *hBDXMiniClusterEseedBOT=0;
	TH1D *hBDXMiniClusterNBOT=0;

	// Just for testing
	if(gPad == NULL) {
		TCanvas *c1 = new TCanvas("cBDXMiniClusters");
		c1->cd(0);
		c1->Draw();
		c1->Update();
	}

	if(!gPad) return;
	TCanvas *c1 = gPad->GetCanvas();
	c1->Divide(2, 3);

	c1->cd(1);
	hBDXMiniClusterEtotTOP->Draw();

	c1->cd(2);
	hBDXMiniClusterEseedTOP->Draw();

	c1->cd(3);
	hBDXMiniClusterNTOP->Draw();

	c1->cd(4);
	hBDXMiniClusterEtotBOT->Draw();

	c1->cd(5);
	hBDXMiniClusterEseedBOT->Draw();

	c1->cd(6);
	hBDXMiniClusterNBOT->Draw();

}

