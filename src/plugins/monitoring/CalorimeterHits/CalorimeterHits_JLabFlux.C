
// hnamepath: /CalorimeterHits/hCaloHitE_allTrg_s0_x0_y0
// hnamepath: /CalorimeterHits/hCaloHitE_allTrg_s0_x1_y0

// hnamepath: /CalorimeterHits/hCaloHitE_rndmTrg_s0_x0_y0
// hnamepath: /CalorimeterHits/hCaloHitE_rndmTrg_s0_x1_y0

{

	TDirectory *dir = (TDirectory*) gDirectory->FindObjectAny("CalorimeterHits");
	if (dir) dir->cd();

	TH1D *hCaloHitE_allTrg_s0_x0_y0=0;
	TH1D *hCaloHitE_allTrg_s0_x1_y0=0;
	TH1D *hCaloHitE_rndmTrg_s0_x0_y0=0;
	TH1D *hCaloHitE_rndmTrg_s0_x1_y0=0;

	hCaloHitE_allTrg_s0_x0_y0 = (TH1D*) gDirectory->FindObjectAny("hCaloHitE_allTrg_s0_x0_y0");
	hCaloHitE_rndmTrg_s0_x1_y0 = (TH1D*) gDirectory->FindObjectAny("hCaloHitE_allTrg_s0_x1_y0");
	hCaloHitE_allTrg_s0_x0_y0 = (TH1D*) gDirectory->FindObjectAny("hCaloHitE_rndmTrg_s0_x0_y0");
	hCaloHitE_rndmTrg_s0_x1_y0 = (TH1D*) gDirectory->FindObjectAny("hCaloHitE_rndmTrg_s0_x1_y0");

	if (gPad == NULL) {
		TCanvas *c1 = new TCanvas("cCaloHits");
		c1->cd(0);
		c1->Draw();
		c1->Update();
	}

	if (!gPad) return;
	TCanvas *c1 = gPad->GetCanvas();
	c1->Divide(2, 2);
	c1->cd(1)->SetLogy();
	if (hCaloHitE_allTrg_s0_x0_y0!=0){
		hCaloHitE_allTrg_s0_x0_y0->Draw();
		if (hCaloHitE_rndmTrg_s0_x0_y0!=0) hCaloHitE_rndmTrg_s0_x0_y0->Draw("sames");
	}
	c1->cd(2)->SetLogy();
	if (hCaloHitE_allTrg_s0_x1_y0!=0){
		hCaloHitE_allTrg_s0_x1_y0->Draw();
		if (hCaloHitE_rndmTrg_s0_x1_y0!=0) hCaloHitE_rndmTrg_s0_x1_y0->Draw("sames");
	}

}
