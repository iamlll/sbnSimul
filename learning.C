#include <iostream>
#include <math.h>
#include <fstream>
#include <vector>
#include <typeinfo>
#include <cstdlib>

using namespace std;

void learning(){
   TFile* output = new TFile("output.root");
   TTree* tree = (TTree*)output -> Get("sbnana");
   vector<int> codes;
   TLeaf* nuEnergy = tree->GetLeaf("interactions.neutrino.energy");
   auto lowE = 10;
   auto highE = 0;
   TLeaf* nuType = tree->GetLeaf("interactions.neutrino.genie_intcode");

   for(int i=0; i<nuEnergy->GetBranch()->GetEntries();i++){
      nuEnergy->GetBranch()->GetEntry(i);
      auto energy = nuEnergy->GetValue();
      if(energy < lowE) lowE = energy; 
      if(energy > highE) highE = energy;
   }
   cout << nuEnergy->GetBranch()->GetEntries()<<endl;
   cout << lowE << "\t" << highE << endl;   

   THStack* parents = new THStack("parents","parentParticles");
   for(int i=0; i<nuType->GetBranch()->GetEntries();i++){
      nuEnergy->GetBranch()->GetEntry(i);
      nuType->GetBranch()->GetEntry(i);
      auto energy = nuEnergy->GetValue(); 
      auto type = nuType->GetValue();
      if(codes.empty()){
         codes.push_back(type);
      }
      else{
         if(find(codes.begin(),codes.end(),type)==codes.end()){
            codes.push_back(type);
         }
      }
   }
   
   vector<TH1D*> hists = vector<TH1D*>();
   
   srand(123456);
   char buffer[20]; 
   for(int i=0;i<codes.size();i++){
      sprintf(buffer, "%s%d","GENIEcode",codes[i]);
      cout << buffer << endl;
      hists.push_back(new TH1D(buffer,"",30,0,6));
      auto num = rand()%12; //number between 0 and 12
      if(num==0) hists[i]->SetFillColor(kOrange);
      else if(num==1) hists[i]->SetFillColor(kRed);
      else if(num==2) hists[i]->SetFillColor(kPink);
      else if(num==3) hists[i]->SetFillColor(kMagenta);
      else if(num==4) hists[i]->SetFillColor(kViolet);
      else if(num==5) hists[i]->SetFillColor(kBlue);
      else if(num==6) hists[i]->SetFillColor(kAzure);
      else if(num==7) hists[i]->SetFillColor(kCyan);
      else if(num==8) hists[i]->SetFillColor(kTeal);
      else if(num==9) hists[i]->SetFillColor(kGreen);
      else if(num==10) hists[i]->SetFillColor(kSpring);
      else if(num==11) hists[i]->SetFillColor(kYellow);
   }

   for(int i=0; i<nuType->GetBranch()->GetEntries();i++){
      nuEnergy->GetBranch()->GetEntry(i);
      nuType->GetBranch()->GetEntry(i);
      auto energy = nuEnergy->GetValue();
      auto type = nuType->GetValue();
      for(size_t n=0;n<codes.size();n++){
         if(type==codes[n]) hists[n]->Fill(energy,1);
      }
   }

   for(int i=0; i<hists.size();i++){
      parents->Add(hists[i]);
   }
   parents->Draw();
   gPad->BuildLegend(0.6, 0.7, 0.99, 0.9,"Parent particles");
}
