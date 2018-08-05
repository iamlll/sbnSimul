#include <iostream>
#include <math.h>
#include <fstream>
#include "TCanvas.h"

using namespace std;

TLegend* CreateLegend(string legTitle, THStack* stack, string entryTitle){ 
  char* cstrLeg = new char[legTitle.length()+1];
  strcpy(cstrLeg, legTitle.c_str());
  int totalEntries = 0;
  TLegend* leg = new TLegend(0.45,0.7,0.9,0.9,cstrLeg);
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
    cout << "Proportion: " << proportion << endl;
    char* cstrEntry = new char [entryTitle.length()+1];
    strcpy (cstrEntry, entryTitle.c_str());
    sprintf(buffer, "%s%d%s%f%s",cstrEntry, num, " (", proportion, "%)");
    leg->AddEntry(hist, buffer, "f");
    num++;
  }
  return leg;
}  

void ProposalCuts_numu(){
  TCanvas* c1 = new TCanvas("c1","c1",1000,1000);
  TFile* numu = new TFile("output.root");
  THStack* cut_numu = (THStack*)numu->Get("fnumuCandidates");
  //Draw histograms
  cut_numu->SetTitle(";E#nu;count");
  auto leg4 = CreateLegend("#nu_#mu CC candidates", cut_numu, "#mu");
  cut_numu->Draw();
  leg4->Draw();
  
  TCanvas* c2 = new TCanvas ("c2", "c2", 1000, 1000);
  THStack* discriminate = (THStack*)numu->Get("fDiscriminate_numu_mupi");
  discriminate->Draw("nostack");
  auto legend = CreateLegend("Detailed #nu_#mu candidates", discriminate, "#mu passing");
  legend->Draw();
  //gPad->BuildLegend(0.45,0.7,0.9,0.9,"Detailed #nu_#mu candidates");
}
