// hnamepath: /CalorimeterHits/hCaloHitE_allTrg_s0_x0_y0
// hnamepath: /CalorimeterHits/hCaloHitE_allTrg_s0_x1_y0

// hnamepath: /CalorimeterHits/hCaloHitE_rndmTrg_s0_x0_y0
// hnamepath: /CalorimeterHits/hCaloHitE_rndmTrg_s0_x1_y0

// hnamepath: /CalorimeterHits/hCaloHitE_Trg0_s0_x0_y0
// hnamepath: /CalorimeterHits/hCaloHitE_Trg0_s0_x1_y0

// hnamepath: /CalorimeterHits/hCaloHitE_Trg3_s0_x0_y0
// hnamepath: /CalorimeterHits/hCaloHitE_Trg3_s0_x1_y0
{

	TDirectory *dir = (TDirectory*) gDirectory->FindObjectAny("CalorimeterHits");
	if (dir) dir->cd();

	TH1D *hCaloHitE_allTrg_s0_x0_y0 = 0;
	TH1D *hCaloHitE_allTrg_s0_x1_y0 = 0;

	TH1D *hCaloHitE_rndmTrg_s0_x0_y0 = 0;
	TH1D *hCaloHitE_rndmTrg_s0_x1_y0 = 0;

	/*Top-bottom*/
	TH1D *hCaloHitE_Trg0_s0_x0_y0 = 0;
	TH1D *hCaloHitE_Trg0_s0_x1_y0 = 0;

	/*Crs*/
	TH1D *hCaloHitE_Trg2_s0_x0_y0 = 0;
	TH1D *hCaloHitE_Trg2_s0_x1_y0 = 0;

	/*Front-back*/
	TH1D *hCaloHitE_Trg3_s0_x0_y0 = 0;
	TH1D *hCaloHitE_Trg3_s0_x1_y0 = 0;

	hCaloHitE_allTrg_s0_x0_y0 = (TH1D*) gDirectory->FindObjectAny("hCaloHitE_allTrg_s0_x0_y0");
	hCaloHitE_rndmTrg_s0_x0_y0 = (TH1D*) gDirectory->FindObjectAny("hCaloHitE_rndmTrg_s0_x0_y0");
	hCaloHitE_Trg0_s0_x0_y0 = (TH1D*) gDirectory->FindObjectAny("hCaloHitE_Trg0_s0_x0_y0");
	hCaloHitE_Trg2_s0_x0_y0 = (TH1D*) gDirectory->FindObjectAny("hCaloHitE_Trg2_s0_x0_y0");
	hCaloHitE_Trg3_s0_x0_y0 = (TH1D*) gDirectory->FindObjectAny("hCaloHitE_Trg3_s0_x0_y0");

	hCaloHitE_allTrg_s0_x1_y0 = (TH1D*) gDirectory->FindObjectAny("hCaloHitE_allTrg_s0_x1_y0");
	hCaloHitE_rndmTrg_s0_x1_y0 = (TH1D*) gDirectory->FindObjectAny("hCaloHitE_rndmTrg_s0_x1_y0");
	hCaloHitE_Trg0_s0_x1_y0 = (TH1D*) gDirectory->FindObjectAny("hCaloHitE_Trg0_s0_x1_y0");
	hCaloHitE_Trg2_s0_x1_y0 = (TH1D*) gDirectory->FindObjectAny("hCaloHitE_Trg2_s0_x1_y0");
	hCaloHitE_Trg3_s0_x1_y0 = (TH1D*) gDirectory->FindObjectAny("hCaloHitE_Trg3_s0_x1_y0");

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
	if (hCaloHitE_allTrg_s0_x0_y0 != 0) {
		hCaloHitE_allTrg_s0_x0_y0->SetLineColor(1);
		hCaloHitE_allTrg_s0_x0_y0->Draw();
		if (hCaloHitE_rndmTrg_s0_x0_y0 != 0) {
			hCaloHitE_rndmTrg_s0_x0_y0->SetLineColor(2);
			hCaloHitE_rndmTrg_s0_x0_y0->Draw("SAMES");
		}

		if (hCaloHitE_Trg0_s0_x0_y0 != 0) {
			hCaloHitE_Trg0_s0_x0_y0->SetLineColor(3);
			hCaloHitE_Trg0_s0_x0_y0->Draw("SAMES");
		}

		if (hCaloHitE_Trg2_s0_x0_y0 != 0) {
			hCaloHitE_Trg2_s0_x0_y0->SetLineColor(4);
			hCaloHitE_Trg2_s0_x0_y0->Draw("SAMES");
		}

		if (hCaloHitE_Trg3_s0_x0_y0 != 0) {
			hCaloHitE_Trg3_s0_x0_y0->SetLineColor(5);
			hCaloHitE_Trg3_s0_x0_y0->Draw("SAMES");
		}

	}

	c1->cd(2)->SetLogy();
	if (hCaloHitE_allTrg_s0_x1_y0 != 0) {
		hCaloHitE_allTrg_s0_x1_y0->SetLineColor(1);
		hCaloHitE_allTrg_s0_x1_y0->Draw();
		if (hCaloHitE_rndmTrg_s0_x1_y0 != 0) {
			hCaloHitE_rndmTrg_s0_x1_y0->SetLineColor(2);
			hCaloHitE_rndmTrg_s0_x1_y0->Draw("SAMES");
		}

		if (hCaloHitE_Trg0_s0_x1_y0 != 0) {
			hCaloHitE_Trg0_s0_x1_y0->SetLineColor(3);
			hCaloHitE_Trg0_s0_x1_y0->Draw("SAMES");
		}

		if (hCaloHitE_Trg2_s0_x1_y0 != 0) {
			hCaloHitE_Trg2_s0_x1_y0->SetLineColor(4);
			hCaloHitE_Trg2_s0_x1_y0->Draw("SAMES");
		}

		if (hCaloHitE_Trg3_s0_x1_y0 != 0) {
			hCaloHitE_Trg3_s0_x1_y0->SetLineColor(5);
			hCaloHitE_Trg3_s0_x1_y0->Draw("SAMES");
		}

	}
}
