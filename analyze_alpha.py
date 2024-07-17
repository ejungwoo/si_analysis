import ROOT
from ROOT import TFile, TTree, TCanvas, TH1D, TF1

file = TFile("data/RUN511.ch.root","read")
tree = file.Get("channels")

cvs = TCanvas("cvs","cvs",800,600)
hist = TH1D("hist","",50,1450,1600)
tree.Draw("adc>>hist","det==2 && dch==10","")

fit = TF1("fit","gaus(0)",1450,1600)
hist.Fit(fit)
