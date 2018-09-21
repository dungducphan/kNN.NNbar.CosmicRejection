#ifndef AccessTree_h
#define AccessTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>

class AccessTree {
public :
  TTree *fChain;
  Int_t fCurrent;

  UInt_t RunNumber;
  UInt_t SubrunNumber;
  UInt_t EventNumber;
  UInt_t SliceNumber;
  Double_t TotalVisibleEnergy;
  Double_t TotalHitCountInXView;
  Double_t TotalHitCountInYView;
  Double_t HitCountXYDifference;
  Double_t TemporalClusterExpand;
  Double_t AverageEnergyPerHitXView;
  Double_t AverageEnergyPerHitYView;
  Double_t CorrelationTCellXView;
  Double_t CorrelationTPlaneXView;
  Double_t CorrelationTCellYView;
  Double_t CorrelationTPlaneYView;
  Double_t ConvexHullAreaXV;
  Double_t ConvexHullAreaYV;
  Double_t MinimalEnclosingCircleAreaXV;
  Double_t MinimalEnclosingCircleAreaYV;
  Double_t AverageYposition;

  TBranch *b_RunNumber;
  TBranch *b_SubrunNumber;
  TBranch *b_EventNumber;
  TBranch *b_SliceNumber;
  TBranch *b_TotalVisibleEnergy;
  TBranch *b_TotalHitCountInXView;
  TBranch *b_TotalHitCountInYView;
  TBranch *b_HitCountXYDifference;
  TBranch *b_TemporalClusterExpand;
  TBranch *b_AverageEnergyPerHitXView;
  TBranch *b_AverageEnergyPerHitYView;
  TBranch *b_CorrelationTCellXView;
  TBranch *b_CorrelationTPlaneXView;
  TBranch *b_CorrelationTCellYView;
  TBranch *b_CorrelationTPlaneYView;
  TBranch *b_ConvexHullAreaXV;
  TBranch *b_ConvexHullAreaYV;
  TBranch *b_MinimalEnclosingCircleAreaXV;
  TBranch *b_MinimalEnclosingCircleAreaYV;
  TBranch *b_AverageYposition;

  AccessTree(std::string fileName, TTree *tree = 0);

  virtual ~AccessTree();

  virtual Int_t Cut(Long64_t entry);

  virtual Int_t GetEntry(Long64_t entry);

  virtual Int_t GetEntries();

  virtual Long64_t LoadTree(Long64_t entry);

  virtual void Init(TTree *tree);

  virtual void Loop();

  virtual Bool_t Notify();

  virtual void Show(Long64_t entry = -1);
};

#endif