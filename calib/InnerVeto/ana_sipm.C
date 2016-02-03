#include <iostream>
#include <map>
#include <TH1D.h>
#include <TTree.h>
#include <TFile.h>
#include <TF1.h>
#include <TMath.h>

using namespace std;

typedef struct{
  int sector;
  int layer;
  int component;
  int readout;
} INT_VETO_IndexLight_t;



bool operator<(const INT_VETO_IndexLight_t &a, const INT_VETO_IndexLight_t &b) {
	if (a.sector < b.sector) return true;
	if (a.sector > b.sector) return false;
	if (a.layer < b.layer) return true;
	if (a.layer > b.layer) return false;
	if (a.component < b.component) return true;
	if (a.component > b.component) return false;
	if (a.readout < b.readout) return true;
	if (a.readout > b.readout) return false;

	return false;
}

std::map<INT_VETO_IndexLight_t,TH1D *> h_map;
std::map<INT_VETO_IndexLight_t,TH1D *>::iterator h_map_it;

std::map<INT_VETO_IndexLight_t,TF1 *> f_map;
std::map<INT_VETO_IndexLight_t,TF1 *>::iterator f_map_it;

double three_gaus(double *xx,double *par){
  double x=xx[0];
  
  double ped=par[0];
  double singlephe=par[1];
  double A1=par[2];
  double A2=par[3];
  double A3=par[4];

  double sig1=par[5];
  double sig2=par[6];
  double sig3=par[7];

  double g1=A1*TMath::Gaus(x,ped,sig1);
  double g2=A2*TMath::Gaus(x,ped+singlephe,sig2);
  double g3=A3*TMath::Gaus(x,ped+2*singlephe,sig3);

  return g1+g2+g3;
}

void ana_sipm(string fname){
 
  static const int NBINS = 600;
  static const double XLOW = -0.5E3;
  static const double XUP = 2.5E3;

  static const double FLOW = -0.1E3;
  static const double FUP  = 1000;

  string outname=fname.substr(0,fname.length()-5); //".root"
  outname+=".ana_sipm.root";

  TFile *file1=new TFile(outname.c_str(),"recreate");
  TFile *file=new TFile(fname.c_str());  
  TTree *t=(TTree*)file->Get("tout");

  INT_VETO_IndexLight_t index;
  double Q;
 
  TH1D *h;
  TF1 *f;
  
  double A,sig,ped;

  t->SetBranchAddress("sector",&index.sector);
  t->SetBranchAddress("layer",&index.layer);
  t->SetBranchAddress("component",&index.component);
  t->SetBranchAddress("readout",&index.readout);
  t->SetBranchAddress("Q",&Q);

  int N=t->GetEntries();
  cout<<"There are: "<<N<<" entries " <<endl;
  
  for (int ii=0;ii<N;ii++){
    t->GetEntry(ii);
    h_map_it=h_map.find(index);
    if (h_map_it==h_map.end()){
      h_map.insert(std::make_pair(index,new TH1D(Form("h_%i_%i_%i_%i",index.sector,index.layer,index.component,index.readout),Form("h_%i_%i_%i_%i",index.sector,index.layer,index.component,index.readout),NBINS,XLOW,XUP)));
      f_map.insert(std::make_pair(index,new TF1(Form("f_%i_%i_%i_%i",index.sector,index.layer,index.component,index.readout),three_gaus,FLOW,FUP,8)));

    }
    h=h_map[index];
    h->Fill(Q);
  }
  
  file1->cd();
  for (h_map_it=h_map.begin();h_map_it!=h_map.end();h_map_it++){
    h=(h_map_it)->second;
    f=f_map.at(h_map_it->first);
    
    //First fit to determine A1, sigma1, ped
    h->Fit("gaus");
    A=h->GetFunction("gaus")->GetParameter(0);
    ped=h->GetFunction("gaus")->GetParameter(1);
    sig=h->GetFunction("gaus")->GetParameter(2);

    f->SetParameter(0,ped);
    f->SetParameter(1,200);
    f->SetParameter(2,A);
    f->SetParameter(3,A/10);
    f->SetParameter(4,A/20);
    f->SetParameter(5,sig);
    f->SetParameter(6,sig);
    f->SetParameter(7,sig);
    
    h->Fit(f->GetName(),"R","",FLOW,FUP);
    h->Write();
  }
  file1->Write();
  file1->Close();
}

