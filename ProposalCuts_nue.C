#include <iostream>
#include <math.h>
#include <fstream>
#include "TCanvas.h"

using namespace std;

TLegend* CreateNamedLegend(string legTitle, THStack* stack, vector<string> titles, bool percent){ 
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
  if(percent==true){
    for(auto obj: *stack->GetHists()){
      TH1D* hist = (TH1D*) obj;
      double proportion = ((double)successes[num])/totalEntries*100;
      char* cstrEntry = new char [titles[num].length()+1];
      strcpy (cstrEntry, titles[num].c_str());
      sprintf(buffer, "%s%s%f%s",cstrEntry, " (", proportion, "%)");
      leg->AddEntry(hist, buffer, "f");
      num++;
    }
  }
  else{
    for(auto obj: *stack->GetHists()){
      TH1D* hist = (TH1D*) obj;
      char* cstrEntry = new char [titles[num].length()+1];
      strcpy (cstrEntry, titles[num].c_str());
      sprintf(buffer, "%s%s%d%s",cstrEntry, " (", successes[num], ")");
      leg->AddEntry(hist, buffer, "f");
      num++;
    }

  }
  return leg;
}  

void DrawRectangle(double xbotleft, double ybotleft, double xtoprt, double ytoprt, Color_t color){
  TLine* left = new TLine(xbotleft, ybotleft, xbotleft, ytoprt); 
  TLine* right = new TLine(xtoprt, ybotleft, xtoprt, ytoprt);
  TLine* bottom = new TLine(xbotleft, ybotleft, xtoprt, ybotleft);
  TLine* top = new TLine(xbotleft, ytoprt, xtoprt, ytoprt);

  left->SetLineColor(color);
  right->SetLineColor(color);
  top->SetLineColor(color);
  bottom->SetLineColor(color);
  left->SetLineWidth(3);
  right->SetLineWidth(3);
  top->SetLineWidth(3);
  bottom->SetLineWidth(3);

  left->Draw();
  right->Draw();
  top->Draw();
  bottom->Draw();
}

