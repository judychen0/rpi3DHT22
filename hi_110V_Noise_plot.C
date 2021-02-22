#include <TCanvas.h>
#include <TFile.h>
#include <TStyle.h>
#include <TAttLine.h>
#include <TAttMarker.h>
#include <TAttFill.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TMultiGraph.h>

#define Nchannels 128
Double_t xN[Nchannels], yN[Nchannels], xCN[Nchannels], yCN[Nchannels];


void hi_110V_Noise_plot()
{
  TFile *f1 = new TFile("hi_110V_fit.root");
  TGraph *Noise = (TGraph*)f1->Get("Noise");

  TFile *f2 = new TFile("hi_110V_CNoise.root");
  
  TGraph *CNoise = (TGraph*)f2->Get("Noise");

  for(int i=0; i<Nchannels; i++)
    {
      xN[i]=0.0;
      yN[i]=0.0;
      xCN[i]=0.0;
      yCN[i]=0.0;
      Noise->GetPoint(i, xN[i], yN[i]);
      CNoise->GetPoint(i, xCN[i], yCN[i]);
      cout<< xN[i] <<" "<< yN[i] << endl;
      cout<< xCN[i] <<" "<< yCN[i] << endl;
    }

  TCanvas *c = new TCanvas("c", "c", 1200, 900);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetLabelSize(0.03,"xy");
  gStyle->SetLabelFont(42,"xy");

  TGraph *gN = new TGraph(Nchannels, xN, yN);
  gN->SetName("Without CM correction");
  gN->SetTitle("Without CM correction");
  gN->SetMarkerStyle(20);
  gN->SetMarkerSize(0.8);
  gN->SetMarkerColor(kBlue);
  gN->SetLineWidth(0);


  TGraph *gCN = new TGraph(Nchannels, xCN, yCN);
  gCN->SetName("CM correction");
  gCN->SetTitle("CM correction");
  gCN->SetMarkerStyle(20);
  gCN->SetMarkerSize(0.8);
  gCN->SetMarkerColor(kRed);
  gCN->SetLineWidth(0);

  TMultiGraph *mg = new TMultiGraph("mg", "mg");
  mg->Add(gN);
  mg->Add(gCN);
  mg->GetXaxis()->SetTitle("ChannelID");
  mg->GetYaxis()->SetTitle("ADC");
  mg->GetYaxis()->SetRangeUser(0, 8);
  mg->Draw("AP");
  c->BuildLegend();
}
