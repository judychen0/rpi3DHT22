#define hi_110V_cxx
// The class definition in hi_110V.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("hi_110V.C")
// root> T->Process("hi_110V.C","some options")

// root> T->Process("hi_110V.C+")
//


#include "hi_110V.h"
//include the header files 
#include <TH1.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TString.h>
#include <TFile.h>
#include <TMath.h>
#include <TCanvas.h>

//set variables
TFile *outfile;
TH1F *pedestal_his[Nchannels], *CNoise_event_his;
TH1F *pedestal_sub[Nchannels], *CNoise_sub[Nchannels], *CNoise_local_sub[Nchannels];
TH1F *Ped_CN_sub[Nchannels];
TH1F *dNoise_his[Nchannels], *aNoise_his[Nchannels];
TFile *f;
TGraph *P_Mean;
Double_t x_p[Nchannels]={.0}, y_p[Nchannels]={.0}, x_n[Nchannels]={.0}, y_n[Nchannels]={.0};
Double_t signal_CNoise[Nchannels]={.0}, signal_CNoise_avg = .0, signal_CNoise_local_avg  = .0, signal_CNoise_sub[Nchannels]={.0}, signal_CNoise_local_sub[Nchannels]={.0}, signal_Ped_CNoise_sub[Nchannels]={.0};
Double_t dNoise[Nchannels], aNoise[Nchannels];
TCanvas *c;

void hi_110V::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   //set canvas
   c = new TCanvas("c","c",1200, 900);
   gStyle->SetOptTitle(0);
   gStyle->SetOptStat(0);
   gStyle->SetOptFit(0);
   gStyle->SetLabelSize(0.03,"xy");
   gStyle->SetLabelFont(42,"xy");

   //open Pedestal Mean graph in hi_110V_fit.root(in 2ne round!!!)
   TFile *f = new TFile("hi_110V_fit.root");
   TGraph *P_Mean = (TGraph*)f->Get("Pedestal Mean");
   TGraph *Noise = (TGraph*)f->Get("Noise");
   for(int i=0; i<Nchannels; i++)
     {
       P_Mean->GetPoint(i, x_p[i], y_p[i]);
       Noise->GetPoint(i, x_n[i], y_n[i]);
       //cout<< "pedestal ch"<< i <<" : " << y_p[i] << endl;
     }

   //Create root file to store pedestal and fitting plot
   //change rootfile name HERE
   outfile = new TFile("hi_plot_1.root","RECREATE");

   cout << "output file = " <<  "hi_plot" << endl << endl;
   
   //Create histograms
   for(int i=0; i<Nchannels; i++)
     {
       pedestal_his[i] = new TH1F(Form("pedestal_his%i", i+1), Form("Pedestal Ch%i", i+1), 400, 399.5, 800.5);
       pedestal_his[i]->Sumw2();

       //2nd round
       pedestal_sub[i] = new TH1F(Form("pedestal_sub%i", i+1), Form("Pedestal sub Ch%i", i+1), 400, -101.5, 300.5);
       pedestal_sub[i]->Sumw2();

       CNoise_sub[i] = new TH1F(Form("CNoise_sub%i", i+1), Form("Common Noise Subtraction Ch%i", i+1), 400, 399.5, 800.5);
       CNoise_sub[i]->Sumw2();

       CNoise_local_sub[i] = new TH1F(Form("CNoise_local_sub%i", i+1), Form("Common Noise Subtraction (local sub) Ch%i", i+1), 400, -101.5, 300.5);
       CNoise_local_sub[i]->Sumw2();

       Ped_CN_sub[i] = new TH1F(Form("PedCN_sub%i", i+1), Form("Pedestal and Common Noise sub Ch%i", i+1), 400, -101.5, 300.5);
       Ped_CN_sub[i]->Sumw2();

       //signal subtraction with neighbor channal
       dNoise_his[i] = new TH1F(Form("dNoise_ch%i-ch%i", i+2, i+1), Form("dNoise_ch%i-ch%i", i+2, i+1), 1000, -50, 50);
       dNoise_his[i]->Sumw2();
     }

   CNoise_event_his = new TH1F("Common Noise", "Common Noise Event by Event", 10000, 0, 10000); 
   CNoise_event_his->Sumw2();

   cout<< "CNoise original :" << signal_CNoise_avg << endl;
  
}

