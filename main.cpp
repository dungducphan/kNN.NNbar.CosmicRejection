#include <iostream>
#include <algorithm>
#include <iterator>

#include "AccessTree.h"
#include "VpTree.h"
#include "Style.C"

enum KNNID {kSignal, kCosmic, kNA};

struct Point {
  bool IsTrainingSample;
  bool IsCosmic;
  KNNID kNNPID;
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
  Double_t AreaRatioXV;
  Double_t AreaRatioYV;
  Double_t AverageYposition;
};

const double TrainingSampleRatio = 0.8;

Int_t HistSizeX = 50;
double kLowerLim = 50;
double kUpperLim = 150;

Int_t HistSizeY = 50;
double voteLowerLim = 0;
double voteUpperLim = 100;

static double DistancePID(const Point&, const Point&);

double DistancePID(const Point& p1, const Point& p2) {
  double sumSq = 0.;
  sumSq   = pow(p1.TotalVisibleEnergy       - p2.TotalVisibleEnergy,       2)
          + pow(p1.TotalHitCountInXView     - p2.TotalHitCountInXView,     2)
          + pow(p1.TotalHitCountInYView     - p2.TotalHitCountInYView,     2)
          + pow(p1.HitCountXYDifference     - p2.HitCountXYDifference,     2)
          + pow(p1.AverageEnergyPerHitXView - p2.AverageEnergyPerHitXView, 2)
          + pow(p1.AverageEnergyPerHitYView - p2.AverageEnergyPerHitYView, 2)
          + pow(p1.AreaRatioXV              - p2.AreaRatioXV,              2)
          + pow(p1.AreaRatioYV              - p2.AreaRatioYV,              2);
  return sqrt(sumSq);
}

