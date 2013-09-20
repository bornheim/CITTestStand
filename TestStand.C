void TestStand()
{
  gROOT->Reset();
  std::cout << "here it breaks -1" << std::endl;
  gROOT->ProcessLine("#include <vector>");
  std::cout << "here it breaks -2" << std::endl;
  
  //TFile* f = new TFile("NewScopeData_Sep2_11_49AM.root");
  
  //TFile* f = new TFile("20db_data.root");
  TFile* f = new TFile("Ag_first.root");
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
  TH1F* DQMh1000Ag = new TH1F("DQMh1000Ag", "sample jitter", 1000, 6.24e-12, 6.26e-12 );
  float min2 = 1.0;

  TH1F *MuonPulse[10000];
  Int_t NHisto = 10000;
  Int_t nShapes = 3000;
  char *histname = new char[10];
  for (Int_t k=0; k<NHisto; k++) {
    sprintf(histname, "muon_pulse_%d",k);  
    //cout << histname << endl;
    //MuonPulse[k]=new TH1F(histname,"", 2500, -3.1999999095e-08,4.68000042275e-07);
    MuonPulse[k]=new TH1F(histname,"",35000,-10.7998388899,218.750003-10.7998388899);
  };
  TH1F* MuonPulseAverage = new TH1F("MuonPulseAverage","MuonPulseAverage", 2500,-30.200014181,470.200014181);  

  float amperror;
  float previousTime;
  //for(int i = 0; tree->GetEntries(); i++){
  for(int i = 0; i < nShapes /*tree->GetEntries()*/; i++){
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
      //if(i == 1)h1->Fill(Time->at(j),Amp->at(j));

      if (Amp->at(j) == 0) cout << " zero amplitude " << endl;

      MuonPulse[i]->Fill((Time->at(j))*1e9,-1.0*(Amp->at(j)));

      MuonPulseAverage->Fill((Time->at(j))*1e9,-1.0*(Amp->at(j))); 

      amperror = (Amp->at(j)*Amp->at(j))**0.25;

      //MuonPulse[i]->SetBinError(j,amperror);
      if (i == 1) 
      { 
	if (j>0 && j<20) cout << i << " " << j << " - " << (Time->at(j))*1e9-((Time->at(0))*1e9) << " " << Amp->at(j) << " " << Time->at(j)-previousTime << " " << j*6.251 << endl;
	if (j>34999 && j<35020) cout << i << " " << j << " - " << (Time->at(j))*1e9-((Time->at(0))*1e9) << " " << Amp->at(j) << " " << Time->at(j)-previousTime << " " << j*6.251 << endl;
       //if (j>5190) cout << i << " " << j << " - " << Time->at(j) << " " << Amp->at(j) << " " << Time->at(j)-previousTime << endl;
      }; 
      DQMh1000->Fill(Time->at(j)-previousTime); 
      DQMh1000Ag->Fill(Time->at(j)-previousTime); 
      if(Amp->at(j) < min2)
      {
         min2 = Amp->at(j);
      }
      //if (j<2510) cout << " -- " << Time->at(0) << " " << j << " " << j*(Time->at(j)-previousTime)+Time->at(0) << endl;
      previousTime =  Time->at(j);  
   }
     
    h->Fill(TMath::Abs(min2));
    //if(min2 != 1.0)std::cout << "Min: " << min2 << std::endl;
    
  }
  
      
  h1->Draw();
  TFile* f1 = new TFile("CosmicPulseShapes_test.root","RECREATE");

  for (Int_t ii=0; ii<nShapes;ii++) {
    MuonPulse[ii]->Write();
  };

  MuonPulseAverage->Write();
  DQMh1000->Write();
  DQMh1000Ag->Write();
  f1->Close();
  f->Close();

  return 0;
}
