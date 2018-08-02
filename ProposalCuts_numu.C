#include <iostream>
#include <math.h>
#include <fstream>
#include "TCanvas.h"

using namespace std;

TLegend* CreateLegend(string legTitle, THStack* stack, string entryTitle){ 
  char* cstrLeg = new char[legTitle.length()+1];
  strcpy(cstrLeg, legTitle.c_str());
  int totalEntries = 0;
  TLegend* leg = new TLegend(0.6,0.7,0.9,0.9,cstrLeg);
  vector<int> successes;
  for(auto obj: *stack->GetHists()){
    TH1D* hist = (TH1D*) obj;
    int entries=0;
    for(int bin=0; bin < hist->GetNbinsX(); bin++){
      entries += hist->GetBinContent(bin);
    }
    totalEntries += entries;
    successes.push_back(entries);
  }
  char buffer[20];
  int num=0; //num of hists
  for(auto obj: *stack->GetHists()){
    TH1D* hist = (TH1D*) obj;
    double proportion = ((double)successes[num])/totalEntries*100;
    char* cstrEntry = new char [entryTitle.length()+1];
    strcpy (cstrEntry, entryTitle.c_str());
    sprintf(buffer, "%s%d%s%f%s",cstrEntry, num, " (", proportion, "%)");
    leg->AddEntry(hist, buffer, "f");
    num++;
  }
  return leg;
}  

void ProposalCuts_numu(){
  TCanvas* canv = new TCanvas("canv","canv",1000,1000);
  canv->Divide(2,2);
  TFile* nue = new TFile("numuOutput.root");
  THStack* cut1 = (THStack*)nue->Get("fnueCC");
  THStack* cut2 = (THStack*)nue->Get("fGammaNC");
  THStack* cut3 = (THStack*)nue->Get("fnumuCC");
  THStack* cut_numu = (THStack*)nue->Get("fnumuCandidates");
  //Draw histograms
  canv->cd(1);
  cut1->SetTitle(";E#nu;count");
  cut1->Draw();
  auto leg1 = CreateLegend("Intrinsic/signal #nu_e CC", cut1, "#nueCC");
  leg1->Draw();

  canv->cd(2);
  cut2->SetTitle(";E#nu;count");
  cut2->Draw();
  auto leg2 = CreateLegend("NC #gamma production", cut2, "#gamma");
  leg2->Draw();
  
  canv->cd(3);
  cut3->SetTitle(";E#nu;count");
  cut3->Draw();
  auto leg3 = CreateLegend("CC #nu_#mu", cut3, "#numuCC");
  leg3->Draw();

  canv->cd(4);
  cut_numu->SetTitle(";E#nu;count");
  auto leg4 = CreateLegend("#nu_#mu CC candidates", cut_numu, "#mu");
  cut_numu->Draw();
  leg4->Draw();
}
