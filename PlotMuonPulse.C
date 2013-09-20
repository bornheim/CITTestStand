void PlotMuonPulse()
{

  //TFile *file0 = TFile::Open("CosmicPulseShapes.root");

tampMinPeak = 12;
tampMaxPeak = 28;

TFile *file0 = TFile::Open("CosmicPulseShapes_Ag1.root");
TH1F *MuonPulse[10000];
TH1F *peakHist = new TH1F("peakHist","peakHist",1000,tampMinPeak,tampMaxPeak);
TH1F *baseline = new TH1F("baseline","baseline",1000,-0.1,0.1);
TH1F *peakCorHist = new TH1F("peakCorHist","peakCorHist",1000,tampMinPeak,tampMaxPeak);
TH1F *ampHist = new TH1F("ampHist","ampHist",1000,0,20);
TH1F *riseHist = new TH1F("riseHist","riseHist",1000,0,10);
TH1F *halfMaxHist = new TH1F("halfMaxHist","halfMaxHist",1000,tampMinPeak,tampMaxPeak);
TH1F *halfMaxCorHist = new TH1F("halfMaxCorHist","halfMaxCorHist",1000,tampMinPeak,tampMaxPeak);
TH1F *halfMaxCor2Hist = new TH1F("halfMaxCor2Hist","halfMaxCor2Hist",1000,tampMinPeak,tampMaxPeak);
TH2F *ampVShalfMaxCorHist = new TH2F("ampVShalfMaxCorHist","ampVShalfMaxCorHist",100,0,2,1000,tampMinPeak,tampMaxPeak);
//
TH2F *peakVShalfMaxHist = new TH2F("peakVShalfMaxHist","peakVShalfMaxHist",1000,tampMinPeak,tampMaxPeak,1000,tampMinPeak,tampMaxPeak);
TH2F *peakVSDeltahalfMaxHist = new TH2F("peakVSDeltahalfMaxHist","peakVSDeltahalfMaxHist",1000,tampMinPeak,tampMaxPeak,1000,-5,5);
TH2F *peakVSDeltahalfMaxCorHist = new TH2F("peakVSDeltahalfMaxCorHist","peakVSDeltahalfMaxCorHist",1000,tampMinPeak,tampMaxPeak,1000,-5,5);
TH1F *DeltahalfMaxHist = new TH1F("DeltahalfMaxHist","DeltahalfMaxHist",1000,-5,5);
TH1F *DeltahalfMaxCorHist = new TH1F("DeltahalfMaxCorHist","DeltahalfMaxCorHist",300,-3,3);
//
TH2F *peakVSriseHist = new TH2F("peakVSriseHist","peakVSriseHist",1000,tampMinPeak,tampMaxPeak,1000,0,1);
TH2F *halfMaxVSriseHist = new TH2F("halfMaxVSriseHist","halfMaxVSriseHist",1000,tampMinPeak,tampMaxPeak,1000,0,1);
TH2F *riseVShalfMaxHist = new TH2F("riseVShalfMaxHist","riseVShalfMaxHist",100,0,1,1000,tampMinPeak,tampMaxPeak);
TH2F *peakVSampHist = new TH2F("peakVSampHist","peakVSampHist",1000,tampMinPeak,tampMaxPeak,1000,0,2);
TH2F *peakVSampCorHist = new TH2F("peakVSampCorHist","peakVSampCorHist",1000,tampMinPeak,tampMaxPeak,1000,0,3);
TH2F *ampVSpeakHist = new TH2F("ampVSpeakHist","amptVSpeakHist",100,0,3,1000,tampMinPeak,tampMaxPeak);
TH2F *ampVShalfMaxHist = new TH2F("ampVShalfMaxHist","amptVShalfMaxHist",100,0,3,1000,tampMinPeak,tampMaxPeak);
TH2F *halfMaxVSiHist = new TH2F("halfMaxVSiHist","halfMaxVSiHist",1000,0,9000,1000,tampMinPeak,tampMinPeak);
char *histname = new char[10];
float peak;
float peakLow;
float peakHigh;
float amp;
float ampCor;
float slope; 
float slopeLow;
float slopeHigh;
float slopeFit;
float offsetFit;
float halfMax;    
float halfMaxCor;
float halfMaxCor2;
float peakCor;
float halfMaxBaseCor;
float time;
float DeltaT;
float DeltaTCor;
float BaseLine;
float meanTime;

 Int_t nHistos = 1540;

TCanvas *MuonPulsePlot = new TCanvas("MuonPulsePlot", "MuonPulsePlot",20,40,800,600);
MuonPulsePlot->cd();
muon_pulse_1->GetYaxis()->SetRangeUser(-0.05,2);
muon_pulse_1->Draw();

TCanvas *MuonPulseFit = new TCanvas("MuonPulseFit", "MuonPulseFit",25,45,800,600);

for(int i = 0; i < nHistos; i++)
 {
  MuonPulseFit->cd();
  sprintf(histname, "muon_pulse_%d",i);  
  MuonPulse[i] = (TH1F*)file0->Get(histname);
  meanTime = MuonPulse[i]->GetMean();
  //cout << meanTime << endl;
  MuonPulse[i]->Fit("gaus","Q","",18.0,22.0);
  MuonPulse[i]->Fit("landau","Q","",17.0,23.0);
  peak = landau->GetParameter(1);
  //cout << i << " == peak : " << peak;
  peakLow = peak -1.5;
  peakHigh = peak + 1.0;
  MuonPulse[i]->Fit("landau","Q","",peakLow,peakHigh);
  peak = landau->GetParameter(1);
  //cout << "  peak iterated : " << peak;
  peakHist -> Fill(peak);

  MuonPulse[i]->Fit("gaus","Q","",peakLow,peakHigh);
  amp = gaus->GetParameter(0);
  //cout << "  amp : " << amp;
  //cout << endl;
  ampHist -> Fill(amp);
  peakVSampHist -> Fill(peak,amp);
  ampVSpeakHist -> Fill(amp,peak);
  // v2   peakCor=peak+1.097*amp;
  peakCor=peak+1.666*amp;
  if (amp>0.8) peakCorHist -> Fill(peakCor);
  slopeLow = peakLow-2.4;
  slopeHigh = peakLow-1.5; 
  if (slopeLow>0.25 && amp>0.7 && amp<100.5 && peak >21.0 && peak<23.0)
  {
    MuonPulse[i]->Fit("pol1","Q","",slopeLow,slopeHigh);
    
    slopeFit = pol1->GetParameter(1);
    if (slopeFit>0.0 && slopeFit<10.40)
    {
      offsetFit = pol1->GetParameter(0);
     halfMax = (amp/2.0-offsetFit)/slopeFit; 
      //offsetFit = 0.0;
     //cout << " slope fit : " << slopeLow << " - " << slopeHigh << "   slope " << slopeFit << "  offset " << offsetFit << "  halfMax " << halfMax << endl;     
     riseHist -> Fill(slopeFit);
     halfMaxHist -> Fill(halfMax);

     MuonPulse[i]->Fit("pol0","Q","",-25.0,10.0);
     BaseLine = pol0->GetParameter(0);   
     baseline -> Fill(BaseLine);
     halfMaxBaseCor = ((amp-BaseLine)/2.0-offsetFit)/slopeFit; 

     halfMax = halfMaxBaseCor;
     ampVShalfMaxHist -> Fill(amp,halfMax);
     time = 1.0*i;

     halfMaxVSiHist -> Fill(time,halfMax);
     peakVShalfMaxHist -> Fill(peak,halfMax);
     peakVSriseHist -> Fill(peak,slopeFit);
     halfMaxVSriseHist -> Fill(halfMax,slopeFit);
     riseVShalfMaxHist -> Fill(slopeFit,halfMax);
//
//
// correct for a rise time dependence 
     halfMaxCor = halfMax + 3.95*slopeFit;
     halfMaxCorHist -> Fill(halfMaxCor); 
     ampVShalfMaxCorHist -> Fill(amp,halfMaxCor);
//
//
//
     halfMaxCor2 = (halfMax + 3.95*slopeFit)-0.6227*amp; 
     halfMaxCor2Hist -> Fill(halfMaxCor2); 
     peakVSampHist -> Fill(peak,amp);
     peakVSDeltahalfMaxHist -> Fill(peak,halfMax-peak);
     DeltaT = halfMax-peak;
    //DeltaTCor = DeltaT+3.994-1.065*peak;
    //DeltaTCor = DeltaT-67.1+4.613*peak-0.07621*peak**2;
     DeltaTCor = DeltaT-1.784+0.2176*peak;
     peakVSDeltahalfMaxHist -> Fill(peak,halfMax-peak);
     peakVSDeltahalfMaxCorHist -> Fill(peak,DeltaTCor);
     DeltahalfMaxHist -> Fill(DeltaT);
     DeltahalfMaxCorHist -> Fill(DeltaTCor);
   };
  }; 


  MuonPulsePlot->cd();
  //MuonPulse[i]->Draw("same");
  //peakHist -> Draw();

 };



TCanvas *MuonPulseFitResult = new TCanvas("MuonPulseFitResult", "MuonPulseFitResult",30,50,800,600);
MuonPulseFitResult->Divide(2,2);
MuonPulseFitResult->cd(1);
// v2 peakHist->Fit("gaus","","",20,22);
peakHist->Fit("gaus","","",21,23);
//peakCorHist->Fit("gaus","","",20,22);
peakCorHist->Fit("gaus","","",22.5,24);
peakHist->Draw();
MuonPulseFitResult->cd(2);
halfMaxHist->Fit("gaus","","",17,19);
halfMaxHist->Draw();
MuonPulseFitResult->cd(3);
peakVSDeltahalfMaxHist -> Draw();
MuonPulseFitResult->cd(4);
DeltahalfMaxCorHist -> Fit("gaus","","",-0.3,0.3);
DeltahalfMaxCorHist ->Draw();

TCanvas *MuonPulseFitResult2 = new TCanvas("MuonPulseFitResult2", "MuonPulseFitResult2",40,460,800,600);
MuonPulseFitResult2->Divide(2,2);
MuonPulseFitResult2->cd(1);
peakCorHist->Fit("gaus","","",21,23);

MuonPulseFitResult2->cd(2);
halfMaxCorHist->Fit("gaus","","",18.75,20);

MuonPulseFitResult2->cd(3);
halfMaxCor2Hist->Fit("gaus","","",18.00,19);


}


