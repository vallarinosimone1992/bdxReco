

// hnamepath: /IntVetoSipm/hSipmCharge_s0_l0_c1_r1
// hnamepath: /IntVetoSipm/hSipmCharge_s0_l0_c2_r1
// hnamepath: /IntVetoSipm/hSipmCharge_s0_l0_c3_r1
// hnamepath: /IntVetoSipm/hSipmCharge_s0_l0_c4_r1
// hnamepath: /IntVetoSipm/hSipmCharge_s0_l0_c5_r1
// hnamepath: /IntVetoSipm/hSipmCharge_s0_l0_c6_r1
// hnamepath: /IntVetoSipm/hSipmCharge_s0_l0_c7_r1
// hnamepath: /IntVetoSipm/hSipmCharge_s0_l0_c8_r1
// hnamepath: /IntVetoSipm/hSipmCharge_s0_l0_c9_r1
// hnamepath: /IntVetoSipm/hSipmCharge_s0_l0_c10_r1
// hnamepath: /IntVetoSipm/hSipmCharge_s0_l0_c11_r1
// hnamepath: /IntVetoSipm/hSipmCharge_s0_l0_c12_r1
// hnamepath: /IntVetoSipm/hSipmCharge_s0_l0_c13_r1

// hnamepath: /IntVetoSipm/hSipmAmpl_s0_l0_c1_r1
// hnamepath: /IntVetoSipm/hSipmAmpl_s0_l0_c2_r1
// hnamepath: /IntVetoSipm/hSipmAmpl_s0_l0_c3_r1
// hnamepath: /IntVetoSipm/hSipmAmpl_s0_l0_c4_r1
// hnamepath: /IntVetoSipm/hSipmAmpl_s0_l0_c5_r1
// hnamepath: /IntVetoSipm/hSipmAmpl_s0_l0_c6_r1
// hnamepath: /IntVetoSipm/hSipmAmpl_s0_l0_c7_r1
// hnamepath: /IntVetoSipm/hSipmAmpl_s0_l0_c8_r1
// hnamepath: /IntVetoSipm/hSipmAmpl_s0_l0_c9_r1
// hnamepath: /IntVetoSipm/hSipmAmpl_s0_l0_c10_r1
// hnamepath: /IntVetoSipm/hSipmAmpl_s0_l0_c11_r1
// hnamepath: /IntVetoSipm/hSipmAmpl_s0_l0_c12_r1
// hnamepath: /IntVetoSipm/hSipmAmpl_s0_l0_c13_r1

{

	TDirectory *dir = (TDirectory*)gDirectory->FindObjectAny("IntVetoSipm");
	if(dir) dir->cd();

	const int nChannels=13;

	TH1D *hQ[nChannels];
	TH1D *hA[nChannels];

	for (int ii=0;ii<nChannels;ii++) {
		hQ[ii]= (TH1D*)gDirectory->FindObjectAny(Form("hSipmCharge_s%i_l%i_c%i_r%i",0,0,ii+1,1));
		hA[ii]= (TH1D*)gDirectory->FindObjectAny(Form("hSipmAmpl_s%i_l%i_c%i_r%i",0,0,ii+1,1));

	}

	// Just for testing
	if(gPad == NULL) {
		TCanvas *c1 = new TCanvas("cIntVetoSipm");
		c1->cd(0);
		c1->Draw();
		c1->Update();
	}

	if(!gPad) return;
	TCanvas *c1 = gPad->GetCanvas();
	c1->Divide(4, 4);

	for (int ii=0;ii<nChannels;ii++) {
		c1->cd(ii+1);
		gPad->SetTicks();
		gPad->SetGrid();
		if (hQ[ii]){
			hQ[ii]->SetLineWidth(2);
			hQ[ii]->Draw("HIST");
		}
		if (hA[ii]) {
			hA[ii]->SetLineWidth(2);
			hA[ii]->SetLineColor(2);
			hA[ii]->Draw("HISTSAME");
		}
	}
}
