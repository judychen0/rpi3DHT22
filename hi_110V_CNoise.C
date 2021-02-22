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

void hi_110V_CNoise()
{
  
  TFile *f = new TFile("hi_plot_1.root");
  TH1F *pedestal_his[Nchannels], *CNoise_sub[Nchannels];
  TCanvas *c;
  Double_t FitMean_CN[Nchannels], FitStd_CN[Nchannels], ChannelID[Nchannels];
  TF1 *fit_result_P[Nchannels], *fit_result_CN[Nchannels];
  
  //set canvas 
  c = new TCanvas("c", "c", 1200, 900);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetLabelSize(0.03,"xy");
  gStyle->SetLabelFont(42,"xy");
  
  for(int i=0; i<Nchannels; i++)
    {
      //draw pedestal histograms
      CNoise_sub[i] = (TH1F*)f->GetDirectory("CNoise_sub")->Get(Form("CNoise_sub%i", i+1));
      CNoise_sub[i]->Fit("gausn", "Q");
      fit_result_CN[i] = CNoise_sub[i]->GetFunction("guasn");
      //fit_result_CN[i]->SetLineColor(kRed);
      FitMean_CN[i] = CNoise_sub[i]->GetFunction("gausn")->GetParameter(1);
      FitStd_CN[i] = CNoise_sub[i]->GetFunction("gausn")->GetParameter(2);
      
      ChannelID[i] = i ;
    }
  //plot FitMean_CN/FitStd_CN and set graph 
  TGraph *CN_Mean = new TGraph(Nchannels, ChannelID, FitMean_CN);
  CN_Mean->SetName("Pedestal Mean");
  CN_Mean->SetTitle("PePestal Mean");
  CN_Mean->SetMarkerStyle(20);
  CN_Mean->SetMarkerSize(0.8);
  CN_Mean->SetLineWidth(0);
  CN_Mean->GetXaxis()->SetTitle("ChannelID");
  CN_Mean->GetYaxis()->SetTitle("ADC");
  CN_Mean->Draw("AP");
  
  TGraph *CN_Std = new TGraph(Nchannels, ChannelID,  FitStd_CN);
  CN_Std->SetName("Noise");
  CN_Std->SetTitle("Noise");
  CN_Std->SetMarkerStyle(20);
  CN_Std->SetMarkerSize(0.8);
  CN_Std->SetLineWidth(0);
  CN_Std->GetXaxis()->SetTitle("ChannelID");
  CN_Std->GetYaxis()->SetTitle("ADC");
  CN_Std->Draw("AP");

  TFile *f1 = new TFile("hi_110V_fit.root");//change _fit.root filename HERE!!!
  
  for(int i=0; i<Nchannels; i++)
    {
      pedestal_his[i]=(TH1F*)f1->Get(Form("pedestal_his%i", i+1));
      fit_result_P[i] = (TF1*)pedestal_his[i]->GetFunction("gausn");
      fit_result_P[i]->SetLineColor(kBlue);
      pedestal_his[i]->SetMarkerStyle(20);
      pedestal_his[i]->SetMarkerSize(1);
      pedestal_his[i]->GetYaxis()->SetRangeUser(0, 5000);
      pedestal_his[i]->GetXaxis()->SetTitle("ADC");
      pedestal_his[i]->Draw();

      CNoise_sub[i]->SetTitle(Form("Ch%i", i+1));
      CNoise_sub[i]->SetName(Form("Ch%i", i+1));
      CNoise_sub[i]->SetMarkerStyle(22);
      CNoise_sub[i]->SetMarkerSize(1);
      //CNoise_sub[i]->GetYaxis()->SetRangeUser(0, 2300);
      CNoise_sub[i]->Draw("same");

      TLegend *legend=new TLegend(0.6,0.68, 0.8, 0.88);
      legend->SetBorderSize(0);
      legend->AddEntry(fit_result_P[i],"#scale[1.2]{Pedestal fit}","L");
      legend->AddEntry(fit_result_CN[i],"#scale[1.2]{Pedestal fit with CM Sub}","L");
      legend->AddEntry(pedestal_his[i],"#scale[1.2]{Signal}","P");
      legend->AddEntry(CNoise_sub[i],"#scale[1.2]{CM Sub Signal}","P");
      legend->Draw("SAME");
      
      c->SaveAs(Form("hi_110V_CNoise_superimpose_plot/Ch%i.png", i+1));
    }
  

  //Write and Draw
  TFile *fit = new TFile("hi_110V_CNoise.root", "RECREATE");
  for(int i=0; i<Nchannels; i++)
    {
      CNoise_sub[i]->Write();
    }
  CN_Mean->Write();
  CN_Std->Write();
  fit->Close();
}
