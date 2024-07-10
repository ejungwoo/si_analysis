import ROOT
from ROOT import TFile
from array import array

file = TFile("data/RUN511.ch.root","read");
tree = file.Get("channels")

det = array('h', [0])
dch = array('h', [0])
adc = array('h', [0])

tree.SetBranchAddress("det",det)
tree.SetBranchAddress("dch",dch)
tree.SetBranchAddress("adc",adc)

num_channels = tree.GetEntries()
num_channels = 100
for event in range(num_channels):
    tree.GetEntry(event)
    print(det[0],dch[0],adc[0])
