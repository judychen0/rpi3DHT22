//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jul 31 15:41:07 2020 by ROOT version 6.20/04
// from TTree tree/tree
// found on file: hi_110V.root
//////////////////////////////////////////////////////////

#ifndef hi_110V_h
#define hi_110V_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

//define Nchannels number!!!
#define Nchannels 128

// Headers needed by this particular selector


class hi_110V : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderArray<Int_t> event = {fReader, "event"};
   TTreeReaderArray<Double_t> clock = {fReader, "clock"};
   TTreeReaderArray<Int_t> signal = {fReader, "signal"};
   TTreeReaderArray<Int_t> header = {fReader, "header"};
   TTreeReaderArray<Double_t> temperature = {fReader, "temperature"};
   TTreeReaderArray<Double_t> time = {fReader, "time"};


   hi_110V(TTree * /*tree*/ =0) { }
   virtual ~hi_110V() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(hi_110V,0);

};

#endif

#ifdef hi_110V_cxx
void hi_110V::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t hi_110V::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef hi_110V_cxx