int main() {
  GeneralStyle();

  VpTree<Point, DistancePID> SearchTree;

  std::vector<Point> megaSample;
  std::vector<Point> trainingSample;
  std::vector<Point> testingSample;

  AccessTree* cosmicTree = new AccessTree("/Users/dphan/nova.nnbar.kNN/NNbar_Analysis_Hist.root");
  AccessTree* signalTree = new AccessTree("/Users/dphan/nova.nnbar.kNN/NNbar_Analysis_Hist.Signal.root");

  auto NCosmic_All = cosmicTree->GetEntries();
  auto NSignal_All = signalTree->GetEntries();

  for (unsigned int i = 0; i < NCosmic_All; i++) {
    cosmicTree->GetEntry(i);
    Point aPoint{};

    aPoint.IsTrainingSample         = i < (int)(TrainingSampleRatio * NCosmic_All);
    aPoint.IsCosmic                 = true;
    aPoint.kNNPID                   = kNA;
    aPoint.TotalVisibleEnergy       = cosmicTree->TotalVisibleEnergy;
    aPoint.TotalHitCountInXView     = cosmicTree->TotalHitCountInXView;
    aPoint.TotalHitCountInYView     = cosmicTree->TotalHitCountInYView;
    aPoint.HitCountXYDifference     = cosmicTree->HitCountXYDifference;
    aPoint.TemporalClusterExpand    = cosmicTree->TemporalClusterExpand;
    aPoint.AverageEnergyPerHitXView = cosmicTree->AverageEnergyPerHitXView;
    aPoint.AverageEnergyPerHitYView = cosmicTree->AverageEnergyPerHitYView;
    aPoint.CorrelationTCellXView    = cosmicTree->CorrelationTCellXView;
    aPoint.CorrelationTPlaneXView   = cosmicTree->CorrelationTPlaneXView;
    aPoint.CorrelationTCellYView    = cosmicTree->CorrelationTCellYView;
    aPoint.CorrelationTPlaneYView   = cosmicTree->CorrelationTPlaneYView;
    aPoint.AreaRatioXV              = cosmicTree->ConvexHullAreaXV/cosmicTree->MinimalEnclosingCircleAreaXV;
    aPoint.AreaRatioYV              = cosmicTree->ConvexHullAreaYV/cosmicTree->MinimalEnclosingCircleAreaYV;
    aPoint.AverageYposition         = cosmicTree->AverageYposition;

    megaSample.push_back(aPoint);
  }

  for (unsigned int i = 0; i < NSignal_All; i++) {
    signalTree->GetEntry(i);
    Point aPoint{};

    aPoint.IsTrainingSample         = i < (int)(TrainingSampleRatio * NSignal_All);
    aPoint.IsCosmic                 = false;
    aPoint.kNNPID                   = kNA;
    aPoint.TotalVisibleEnergy       = signalTree->TotalVisibleEnergy;
    aPoint.TotalHitCountInXView     = signalTree->TotalHitCountInXView;
    aPoint.TotalHitCountInYView     = signalTree->TotalHitCountInYView;
    aPoint.HitCountXYDifference     = signalTree->HitCountXYDifference;
    aPoint.TemporalClusterExpand    = signalTree->TemporalClusterExpand;
    aPoint.AverageEnergyPerHitXView = signalTree->AverageEnergyPerHitXView;
    aPoint.AverageEnergyPerHitYView = signalTree->AverageEnergyPerHitYView;
    aPoint.CorrelationTCellXView    = signalTree->CorrelationTCellXView;
    aPoint.CorrelationTPlaneXView   = signalTree->CorrelationTPlaneXView;
    aPoint.CorrelationTCellYView    = signalTree->CorrelationTCellYView;
    aPoint.CorrelationTPlaneYView   = signalTree->CorrelationTPlaneYView;
    aPoint.AreaRatioXV              = signalTree->ConvexHullAreaXV/signalTree->MinimalEnclosingCircleAreaXV;
    aPoint.AreaRatioYV              = signalTree->ConvexHullAreaYV/signalTree->MinimalEnclosingCircleAreaYV;
    aPoint.AverageYposition         = signalTree->AverageYposition;

    megaSample.push_back(aPoint);
  }

  for (auto aPoint : megaSample) {
    bool isTraining = aPoint.IsTrainingSample;
    bool isCosmic   = aPoint.IsCosmic;

    if (isTraining) {
      trainingSample.push_back(aPoint);
    } else {
      testingSample.push_back(aPoint);
    }
  }

  // Make kNN search tree
  SearchTree.create(trainingSample);

  // Validation
  TH2D* Type1Error = new TH2D("Type1Error", "Type-1 Error;k-Value;Cosmic Vote Threshold (%)", HistSizeX, kLowerLim, kUpperLim, HistSizeY, voteLowerLim, voteUpperLim);
  TH2D* SignalEffiency = new TH2D("SignalEffiency", "1 - Signal Eff.;k-Value;Cosmic Vote Threshold (%)", HistSizeX, kLowerLim, kUpperLim, HistSizeY, voteLowerLim, voteUpperLim);
  TH2D* Optimization = new TH2D("Optimization", "Optimization;k-Value;Cosmic Vote Threshold (%)", HistSizeX, kLowerLim, kUpperLim, HistSizeY, voteLowerLim, voteUpperLim);
  for (unsigned int ik = 0; ik < HistSizeX; ik++) {
    for (unsigned int it = 0; it < HistSizeY; it++) {
      unsigned int trueCosmic_as_kNNCosmic = 0;
      unsigned int trueCosmic_as_kNNSignal = 0;
      unsigned int trueSignal_as_kNNCosmic = 0;
      unsigned int trueSignal_as_kNNSignal = 0;
      unsigned int trueCosmic = 0;
      unsigned int trueSignal = 0;
      for (auto aTestPoint : testingSample) {
        std::vector<Point> results;
        std::vector<double> distances;

        (aTestPoint.IsCosmic) ? trueCosmic++ : trueSignal++;

        auto kValue = (unsigned int) Type1Error->ProjectionX()->GetBinLowEdge(ik + 1);
        double CosmicVoteThreshold = (double) Type1Error->ProjectionY()->GetBinLowEdge(it + 1) / 100;
        SearchTree.search(aTestPoint, kValue, &results, &distances);
        unsigned int cosmicVote = 0;
        for (auto &result : results) {
          if (result.IsCosmic) cosmicVote++;
        }

        double voteRatio = (double) cosmicVote / (double) kValue;
        KNNID knnpid = (voteRatio > CosmicVoteThreshold) ? kCosmic : kSignal;

        if ((aTestPoint.IsCosmic) && (knnpid == kCosmic)) trueCosmic_as_kNNCosmic++;
        if ((aTestPoint.IsCosmic) && (knnpid == kSignal)) trueCosmic_as_kNNSignal++;
        if (!(aTestPoint.IsCosmic) && (knnpid == kCosmic)) trueSignal_as_kNNCosmic++;
        if (!(aTestPoint.IsCosmic) && (knnpid == kSignal)) trueSignal_as_kNNSignal++;
      }
      Type1Error->SetBinContent(ik + 1, it + 1, (double) trueCosmic_as_kNNSignal / (double) trueCosmic);
      SignalEffiency->SetBinContent(ik + 1, it + 1, 1 - (double) trueSignal_as_kNNSignal / (double) trueSignal);
      Optimization->SetBinContent(ik + 1, it + 1, ((double) trueCosmic_as_kNNSignal / (double) trueCosmic)*(1 - (double) trueSignal_as_kNNSignal / (double) trueSignal));
    }
  }

  TCanvas* c = new TCanvas("c", "c", 800, 400);
  c->Divide(2,1);

  c->cd(1);
  gPad->SetRightMargin(0.17);
  gPad->SetLeftMargin(0.17);
  gPad->SetBottomMargin(0.17);
  gPad->SetTopMargin(0.17);
  Type1Error->GetZaxis()->SetMaxDigits(2);
  Type1Error->GetXaxis()->SetMaxDigits(3);
  Type1Error->GetYaxis()->SetMaxDigits(3);
  Type1Error->GetZaxis()->SetRangeUser(0, 1);
  Type1Error->GetXaxis()->SetLabelOffset(0.015);
  Type1Error->GetYaxis()->SetLabelOffset(0.015);
  Type1Error->GetXaxis()->CenterTitle();
  Type1Error->GetYaxis()->CenterTitle();
  Type1Error->GetXaxis()->SetTitleOffset(1.3);
  Type1Error->GetYaxis()->SetTitleOffset(1.3);
  Type1Error->Draw("COLZ");

  c->cd(2);
  gPad->SetRightMargin(0.17);
  gPad->SetLeftMargin(0.17);
  gPad->SetBottomMargin(0.17);
  gPad->SetTopMargin(0.17);
  SignalEffiency->GetZaxis()->SetMaxDigits(2);
  SignalEffiency->GetXaxis()->SetMaxDigits(3);
  SignalEffiency->GetYaxis()->SetMaxDigits(3);
  SignalEffiency->GetZaxis()->SetRangeUser(0, 1);
  SignalEffiency->GetXaxis()->SetLabelOffset(0.015);
  SignalEffiency->GetYaxis()->SetLabelOffset(0.015);
  SignalEffiency->GetXaxis()->CenterTitle();
  SignalEffiency->GetYaxis()->CenterTitle();
  SignalEffiency->GetXaxis()->SetTitleOffset(1.3);
  SignalEffiency->GetYaxis()->SetTitleOffset(1.3);
  SignalEffiency->Draw("COLZ");

  c->SaveAs("kNN.pdf");

  TCanvas* d = new TCanvas("d", "d", 200, 200);
  d->SetLogz();
  d->SetRightMargin(0.17);
  d->SetLeftMargin(0.17);
  d->SetBottomMargin(0.17);
  d->SetTopMargin(0.17);
  Optimization->GetZaxis()->SetMaxDigits(2);
  Optimization->GetXaxis()->SetMaxDigits(3);
  Optimization->GetYaxis()->SetMaxDigits(3);
  Optimization->GetZaxis()->SetRangeUser(1E-2, 1E-1);
  Optimization->GetXaxis()->SetLabelOffset(0.015);
  Optimization->GetYaxis()->SetLabelOffset(0.015);
  Optimization->GetXaxis()->CenterTitle();
  Optimization->GetYaxis()->CenterTitle();
  Optimization->GetXaxis()->SetTitleOffset(1.3);
  Optimization->GetYaxis()->SetTitleOffset(1.3);
  Optimization->Draw("COLZ");
  d->SaveAs("Optimize.pdf");

  return 0;
}