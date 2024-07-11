void read()
{
    auto file = new TFile("data/RUN511.ch.root","read");
    auto tree = (TTree*) file -> Get("channels");

    Short_t det, dch, adc;
    tree -> SetBranchAddress("det",&det);
    tree -> SetBranchAddress("dch",&dch);
    tree -> SetBranchAddress("adc",&adc);

    auto num_channels = 100;
    for (auto event=0; event<num_channels; ++event) {
        tree -> GetEntry(event);
        cout << det << " " << dch << " " << adc << endl;
    }
}

