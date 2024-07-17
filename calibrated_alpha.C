void calibrated_alpha()
{
    gStyle -> SetTitleSize(0.09,"t"); // set title size on the top of the histograms

    int i, j;
    double mean, sigma;
    double conversion_factor[5][33] = {0};
    ifstream fcal("data/calibration_file.txt");
    while (fcal >> i >> j >> mean >> sigma)
        conversion_factor[i][j] = 5.486/mean;

    auto file = new TFile("data/RUN511.ch.root","read");
    auto tree = (TTree*) file -> Get("channels");

    for (auto det : {2,3,4})
    {
        double x1 = 5.1;
        double x2 = 5.7;

        auto cvs1 = new TCanvas(Form("calibrated_alpha_det%d_1",det),"cvs1",1200,700);
        cvs1 -> Divide(4,4);
        auto cvs2 = new TCanvas(Form("calibrated_alpha_det%d_2",det),"cvs2",1200,700);
        cvs2 -> Divide(4,4);

        for (auto dch=1; dch<=32; ++dch)
        {
            TVirtualPad* pad;
            if (dch<=16)
                pad = cvs1 -> cd(dch);
            else
                pad = cvs2 -> cd(dch-16);
            pad -> SetMargin(0.1,0.05,0.1,0.1);

            TString nameHist = Form("hist_d%dc%d",det,dch);
            TCut cut = Form("det==%d && dch==%d",det,dch);
            auto hist = new TH1D(nameHist,cut,50,x1,x2);
            hist -> GetXaxis() -> SetLabelSize(0.065);
            hist -> GetYaxis() -> SetLabelSize(0.065);

            TString selection = Form("adc*%f>>%s",conversion_factor[det][dch],nameHist.Data());
            cout << selection << endl;
            tree -> Draw(selection,cut);
        }

        cvs1 -> cd(); cvs1 -> SaveAs(Form("figures/%s.pdf",cvs1->GetName()));
        cvs2 -> cd(); cvs2 -> SaveAs(Form("figures/%s.pdf",cvs2->GetName()));
    }
}
