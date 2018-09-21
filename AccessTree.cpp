#include "AccessTree.h"

void AccessTree::Loop() {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry = 0; jentry < nentries; jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);
    nbytes += nb;
  }
}

AccessTree::AccessTree(std::string fileName, TTree *tree) : fChain(0) {
  if (tree == 0) {
    TFile *f = (TFile *) gROOT->GetListOfFiles()->FindObject(fileName.c_str());
    if (!f || !f->IsOpen()) {
      f = new TFile(fileName.c_str());
    }
    std::string folderName = fileName + ":/cosmicshoweranalyzer";
    TDirectory *dir = (TDirectory *) f->Get(folderName.c_str());
    dir->GetObject("t_VariableTree", tree);

  }
  Init(tree);
}

AccessTree::~AccessTree() {
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t AccessTree::GetEntry(Long64_t entry) {
// Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}

Long64_t AccessTree::LoadTree(Long64_t entry) {
// Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Notify();
  }
  return centry;
}

void AccessTree::Init(TTree *tree) {
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

//  fChain->SetBranchAddress("RunNumber", &RunNumber, &b_RunNumber);
//  fChain->SetBranchAddress("SubrunNumber", &SubrunNumber, &b_SubrunNumber);
//  fChain->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber);
//  fChain->SetBranchAddress("SliceNumber", &SliceNumber, &b_SliceNumber);
  fChain->SetBranchAddress("TotalVisibleEnergy", &TotalVisibleEnergy, &b_TotalVisibleEnergy);
  fChain->SetBranchAddress("TotalHitCountInXView", &TotalHitCountInXView, &b_TotalHitCountInXView);
  fChain->SetBranchAddress("TotalHitCountInYView", &TotalHitCountInYView, &b_TotalHitCountInYView);
  fChain->SetBranchAddress("HitCountXYDifference", &HitCountXYDifference, &b_HitCountXYDifference);
  fChain->SetBranchAddress("TemporalClusterExpand", &TemporalClusterExpand, &b_TemporalClusterExpand);
  fChain->SetBranchAddress("AverageEnergyPerHitXView", &AverageEnergyPerHitXView, &b_AverageEnergyPerHitXView);
  fChain->SetBranchAddress("AverageEnergyPerHitYView", &AverageEnergyPerHitYView, &b_AverageEnergyPerHitYView);
  fChain->SetBranchAddress("CorrelationTCellXView", &CorrelationTCellXView, &b_CorrelationTCellXView);
  fChain->SetBranchAddress("CorrelationTPlaneXView", &CorrelationTPlaneXView, &b_CorrelationTPlaneXView);
  fChain->SetBranchAddress("CorrelationTCellYView", &CorrelationTCellYView, &b_CorrelationTCellYView);
  fChain->SetBranchAddress("CorrelationTPlaneYView", &CorrelationTPlaneYView, &b_CorrelationTPlaneYView);
  fChain->SetBranchAddress("ConvexHullAreaXV", &ConvexHullAreaXV, &b_ConvexHullAreaXV);
  fChain->SetBranchAddress("ConvexHullAreaYV", &ConvexHullAreaYV, &b_ConvexHullAreaYV);
  fChain->SetBranchAddress("MinimalEnclosingCircleAreaXV", &MinimalEnclosingCircleAreaXV,
                           &b_MinimalEnclosingCircleAreaXV);
  fChain->SetBranchAddress("MinimalEnclosingCircleAreaYV", &MinimalEnclosingCircleAreaYV,
                           &b_MinimalEnclosingCircleAreaYV);
  fChain->SetBranchAddress("AverageYposition", &AverageYposition, &b_AverageYposition);
  Notify();
}

Bool_t AccessTree::Notify() {
  return kTRUE;
}

void AccessTree::Show(Long64_t entry) {
  if (!fChain) return;
  fChain->Show(entry);
}

Int_t AccessTree::Cut(Long64_t entry) {
  return 1;
}

Int_t AccessTree::GetEntries() {
  return fChain->GetEntries();
}
