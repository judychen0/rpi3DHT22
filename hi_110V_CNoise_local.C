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

void hi_110V_CNoise_local()
{
  
  TFile *f = new TFile("hi_plot_1.root");
  TH1F *pedestal_sub[Nchannels], *PedCN_sub[Nchannels];
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
  
  for(int i=2; i<Nchannels-2; i++)
    {
      //draw pedestal histograms
      PedCN_sub[i] = (TH1F*)f->GetDirectory("CNoise_local_sub")->Get(Form("CNoise_local_sub%i", i+1));
      PedCN_sub[i]->Fit("gausn", "Q");
      fit_result_CN[i] = PedCN_sub[i]->GetFunction("guasn");
      //fit_result_CN[i]->SetLineColor(kGreen);
      FitMean_CN[i] = PedCN_sub[i]->GetFunction("gausn")->GetParameter(1);
      FitStd_CN[i] = PedCN_sub[i]->GetFunction("gausn")->GetParameter(2);
      cout << "FitStd" << i << ": " << FitStd_CN[i]  << endl;

      
      pedestal_sub[i] = (TH1F*)f->GetDirectory("pedestal_sub")->Get(Form("pedestal_sub%i", i+1));
      pedestal_sub[i]->Fit("gausn", "Q");
      //pedestal_sub[i] = (TH1F*)f->Get(Form("pedestal_sub%i", i+1));
      fit_result_P[i] = (TF1*)pedestal_sub[i]->GetFunction("gausn");
      fit_result_P[i]->SetLineColor(kBlue);
      
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
  
  for(int i=2; i<Nchannels-2; i++)
    {
      pedestal_sub[i]->SetMarkerStyle(20);
      pedestal_sub[i]->SetMarkerSize(1);
      pedestal_sub[i]->GetYaxis()->SetRangeUser(0, 1600);
      pedestal_sub[i]->GetXaxis()->SetTitle("ADC");
      pedestal_sub[i]->Draw();

      PedCN_sub[i]->SetTitle(Form("Ch%i", i+1));
      PedCN_sub[i]->SetName(Form("Ch%i", i+1));
      PedCN_sub[i]->SetMarkerStyle(22);
      PedCN_sub[i]->SetMarkerSize(1);
      //PedCN_sub[i]->GetYaxis()->SetRangeUser(0, 2300);
      PedCN_sub[i]->Draw("same");

      TLegend *legend=new TLegend(0.6,0.68, 0.8, 0.88);
      legend->SetBorderSize(0);
      legend->AddEntry(fit_result_P[i],"#scale[1.2]{Fit without CM Sub}","L");
      legend->AddEntry(fit_result_CN[i],"#scale[1.2]{Fit with CM Sub}","L");
      legend->AddEntry(pedestal_sub[i],"#scale[1.2]{Signal}","P");
      legend->AddEntry(PedCN_sub[i],"#scale[1.2]{CM Sub Signal}","P");
      legend->Draw("SAME");
      
      c->SaveAs(Form("hi_110V_CNoise_local_superimpose_plot/Ch%i.png", i+1));
    }
  

  //Write and Draw
  TFile *fit = new TFile("hi_110V_CNoise_local.root", "RECREATE");
  for(int i=2; i<Nchannels-2; i++)
    {
      PedCN_sub[i]->Write();
    }
  CN_Mean->Write();
  CN_Std->Write();
  fit->Close();
}
