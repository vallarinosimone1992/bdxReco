// hnamepath: /test/h1
// hnamepath: /test/h2

{

	TDirectory *dir = (TDirectory*)gDirectory->FindObjectAny("test");
	if(dir) dir->cd();

	TH1D *h1 = (TH1D*)gDirectory->FindObjectAny("h1");
	TH1D *h2 = (TH1D*)gDirectory->FindObjectAny("h2");


	// Just for testing
	if(gPad == NULL){
		TCanvas *c1 = new TCanvas("c1");
		c1->cd(0);
		c1->Draw();
		c1->Update();
	}

	if(!gPad) return;
	TCanvas *c1 = gPad->GetCanvas();
	c1->Divide(2, 2);

	c1->cd(1);
	gPad->SetTicks();
	gPad->SetGrid();
	if(h1) h1->Draw();

	c1->cd(2);
	gPad->SetTicks();
	gPad->SetGrid();
	if(h1) h1->Draw();

	c1->cd(3);
	gPad->SetTicks();
	gPad->SetGrid();
	if(h2) h2->Draw();

	c1->cd(4);
	gPad->SetTicks();
	gPad->SetGrid();
	if(h1) h1->Draw();
}



