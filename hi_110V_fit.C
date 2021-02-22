#include <TCanvas.h>
#include <TF1.h>
#include <TH1F.h>
#include <TFile.h>
#include <TStyle.h>
#include <TFormula.h>
#include <TAttLine.h>
#include <TAttMarker.h>
#include <TAttFill.h>
#include <TLegend.h>
#include <TPaveText.h>

#define Nchannels 128

void hi_110V_fit()
{
  //change the pedestal_his filename HERE!!!
  TFile *f = new TFile("hi_plot_1.root");
  //set open his variable name
  TH1F *pedestal_his[Nchannels];
  TCanvas *c;
  //set fitting variables
  Double_t FitMean[Nchannels], FitStd[Nchannels], ChannelID[Nchannels];
  
  //set canvas 
  c = new TCanvas("c", "c",1200, 900);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(111);
  gStyle->SetLabelSize(0.03,"xy");
  gStyle->SetLabelFont(42,"xy");
  
  for(int i=0; i<Nchannels; i++)
    {
      //draw pedestal histograms
      pedestal_his[i] = (TH1F*)f->GetDirectory("pedestal_his")->Get(Form("pedestal_his%i", i+1));
      pedestal_his[i]->Fit("gausn", "Q");

      FitMean[i] = pedestal_his[i]->GetFunction("gausn")->GetParameter(1);
      FitStd[i] = pedestal_his[i]->GetFunction("gausn")->GetParameter(2);
      cout<< "noise"<< i << " " << FitStd[i] << endl;
      ChannelID[i] = i ;
    }

  
  

  //plot FitMean/FitStd and set graph 
  TGraph *P_Mean = new TGraph(Nchannels, ChannelID, FitMean);
  P_Mean->SetName("Pedestal Mean");
  P_Mean->SetTitle("Pedestal Mean");
  P_Mean->SetMarkerStyle(20);
  P_Mean->SetMarkerSize(0.8);
  P_Mean->SetLineWidth(0);
  P_Mean->GetXaxis()->SetTitle("ChannelID");
  P_Mean->GetYaxis()->SetTitle("ADC");
  P_Mean->Draw("AP");
  
  TGraph *P_Std = new TGraph(Nchannels, ChannelID,  FitStd);
  P_Std->SetName("Noise");
  P_Std->SetTitle("Noise");
  P_Std->SetMarkerStyle(20);
  P_Std->SetMarkerSize(0.8);
  P_Std->SetLineWidth(0);
  P_Std->GetXaxis()->SetTitle("ChannelID");
  P_Std->GetYaxis()->SetTitle("ADC");
  P_Std->Draw("AP");

  //Write and Draw
  //Change savefile name HERE!!!
  //personally suggest to use the same filename as this .C file
  TFile *Fit = new TFile("hi_110V_fit.root", "RECREATE");
  for(int i=0; i<Nchannels; i++)
    {
      pedestal_his[i]->Write();
    }
  P_Mean->Write();
  P_Std->Write();
  Fit->Close();
}