void ProposalCuts_nue(){
  //TFile* nue = new TFile("nue100koutput.root");
  TFile* nue = new TFile("output.root");
 
  TH2D* xy = (TH2D*)nue->Get("nu_vtx_XY");
  TH2D* xz = (TH2D*)nue->Get("nu_vtx_XZ");
  TH2D* yz = (TH2D*)nue->Get("nu_vtx_YZ");
  
  THStack* shAssns = (THStack*)nue->Get("showerE_stack");
  THStack* cut1 = (THStack*)nue->Get("fnueCC");
  THStack* cut1reco = (THStack*)nue->Get("fnueCC_reco");
  THStack* fig11 = (THStack*)nue->Get("fig11Stack");
  THStack* cut2 = (THStack*)nue->Get("fGammaNC");
  //THStack* gammas = (THStack*)nue->Get("gammaStack");
  THStack* cut3 = (THStack*)nue->Get("fnumuCC");
  //TH2D* hadronE = (TH2D*)nue->Get("E_hadron");
  TH1D* dEdx = (TH1D*)nue->Get("dEdx");
  THStack* dEdx_stack = (THStack*)nue->Get("showerStack");
  THStack* gammaVertex = (THStack*)nue->Get("phoStack");
  THStack* chargedHadAct = (THStack*)nue->Get("hadStack");
  TH2D* nue_vs_reco = (TH2D*)nue->Get("nuE_vs_reco");
  THStack* prelimCuts = (THStack*)nue->Get("prelim_stack_nue");
  TH1D* dist_from_vertex = (TH1D*)nue->Get("dist_from_vertex");
  THStack* vertexDist_truth = (THStack*)nue->Get("truthVD_stack");
  THStack* vertexDist_reco = (THStack*)nue->Get("recoVD_stack");

  //Draw histograms
  TCanvas* canv2d = new TCanvas("2dcanv","2dcanv",2400,800);
  canv2d->Divide(3,1);
  canv2d->cd(1);
  xy->Draw();
  //TPC 0
  DrawRectangle(-199.15,-200,-2.65,200,kBlue);//act vol
  DrawRectangle(-199.15+25,-200+25,-2.65-25,200-25, kRed);//fid vol
  //TPC 1
  DrawRectangle(2.65,-200,199.15,200,kBlue);//act vol
  DrawRectangle(2.65+25,-200+25,199.15-25,200-25, kRed);//fid vol
  //bounding box
  DrawRectangle(-260.1,-271.15,260.1,271.15,kYellow);

  canv2d->cd(2);  
  yz->Draw();
  DrawRectangle(-200,0,200,500,kBlue);//act vol
  DrawRectangle(-200+25,25,200-25,500-25, kRed);//fid vol
  //bounding box
  DrawRectangle(-271.15,-143.1,271.15,559.6,kYellow);

  canv2d->cd(3);
  xz->Draw();
  //TPC 0
  DrawRectangle(-199.15,0,-2.65,500,kBlue);//act vol
  DrawRectangle(-199.15+25,25,-2.65-25,500-25, kRed);//fid vol
  //TPC 1
  DrawRectangle(2.65,0,199.15,500,kBlue);//act vol
  DrawRectangle(2.65+25,25,199.15-25,500-25, kRed);//fid vol
  //bounding box
  DrawRectangle(-260.1,-143.1,260.1,559.6,kYellow);

/*
  TCanvas* shcanv = new TCanvas("shcanv","shcanv",800,800);
  shAssns->Draw();
  CreateNamedLegend("shower assns",shAssns,{"showers assoc. w/ #nu", "showers not assoc. w/ #nu"},false)->Draw();

  TCanvas* pcanv = new TCanvas("pcanv","pcanv",1000,1000);
  prelimCuts->Draw("nostack");  
  CreateNamedLegend("",prelimCuts,{"Total #nu interactions","Interactions in fid vol","true #nu_e CC interactions","#nu_e CC matched w/ shower","matched #nu_e CC w/ prim. e- shower"},false)->Draw();

  TCanvas* distcanv = new TCanvas("distcanv","distcanv",1500,750);
  distcanv->Divide(2,1);
  distcanv->cd(1);
  dist_from_vertex->Draw();
  distcanv->cd(2);
  vertexDist_truth->Draw();
  CreateNamedLegend("",vertexDist_truth,{"true #nu_e CC tr+sh","photon showers","cosmic rays"},false)->Draw();

  TCanvas *energycanv = new TCanvas("energycanv","energycanv",1000,1000);
  nue_vs_reco->Draw();

  TCanvas* c1 = new TCanvas("c1","c1",2000,1000);
  c1->Divide(2,1);
  c1->cd(1);
  cut1->Draw();
  CreateNamedLegend("#nu_e CC candidates (matched)", cut1,{"nu_e CC interactions","\" w/ 1+ matched E_sh > 200 MeV","\" which are prim. e- sh"},false)->Draw();
  c1->cd(2);
  cut1reco->Draw();  
  CreateNamedLegend("Cut 1, reconstructed E_#nu", cut1reco, {"ev w/ 1+ matched E_sh > 200 MeV","\" which are prim. e- sh","empty"},false)->Draw();
*/
/*
  TCanvas* canv11 = new TCanvas("canv11", "canv11", 1000, 1000);
  fig11->Draw();
  CreateNamedLegend("", fig11, {"#mu->#nue","K+->#nue","K0->#nue","NC single #gamma", "#nu_mu CC"},false)->Draw();

  TCanvas* c2 = new TCanvas("c2","c2",2000,1000);
  c2->Divide(2,1);
  c2->cd(1);
  cut2->Draw();
  CreateNamedLegend("NC #gamma prod", cut2, {"event rejected","event passed"},true)->Draw();
 
  c2->cd(2);
  gammas->Draw("nostack");
  CreateNamedLegend("Detailed #gamma prod", gammas, {"Total # #gamma showers","#gamma showers w/ #pi0 parent","#gamma sh with E_#gamma1 > 200 MeV","#pi0->2#gamma","2nd #gamma with E_#gamma2 > 100 MeV"},false)->Draw();
*/
/* 
  TCanvas* canvG = new TCanvas("canvG", "canvG", 1000,1000);
  gammaVertex->Draw("nostack");
  CreateNamedLegend("#gamma shower-vertex cuts", gammaVertex, {"Total # #gamma showers","#gamma prod from visible vertex","#gamma converting >3 cm from vertex"},false)->Draw();
*/

/*
  TCanvas* canv_showernu = new TCanvas("canv_showernu","canv_showernu",1000,1000);
  E_shower_nu->Draw();
*/

/*  TCanvas* c3 = new TCanvas("c3","c3",1000,1000);
  cut3->Draw();
  CreateNamedLegend("CC #nu_#mu", cut3, {"Failed #mu","Passed #mu"},true)->Draw();

  TCanvas* c4 = new TCanvas("c4","c4",2000,1000);
  c4->Divide(2,1);
  c4->cd(1);
  hadronE->Draw(); 
  c4->cd(2);
  chargedHadAct->Draw("nostack");
  CreateNamedLegend("Charged had. activity",chargedHadAct,{"Tot # interactions","E_ch.had > 0","E_hadron > 50 MeV"},false)->Draw();

  TCanvas* c5 = new TCanvas("c5","c5",2000,1000);
  c5->Divide(2,1);
  c5->cd(1);
  dEdx->Draw();
  c5->cd(2);
  dEdx_stack->Draw("nostack");
  CreateNamedLegend("dEdx",dEdx_stack,{"#gamma","e"},true)->Draw();
*/
}
