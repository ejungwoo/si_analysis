import ROOT
from ROOT import TFile, TTree, TCanvas

file = TFile("data/RUN511.ch.root","read");
tree = file.Get("channels")

cvs = TCanvas("cvs","cvs",1200,750)
cvs.Divide(2,2)
for i, var in enumerate(["adc","det","dch"]):
    print(i,"cvs_"+var)
    cvs.cd(i+1)
    tree.Draw(var)
