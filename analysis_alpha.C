void analysis_alpha()
{
    auto file = new TFile("data/RUN511.ch.root","read");
    auto tree = (TTree*) file -> Get("channels");

    for (auto det : {2,3,4})
    {
        auto cvs = new TCanvas(Form("cvs_det%d",det),"cvs",3200,2000);
        cvs -> Divide(7,5);

        for (auto dch=1; dch<=32; ++dch)
        {
            cvs -> cd(dch);
            TString nameHist = Form("hist_d%dc%d",det,dch);
            TCut cut = Form("det==%d && dch==%d",det,dch);
            TH1D* hist = nullptr;
            if (det==2)
                hist = new TH1D(nameHist,cut,50,1450,1600);
            else
                hist = new TH1D(nameHist,cut,50,2800,3200);
            tree -> Draw(TString("adc>>")+nameHist,cut,"");

            auto fit = new TF1("fit","gaus(0)",1450,1600);
            hist -> Fit(fit);
        }
    }
}
