// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.

#if !defined(__CLING__) || defined(__ROOTCLING__)
#include <string>

#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TRatioPlot.h>
#include <TGraph.h>
#include <TSystem.h>
#include <TLegend.h>
#include <TStyle.h>
#endif

using std::string;
void compareTracks(const string cpuFileName = "o2trac_its_cpu.root",
                   const string cudaFileName = "o2trac_its_cuda.root",
                   const string hipFileName = "o2trac_its_hip.root",
                   const string gpuWfFileName = "o2trac_its_gpuwf.root",
                   const string path = "./")
{
  gStyle->SetOptStat(0);
  bool anyFile = false;
  TCanvas* canvasPt = new TCanvas("trackpt", "tracks pt", 800, 600);
  TCanvas* canvasChi2 = new TCanvas("trackchi2", "tracks chi2", 800, 600);

  TCanvas* canvasCPUCUDA = new TCanvas("ptratiocpucuda", "pt Ratio cpu/cuda", 800, 1200);
  canvasCPUCUDA->Divide(1, 2);
  TCanvas* canvasCPUHIP = new TCanvas("ptratiocpuhip", "pt Ratio cpu/hip", 800, 1200);
  canvasCPUHIP->Divide(1, 2);
  TCanvas* canvasHIPCUDA = new TCanvas("ptratiohipcuda", "pt Ratio hip/cuda", 800, 1200);
  canvasHIPCUDA->Divide(1, 2);
  TCanvas* canvasCPUGPUWF = new TCanvas("ptratiocpugpuwf", "pt Ratio cpu/gpuwf", 800, 1200);
  canvasCPUGPUWF->Divide(1, 2);
  TCanvas* canvasCUDAGPUWF = new TCanvas("ptratiocudagpuwf", "pt Ratio cuda/gpuwf", 800, 1200);
  canvasCUDAGPUWF->Divide(1, 2);

  canvasPt->SetLogy();
  canvasChi2->SetLogy();
  canvasCPUCUDA->SetLogy();
  canvasCPUHIP->SetLogy();
  canvasHIPCUDA->SetLogy();
  canvasCPUGPUWF->SetLogy();
  canvasCUDAGPUWF->SetLogy();

  float ptMin = 0., ptMax = 20.;
  float chi2Min = 0., chi2Max = 300.;
  int ptBins = 150, chi2Bins = 150;

  TH1F *cpuPt = nullptr, *cudaPt = nullptr, *hipPt = nullptr;
  TH1F *cpuChi2 = nullptr, *cudaChi2 = nullptr, *hipChi2 = nullptr;

  TFile* cpuFile = TFile::Open((path + cpuFileName).c_str());
  TFile* cudaFile = TFile::Open((path + cudaFileName).c_str());
  TFile* hipFile = TFile::Open((path + hipFileName).c_str());
  TFile* gpuWfFile = TFile::Open((path + gpuWfFileName).c_str());

  if (cudaFile) {
    cudaPt = new TH1F("cudaPt", "CUDA track #it{p}_{T};#it{p}_{T} (GeV);counts", ptBins, ptMin, ptMax);
    cudaChi2 = new TH1F("cudaChi2", "CUDA track #chi^{2};#chi^{2};counts", chi2Bins, chi2Min, chi2Max);
    TTree* cudaTree = (TTree*)cudaFile->Get("o2sim");
    canvasPt->cd();
    cudaTree->Draw("ITSTrack.getPt()>>cudaPt", "", "same");
    cudaPt->Draw("sames");
    canvasChi2->cd();
    cudaTree->Draw("ITSTrack.getChi2()>>cudaChi2", "", "same");
    cudaPt->SetLineColor(kGreen - 4);
    cudaChi2->SetLineColor(kGreen - 4);
    cudaChi2->Draw("sames");

    anyFile = true;
  }
  if (hipFile) {
    hipPt = new TH1F("hipPt", "HIP track #it{p}_{T};#it{p}_{T} (GeV);counts", ptBins, ptMin, ptMax);
    hipChi2 = new TH1F("hipChi2", "HIP track #chi^{2};#chi^{2};counts", chi2Bins, chi2Min, chi2Max);
    TTree* hipTree = (TTree*)hipFile->Get("o2sim");
    canvasPt->cd();
    hipTree->Draw("ITSTrack.getPt()>>hipPt", "", "same");
    hipPt->Draw("sames");
    canvasChi2->cd();
    hipTree->Draw("ITSTrack.getChi2()>>hipChi2", "", "same");
    hipChi2->Draw("sames");
    hipPt->SetLineColor(kRed - 4);
    hipChi2->SetLineColor(kRed - 4);

    anyFile = true;
  }
  if (cpuFile) {
    cpuPt = new TH1F("cpuPt", "CPU track #it{p}_{T};#it{p}_{T} (GeV);counts", ptBins, ptMin, ptMax);
    cpuChi2 = new TH1F("cpuChi2", "CPU track #chi^{2};#chi^{2};counts", chi2Bins, chi2Min, chi2Max);
    TTree* cpuTree = (TTree*)cpuFile->Get("o2sim");
    canvasPt->cd();
    cpuTree->Draw("ITSTrack.getPt()>>cpuPt", "", "same");
    cpuPt->Draw("sames");
    canvasChi2->cd();
    cpuTree->Draw("ITSTrack.getChi2()>>cpuChi2", "", "same");
    cpuChi2->Draw("sames");

    anyFile = true;
  }
  if (gpuWfFile) {
    TH1F* gpuWfPt = new TH1F("gpuWfPt", "GPU WF track #it{p}_{T};#it{p}_{T} (GeV);counts", ptBins, ptMin, ptMax);
    TH1F* gpuWfChi2 = new TH1F("gpuWfChi2", "GPU WF track #chi^{2};#chi^{2};counts", chi2Bins, chi2Min, chi2Max);
    TTree* gpuWfTree = (TTree*)gpuWfFile->Get("o2sim");
    canvasPt->cd();
    gpuWfTree->Draw("ITSTrack.getPt()>>gpuWfPt", "", "same");
    gpuWfPt->Draw("sames");
    canvasChi2->cd();
    gpuWfTree->Draw("ITSTrack.getChi2()>>gpuWfChi2", "", "same");
    gpuWfChi2->Draw("sames");
    gpuWfPt->SetLineColor(kOrange);
    gpuWfChi2->SetLineColor(kOrange);

    anyFile = true;
  }

  if (cpuFile && cudaFile) {
    TH1F* cpuPtClone = (TH1F*)cpuPt->Clone("cpuPt");
    cpuPtClone->SetTitle("CPU vs CUDA #it{p}_{T} Ratio");
    auto* ratioPtCN = new TRatioPlot(cpuPtClone, cudaPt);
    ratioPtCN->GetUpperPad()->SetBottomMargin(0);
    ratioPtCN->GetUpperPad()->SetGridx();
    ratioPtCN->GetLowerPad()->SetTopMargin(0);
    ratioPtCN->GetLowerPad()->SetGridx();
    canvasCPUCUDA->cd(1);
    gPad->SetLogy();
    ratioPtCN->Draw();

    ratioPtCN->GetLowerRefGraph()->SetMinimum(0);
    ratioPtCN->GetLowerRefGraph()->SetMaximum(2);
    ratioPtCN->GetLowerRefGraph()->Fit("pol0");

    TH1F* cpuChi2Clone = (TH1F*)cpuChi2->Clone("cpuChi2");
    cpuChi2Clone->SetTitle("CPU vs CUDA #chi^{2} Ratio");
    auto* ratioChi2CN = new TRatioPlot(cpuChi2Clone, cudaChi2);
    ratioChi2CN->GetUpperPad()->SetBottomMargin(0);
    ratioChi2CN->GetUpperPad()->SetGridx();
    ratioChi2CN->GetLowerPad()->SetTopMargin(0);
    ratioChi2CN->GetLowerPad()->SetGridx();
    canvasCPUCUDA->cd(2);
    gPad->SetLogy();
    ratioChi2CN->Draw();

    ratioChi2CN->GetLowerRefGraph()->SetMinimum(0);
    ratioChi2CN->GetLowerRefGraph()->SetMaximum(2);
    ratioChi2CN->GetLowerRefGraph()->Fit("pol0");

    ratioPtCN->GetUpperPad()->cd();
    TLegend* legCN = new TLegend(0.3, 0.7, 0.7, 0.85);
    legCN->AddEntry(cpuPt, "CPU", "l");
    legCN->AddEntry(cudaPt, "CUDA", "l");
    legCN->Draw();
  }

  if (cpuFile && hipFile) {
    TH1F* cpuPtClone = (TH1F*)cpuPt->Clone("cpuPt");
    cpuPtClone->SetTitle("CPU vs HIP #it{p}_{T} Ratio");
    auto* ratioPtCH = new TRatioPlot(cpuPtClone, hipPt);
    ratioPtCH->GetUpperPad()->SetBottomMargin(0);
    ratioPtCH->GetUpperPad()->SetGridx();
    ratioPtCH->GetLowerPad()->SetTopMargin(0);
    ratioPtCH->GetLowerPad()->SetGridx();
    canvasCPUHIP->cd(1);
    gPad->SetLogy();
    ratioPtCH->Draw();

    ratioPtCH->GetLowerRefGraph()->SetMinimum(0);
    ratioPtCH->GetLowerRefGraph()->SetMaximum(2);
    ratioPtCH->GetLowerRefGraph()->Fit("pol0");

    TH1F* cpuChi2Clone = (TH1F*)cpuChi2->Clone("cpuChi2");
    cpuChi2Clone->SetTitle("CPU vs HIP #chi^{2} Ratio");
    auto* ratioChi2CH = new TRatioPlot(cpuChi2Clone, hipChi2);
    ratioChi2CH->GetUpperPad()->SetBottomMargin(0);
    ratioChi2CH->GetUpperPad()->SetGridx();
    ratioChi2CH->GetLowerPad()->SetTopMargin(0);
    ratioChi2CH->GetLowerPad()->SetGridx();
    canvasCPUHIP->cd(2);
    gPad->SetLogy();
    ratioChi2CH->Draw();

    ratioChi2CH->GetLowerRefGraph()->SetMinimum(0);
    ratioChi2CH->GetLowerRefGraph()->SetMaximum(2);
    ratioChi2CH->GetLowerRefGraph()->Fit("pol0");

    ratioPtCH->GetUpperPad()->cd();
    TLegend* legCH = new TLegend(0.3, 0.7, 0.7, 0.85);
    legCH->AddEntry(cpuPt, "CPU", "l");
    legCH->AddEntry(hipPt, "HIP", "l");
    legCH->Draw();
  }

  if (cudaFile && hipFile) {
    TH1F* hipPtClone = (TH1F*)hipPt->Clone("hipPt");
    hipPtClone->SetTitle("CUDA vs HIP #it{p}_{T} Ratio");
    auto* ratioPtHN = new TRatioPlot(hipPtClone, cudaPt);
    ratioPtHN->GetUpperPad()->SetBottomMargin(0);
    ratioPtHN->GetUpperPad()->SetGridx();
    ratioPtHN->GetLowerPad()->SetTopMargin(0);
    ratioPtHN->GetLowerPad()->SetGridx();
    canvasHIPCUDA->cd(1);
    gPad->SetLogy();
    ratioPtHN->Draw();
    ratioPtHN->GetLowerRefGraph()->SetMinimum(0);
    ratioPtHN->GetLowerRefGraph()->SetMaximum(2);
    ratioPtHN->GetLowerRefGraph()->Fit("pol0");

    TH1F* hipChi2Clone = (TH1F*)hipChi2->Clone("hipChi2");
    hipChi2Clone->SetTitle("HIP vs CUDA #chi^{2} Ratio");
    auto* ratioChi2HN = new TRatioPlot(hipChi2Clone, cudaChi2);
    ratioChi2HN->GetUpperPad()->SetBottomMargin(0);
    ratioChi2HN->GetUpperPad()->SetGridx();
    ratioChi2HN->GetLowerPad()->SetTopMargin(0);
    ratioChi2HN->GetLowerPad()->SetGridx();
    canvasHIPCUDA->cd(2);
    gPad->SetLogy();
    ratioChi2HN->Draw();
    ratioChi2HN->GetLowerRefGraph()->SetMinimum(0);
    ratioChi2HN->GetLowerRefGraph()->SetMaximum(2);
    ratioChi2HN->GetLowerRefGraph()->Fit("pol0");

    ratioPtHN->GetUpperPad()->cd();
    TLegend* legHN = new TLegend(0.3, 0.7, 0.7, 0.85);
    legHN->AddEntry(hipPt, "HIP", "l");
    legHN->AddEntry(cudaPt, "CUDA", "l");
    legHN->Draw();
  }

  if (cpuFile && gpuWfFile) {
    TH1F* cpuPtClone = (TH1F*)cpuPt->Clone("cpuPt");
    cpuPtClone->SetTitle("CPU vs GPU WF #it{p}_{T} Ratio");
    auto* ratioPtCG = new TRatioPlot(cpuPtClone, (TH1F*)gpuWfFile->Get("gpuWfPt"));
    ratioPtCG->GetUpperPad()->SetBottomMargin(0);
    ratioPtCG->GetUpperPad()->SetGridx();
    ratioPtCG->GetLowerPad()->SetTopMargin(0);
    ratioPtCG->GetLowerPad()->SetGridx();
    canvasCPUGPUWF->cd(1);
    gPad->SetLogy();
    ratioPtCG->Draw();
    ratioPtCG->GetLowerRefGraph()->SetMinimum(0);
    ratioPtCG->GetLowerRefGraph()->SetMaximum(2);
    ratioPtCG->GetLowerRefGraph()->Fit("pol0");

    TH1F* cpuChi2Clone = (TH1F*)cpuChi2->Clone("cpuChi2");
    cpuChi2Clone->SetTitle("CPU vs GPU WF #chi^{2} Ratio");
    auto* ratioChi2CG = new TRatioPlot(cpuChi2Clone, (TH1F*)gpuWfFile->Get("gpuWfChi2"));
    ratioChi2CG->GetUpperPad()->SetBottomMargin(0);
    ratioChi2CG->GetUpperPad()->SetGridx();
    ratioChi2CG->GetLowerPad()->SetTopMargin(0);
    ratioChi2CG->GetLowerPad()->SetGridx();
    canvasCPUGPUWF->cd(2);
    gPad->SetLogy();
    ratioChi2CG->Draw();
    ratioChi2CG->GetLowerRefGraph()->SetMinimum(0);
    ratioChi2CG->GetLowerRefGraph()->SetMaximum(2);
    ratioChi2CG->GetLowerRefGraph()->Fit("pol0");

    ratioPtCG->GetUpperPad()->cd();
    TLegend* legCG = new TLegend(0.3, 0.7, 0.7, 0.85);
    legCG->AddEntry(cpuPt, "CPU", "l");
    legCG->AddEntry(gpuWfFile->Get("gpuWfPt"), "GPU WF", "l");
    legCG->Draw();
  }

  if (cudaFile && gpuWfFile) {
    TH1F* cudaPtClone = (TH1F*)cudaPt->Clone("cudaPt");
    cudaPtClone->SetTitle("CUDA vs GPU WF #it{p}_{T} Ratio");
    auto* ratioPtCG = new TRatioPlot(cudaPtClone, (TH1F*)gpuWfFile->Get("gpuWfPt"));
    ratioPtCG->GetUpperPad()->SetBottomMargin(0);
    ratioPtCG->GetUpperPad()->SetGridx();
    ratioPtCG->GetLowerPad()->SetTopMargin(0);
    ratioPtCG->GetLowerPad()->SetGridx();
    canvasCUDAGPUWF->cd(1);
    gPad->SetLogy();
    ratioPtCG->Draw();
    ratioPtCG->GetLowerRefGraph()->SetMinimum(0);
    ratioPtCG->GetLowerRefGraph()->SetMaximum(2);
    ratioPtCG->GetLowerRefGraph()->Fit("pol0");

    TH1F* cudaChi2Clone = (TH1F*)cudaChi2->Clone("cudaChi2");
    cudaChi2Clone->SetTitle("CUDA vs GPU WF #chi^{2} Ratio");
    auto* ratioChi2CG = new TRatioPlot(cudaChi2Clone, (TH1F*)gpuWfFile->Get("gpuWfChi2"));
    ratioChi2CG->GetUpperPad()->SetBottomMargin(0);
    ratioChi2CG->GetUpperPad()->SetGridx();
    ratioChi2CG->GetLowerPad()->SetTopMargin(0);
    ratioChi2CG->GetLowerPad()->SetGridx();
    canvasCUDAGPUWF->cd(2);
    gPad->SetLogy();
    ratioChi2CG->Draw();
    ratioChi2CG->GetLowerRefGraph()->SetMinimum(0);
    ratioChi2CG->GetLowerRefGraph()->SetMaximum(2);
    ratioChi2CG->GetLowerRefGraph()->Fit("pol0");

    ratioPtCG->GetUpperPad()->cd();
    TLegend* legCG = new TLegend(0.3, 0.7, 0.7, 0.85);
    legCG->AddEntry(cudaPt, "CUDA", "l");
    legCG->AddEntry(gpuWfFile->Get("gpuWfPt"), "GPU WF", "l");
    legCG->Draw();
  }

  canvasPt->SaveAs("ptComp.png");
  canvasChi2->SaveAs("chi2Comp.png");
  canvasCPUCUDA->SaveAs("ptRatioCN.png");
  canvasCPUHIP->SaveAs("ptRatioCH.png");
  canvasHIPCUDA->SaveAs("ptRatioHN.png");
  canvasCPUGPUWF->SaveAs("ptRatioCG.png");
  canvasCUDAGPUWF->SaveAs("ptRatioNG.png");
}