void hi_110V::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t hi_110V::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   fReader.SetLocalEntry(entry);

   //fill pedestal_his & pedestal sub & CNoise event by event histograms
   signal_CNoise_avg = 0.0;
   for(int i=0; i<Nchannels; i++)
     {
       pedestal_his[i]->Fill(signal[i]);

       //2nd round
       signal_CNoise[i] = signal[i] - y_p[i];
       pedestal_sub[i]->Fill(signal_CNoise[i]);
       //cout << "signal_CNoise ch"<< i <<" "<< signal_CNoise[i] << endl;
       signal_CNoise_avg += (signal_CNoise[i] / Nchannels);
       
     }
   CNoise_event_his->Fill(entry, signal_CNoise_avg);
   
   //fill CNoise sub & Ped_CN_sub histograms
   for(int i=0; i<Nchannels; i++)
     {
       signal_CNoise_sub[i] = signal[i] - signal_CNoise_avg;
       CNoise_sub[i]->Fill(signal_CNoise_sub[i]);
       signal_Ped_CNoise_sub[i] = signal_CNoise_sub[i] - y_p[i];
       Ped_CN_sub[i]->Fill(signal_Ped_CNoise_sub[i]);
     }

   //fill dNoise histograms
   for(int i=0; i<Nchannels-1; i++)
     {
       dNoise[i] = signal_CNoise[i+1]- signal_CNoise[i];
       dNoise_his[i]->Fill(dNoise[i]);
     }

   //optimized 1 method by local avg
   for(int i=2; i<Nchannels-2; i++)
     {
       signal_CNoise_local_avg = 0.0;
       signal_CNoise_local_avg = (signal_CNoise[i-2] + signal_CNoise[i-1] + signal_CNoise[i] + signal_CNoise[i+2] + signal_CNoise[i+2]) / 5;
       //cout << "signal CNoise_local_" << i << " : " <<signal_CNoise_local_avg << endl;
       signal_CNoise_local_sub[i] = signal[i] - signal_CNoise_local_avg;
       //cout << "local sub : " << signal_CNoise_local_sub[i] << endl; 
       CNoise_local_sub[i]->Fill(signal_CNoise_local_sub[i] - y_p[i]);
     }
   
   

   return kTRUE;
}

void hi_110V::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void hi_110V::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

  //Draw histogram and save file
  TDirectory *d1 = outfile->mkdir("pedestal_his");
  TDirectory *d2 = outfile->mkdir("pedestal_sub");
  TDirectory *d3 = outfile->mkdir("CNoise_sub");
  TDirectory *d4 = outfile->mkdir("PedCN_sub");
  TDirectory *d5 = outfile->mkdir("dNoise_his");
  TDirectory *d6 = outfile->mkdir("CNoise_local_sub");

   for(int i=0; i<Nchannels; i++)
    {
      d1->cd();
      pedestal_his[i]->Write();

      
      d2->cd();
      pedestal_sub[i]->Write();

      d3->cd();
      CNoise_sub[i]->Write();

      d4->cd();
      Ped_CN_sub[i]->Write();
     
      d5->cd();
      dNoise_his[i]->Write();

      d6->cd();
      CNoise_local_sub[i]->Write();
    }
   
   //draw & write CNoise_event_his
   outfile->cd();
   CNoise_event_his->GetXaxis()->SetTitle("Event Number");
   CNoise_event_his->GetYaxis()->SetTitle("ADC");
   CNoise_event_his->Draw("hist");
   c->SaveAs("CNoise_event_his.png");
   CNoise_event_his->Write();

   outfile->Close();
}
