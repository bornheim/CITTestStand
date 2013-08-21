void PlotMuonPulse()
{

TFile *file0 = TFile::Open("CosmicPulseShapes.root");
TH1F *MuonPulse[10000];
TH1F *peakHist = new TH1F("peakHist","peakHist",1000,20,34);
TH1F *ampHist = new TH1F("ampHist","ampHist",1000,0,20);
TH1F *riseHist = new TH1F("riseHist","riseHist",1000,0,10);
TH1F *halfMaxHist = new TH1F("halfMaxHist","halfMaxHist",1000,10,32);
//
TH2F *peakVShalfMaxHist = new TH2F("peakVShalfMaxHist","peakVShalfMaxHist",1000,22,34,1000,18,32);
TH2F *peakVSDeltahalfMaxHist = new TH2F("peakVSDeltahalfMaxHist","peakVSDeltahalfMaxHist",1000,22,34,1000,-5,5);
TH2F *peakVSDeltahalfMaxCorHist = new TH2F("peakVSDeltahalfMaxCorHist","peakVSDeltahalfMaxCorHist",1000,22,34,1000,-5,5);
TH1F *DeltahalfMaxHist = new TH1F("DeltahalfMaxHist","DeltahalfMaxHist",1000,-5,5);
TH1F *DeltahalfMaxCorHist = new TH1F("DeltahalfMaxCorHist","DeltahalfMaxCorHist",300,-3,3);
//
TH2F *peakVSriseHist = new TH2F("peakVSriseHist","peakVSriseHist",1000,20,34,1000,0,6);
TH2F *peakVSampHist = new TH2F("peakVSampHist","peakVSampHist",1000,20,34,1000,0,20);
TH2F *halfMaxVSiHist = new TH2F("halfMaxVSiHist","halfMaxVSiHist",1000,0,9000,1000,20,32);
char *histname = new char[10];
float peak;
float peakLow;
float peakHigh;
float amp;
float slope; 
float slopeLow;
float slopeHigh;
float slopeFit;
float offsetFit;
float halfMax;    
float time;
float DeltaT;
float DeltaTCor;
muon_pulse_1->Draw();

for(int i = 0; i < 9000; i++)
 {
  sprintf(histname, "muon_pulse_%d",i);  
  MuonPulse[i] = (TH1F*)file0->Get(histname);
  MuonPulse[i]->Fit("gaus","Q","",22.0,31.0);
  peak = gaus->GetParameter(1);
  cout << i << " == " << peak << endl;
  peakLow = peak -1.0;
  peakHigh = peak + 2.0;
  MuonPulse[i]->Fit("gaus","Q","",peakLow,peakHigh);
  peak = gaus->GetParameter(1);
  cout << peak << endl;
  peakHist -> Fill(peak);
  amp = gaus->GetParameter(0);
  cout << amp << endl;
  cout << endl;
  ampHist -> Fill(amp);
  peakVSampHist -> Fill(peak,amp);
  slopeLow = peakLow-2.5;
  slopeHigh = peakLow-0.5; 
  if (slopeLow>10 && amp>10 && amp<15 && peak >26)
  {
    MuonPulse[i]->Fit("pol1","Q","",slopeLow,slopeHigh);
    slopeFit = pol1->GetParameter(1);
    offsetFit = pol1->GetParameter(0);
    halfMax = (amp/2.0-offsetFit)/slopeFit; 
    riseHist -> Fill(slopeFit);
    halfMaxHist -> Fill(halfMax);
    time = 1.0*i;
    
    halfMaxVSiHist -> Fill(time,halfMax);
    peakVShalfMaxHist -> Fill(peak,halfMax);
    peakVSDeltahalfMaxHist -> Fill(peak,halfMax-peak);
    DeltaT = halfMax-peak;
    DeltaTCor = DeltaT-67.1+4.613*peak-0.07621*peak**2;
    peakVSDeltahalfMaxHist -> Fill(peak,halfMax-peak);
    peakVSDeltahalfMaxCorHist -> Fill(peak,DeltaTCor);
    DeltahalfMaxHist -> Fill(DeltaT);
    DeltahalfMaxCorHist -> Fill(DeltaTCor);
  }; 
  MuonPulse[i]->Draw("same");
  peakHist -> Draw();
 };

}


