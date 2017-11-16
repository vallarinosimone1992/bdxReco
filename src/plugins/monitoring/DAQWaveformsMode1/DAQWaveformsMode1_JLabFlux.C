// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c0
// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c1
// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c2
// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c3
// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c4
// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c5
// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c6
// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c7
// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c8
// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c9
// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c10
// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c11
// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c12
// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c13
// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c14
// hnamepath: /DAQWaveformsMode1/hDAQWaveform_c0_s1_c15
{

	TDirectory *dir = (TDirectory*)gDirectory->FindObjectAny("DAQWaveformsMode1");
	if(dir) dir->cd();

	const int nChannels=16;

	TH1D *h[nChannels];
	for (int ii=0;ii<nChannels;ii++){
		h[ii]= (TH1D*)gDirectory->FindObjectAny(Form("hDAQWaveform_c%i_s%i_c%i",0,1,ii));
	}


	// Just for testing
	if(gPad == NULL){
		TCanvas *c1 = new TCanvas("cDAQWaveforms");
		c1->cd(0);
		c1->Draw();
		c1->Update();
	}

	if(!gPad) return;
	TCanvas *c1 = gPad->GetCanvas();
	c1->Divide(4, 4);

	for (int ii=0;ii<nChannels;ii++){
		c1->cd(ii+1);
		gPad->SetTicks();
		gPad->SetGrid();
		if (h[ii]) h[ii]->Draw();
	}
}



