void TestStand()
{
  gROOT->Reset();
  std::cout << "here it breaks -1" << std::endl;
  gROOT->ProcessLine("#include <vector>");
  std::cout << "here it breaks -2" << std::endl;
  
  TFile* f = new TFile("CrystalData.root");
  TTree* tree = (TTree*)f->Get("outTree");
  
  std::cout.precision(12);
  
  double ptime;
  std::vector<float> *Amp;
  std::vector<float> *Time;
  TBranch *BTime = 0;
  TBranch *BAmp = 0;

  //tree->SetBranchAddress("ptime", &ptime);
  //tree->SetBranchAddress("Amp", &Amp, &BAmp);
  //tree->SetBranchAddress("Time", &Time, &BTime);
  
  tree->SetBranchAddress("ptime", &ptime);
  tree->SetBranchAddress("Amp", &Amp);
  tree->SetBranchAddress("Time", &Time);
  
  TH1F* h = new TH1F("h", "muon spectrum", 1000, .0, .5);
  TH1F* h1 = new TH1F("h1", "Pulse Output1", 2500, -3.1999999095e-08,4.68000042275e-07);
  TH1F* h2 = new TH1F("h2", "Pulse Output1", 2500, -3.1999999095e-08,4.68000042275e-07);
  TH1F* h109 = new TH1F("h109", "Pulse Output2", 2500, -3.1999999095e-08,4.68000042275e-07);
  TH1F* h267 = new TH1F("h267", "Pulse Output3", 2500, -3.1999999095e-08,4.68000042275e-07);
  TH1F* DQMh1000 = new TH1F("DQMh1000", "sample jitter", 1000, 1.999e-10, 2.001e-10 );
  float min2 = 1.0;

  TH1F *MuonPulse[10000];
  Int_t NHisto = 10000;
  char *histname = new char[10];
  for (Int_t k=0; k<NHisto; k++) {
    sprintf(histname, "muon_pulse_%d",k);  
    //cout << histname << endl;
    //MuonPulse[k]=new TH1F(histname,"", 2500, -3.1999999095e-08,4.68000042275e-07);
    MuonPulse[k]=new TH1F(histname,"", 2500,-30.1999999095,468.000042275);
  };
  
  float amperror;
  float previousTime;
  //for(int i = 0; i < 10/*tree->GetEntries()*/; i++){
  for(int i = 0; i < 9000 /*tree->GetEntries()*/; i++){
    tree->GetEntry(i);
    //BTime->GetEntry(i);
    //BAmp->GetEntry(i);
    //std::cout << "here it breaks!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    cout << " Processing event " << i << endl; 
    float min = 1.0;
    min2 = 1.0;
    //int size = Time->size();
    //std::cout << "vector size: " << Amp->size() << " Time: " << Time->at(0) << " " << Time->at(1) << " " << Time->at(2) << " " << Time->at(size-1) << std::endl;
    //std::cout << " Time: " << Time->at(0)-Time->at(1) << " " << Time->at(1)-Time->at(2) << " " << Time->at(2)-Time->at(3) << " " << Time->at(size-1) << std::endl;
    //std::cout << "time stamp: " << ptime << std::endl;
    for(int j = 0; j < Amp->size(); j++){
      if(i == 1)h1->Fill(Time->at(j),Amp->at(j));
      if(i == 2)h2->Fill(Time->at(j),Amp->at(j));  
 
      MuonPulse[i]->Fill((Time->at(j))*1e9,-1.0*(Amp->at(j)));

      amperror = (Amp->at(j)*Amp->at(j))**0.25;
      //MuonPulse[i]->SetBinError(j,amperror);
      //cout << i << " " << j << " - " << Time->at(j) << " " << Amp->at(j) << " " << Time->at(j)-previousTime << endl;
      DQMh1000->Fill(Time->at(j)-previousTime); 
      if(i == 109)h109->Fill(Time->at(j),Amp->at(j));
      if(i == 267)h267->Fill(Time->at(j),Amp->at(j));
      if(Amp->at(j) < min2){
         min2 = Amp->at(j);
      }
      //if (j<2510) cout << " -- " << Time->at(0) << " " << j << " " << j*(Time->at(j)-previousTime)+Time->at(0) << endl;
      previousTime =  Time->at(j);  
   }
    
    h->Fill(TMath::Abs(min2));
    //if(min2 != 1.0)std::cout << "Min: " << min2 << std::endl;
    
  }
  
      
  h1->Draw();
  TFile* f1 = new TFile("test2.root","RECREATE");
  h->Write();
  h1->Write();
  h2->Write();
  h109->Write();
  h267->Write();

  for (Int_t ii=0; ii<NHisto;ii++) {
    MuonPulse[ii]->Write();
  };


  DQMh1000->Write();
  f1->Close();
  f->Close();

  return 0;
}
