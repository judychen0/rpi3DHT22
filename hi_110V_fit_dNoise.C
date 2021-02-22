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
#include <TMath.h>
#include <TMultiGraph.h>


#define Nchannels 128

void hi_110V_fit_dNoise()
{
  TFile *f = new TFile("hi_plot_1.root");
  TH1F *dNoise_his[Nchannels -1];
  TCanvas *c;
  Double_t FitMean[Nchannels], FitStd[Nchannels], FitStd_Norm[Nchannels], ChannelID[Nchannels];
  Double_t sqrt2 = TMath::Sqrt(2);

  c = new TCanvas("c", "c",1200, 900);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetLabelSize(0.03,"xy");
  gStyle->SetLabelFont(42,"xy");

  for(int i=0; i<Nchannels-1; i++)
    {
      dNoise_his[i] = (TH1F*)f->GetDirectory("dNoise_his")->Get(Form("dNoise_ch%i-ch%i", i+2, i+1));
      dNoise_his[i]->Fit("gausn", "Q");

      FitMean[i] = dNoise_his[i]->GetFunction("gausn")->GetParameter(1);
      FitStd[i] = dNoise_his[i]->GetFunction("gausn")->GetParameter(2);
      cout << "FitStd sigma : " << FitStd[i] << endl;
      FitStd_Norm[i] = FitStd[i] / sqrt2;
      cout<< "FitStd  Normsigma"<< i<< ": " << FitStd_Norm[i] << endl;
      
      ChannelID[i] = i;
      
    }
  
  //plot FitMean/FitStd and set graph 
  TGraph *dNoise_Mean = new TGraph(Nchannels-1, ChannelID, FitMean);
  dNoise_Mean->SetName("Noise Sub Mean");
  dNoise_Mean->SetTitle("Noise Sub Mean");
  dNoise_Mean->SetMarkerStyle(20);
  dNoise_Mean->SetMarkerSize(0.8);
  dNoise_Mean->SetLineWidth(0);
  dNoise_Mean->GetXaxis()->SetTitle("ChannelID");
  dNoise_Mean->GetYaxis()->SetTitle("ADC");
  //dNoise_Mean->Draw("AP");
  
  TGraph *dNoise_Std = new TGraph(Nchannels-1, ChannelID,  FitStd);
  dNoise_Std->SetName("Noise Sub Std");
  dNoise_Std->SetTitle("Noise Sub Std");
  dNoise_Std->SetMarkerStyle(20);
  dNoise_Std->SetMarkerSize(0.8);
  dNoise_Std->SetMarkerColor(kBlue);
  dNoise_Std->SetLineWidth(0);
  dNoise_Std->GetXaxis()->SetTitle("ChannelID");
  dNoise_Std->GetYaxis()->SetTitle("ADC");
  //dNoise_Std->Draw("AP");

  TGraph *dNoise_Std_Norm = new TGraph(Nchannels-1, ChannelID, FitStd_Norm);
  dNoise_Std_Norm->SetName("CM correction");
  dNoise_Std_Norm->SetTitle("CM correction");
  dNoise_Std_Norm->SetMarkerStyle(20);
  dNoise_Std_Norm->SetMarkerSize(0.8);
  dNoise_Std_Norm->SetMarkerColor(kRed);
  dNoise_Std_Norm->SetLineWidth(0);
  dNoise_Std_Norm->GetXaxis()->SetTitle("ChannelID");
  dNoise_Std_Norm->GetYaxis()->SetTitle("ADC");
  //dNoise_Std_Norm->Draw("AP");

  TFile *f1 = new TFile("hi_110V_fit.root");
  TGraph *Noise = (TGraph*)f1->Get("Noise");
  Double_t xN[Nchannels], yN[Nchannels];
  for(int i=0; i<Nchannels; i++)
    {
      Noise->GetPoint(i, xN[i], yN[i]);
    }
  TGraph *gN = new TGraph(Nchannels, xN, yN);
  gN->SetName("Without CM correction");
  gN->SetTitle("Without CM correction");
  gN->SetMarkerStyle(20);
  gN->SetMarkerSize(0.8);
  gN->SetMarkerColor(kBlue);
  gN->SetLineWidth(0);
  
  TMultiGraph *mg = new TMultiGraph("mg", "");
  mg->Add(gN);
  mg->Add(dNoise_Std_Norm);
  mg->GetXaxis()->SetTitle("ChannelID");
  mg->GetYaxis()->SetTitle("ADC");
  mg->GetYaxis()->SetRangeUser(0, 8);
  mg->Draw("SAME P");
  c->BuildLegend();
  
 
  //Write and Draw
  TFile *Fit = new TFile("hi_110V_fit_dNoise.root", "RECREATE");
  for(int i=0; i<Nchannels-1; i++)
    {
      dNoise_his[i]->Write();
    }
  dNoise_Mean->Write();
  dNoise_Std->Write();
  dNoise_Std_Norm->Write();
  mg->Write();
  Fit->Close();
}
