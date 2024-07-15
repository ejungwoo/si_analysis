void analysis_alpha()
{
    gStyle -> SetTitleSize(0.09,"t"); // set title size on the top of the histograms

    ofstream fcal("calibration_file.txt");

    auto file = new TFile("data/RUN511.ch.root","read");
    auto tree = (TTree*) file -> Get("channels");

    for (auto det : {2,3,4})
    {
        // set expected adc ranges
        double x1 = 1450;
        double x2 = 1600;
        if (det==3 || det==4) {
            x1 = 2800;
            x2 = 3200;
        }

        auto cvs1 = new TCanvas(Form("cvs1_det%d",det),"cvs1",1200,700);
        cvs1 -> Divide(4,4);
        auto cvs2 = new TCanvas(Form("cvs2_det%d",det),"cvs2",1200,700);
        cvs2 -> Divide(4,4);

        for (auto dch=1; dch<=32; ++dch)
        //for (auto dch : {1})
        {
            // set expected adc ranges
            if (det==3 && (dch==22 || dch==23)) {
                x1 = 2500;
                x2 = 3500;
            }
            TVirtualPad* pad;
            if (dch<=16)
                pad = cvs1 -> cd(dch);
            else
                pad = cvs2 -> cd(dch-16);
            pad -> SetMargin(0.1,0.01,0.1,0.1);

            TString nameHist = Form("hist_d%dc%d",det,dch);
            TCut cut = Form("det==%d && dch==%d",det,dch);
            TH1D* hist = nullptr;
            hist = new TH1D(nameHist,cut,50,x1,x2);
            hist -> GetXaxis() -> SetLabelSize(0.065);
            hist -> GetYaxis() -> SetLabelSize(0.065);

            tree -> Draw(TString("adc>>")+nameHist,cut,"");
            if (hist->GetEntries()==0) // skip fitting if data do not exist
                continue;

            auto amplitude = hist -> GetBinContent(hist -> GetMaximumBin()); // get initial amplitude from histogram statistics
            auto mean = hist -> GetBinCenter(hist -> GetMaximumBin()); // get initial mean from histogram statistics
            auto sigma = hist -> GetStdDev(); // get initial stddev from histogram statistics

            auto fit = new TF1("fit","gaus(0)",x1,x2);
            fit -> SetParameters(amplitude, mean, sigma); // set initial parameter
            fit -> SetRange(mean - 0.5*sigma, mean + 3*sigma); // set initial range
            hist -> Fit(fit,"RQ");

            if (1) { // set range from the previous fit and perform fit
                amplitude = fit -> GetParameter(0);
                mean = fit -> GetParameter(1);
                sigma = fit -> GetParameter(2);
                if (dch==1||dch==32)
                    fit -> SetRange(mean - 0.5*sigma, mean + 3*sigma);
                else
                    fit -> SetRange(mean - sigma, mean + 3*sigma);
                hist -> Fit(fit,"RQ");
            }

            amplitude = fit -> GetParameter(0);
            mean = fit -> GetParameter(1);
            sigma = fit -> GetParameter(2);
            //cout << "detector=" << det << ", channel=" << dch << ", energy-resoluation=" << sigma/mean*100 << " %" << endl;
            cout << det << " " << dch << " " << mean << " " << sigma << endl;
            fcal << det << " " << dch << " " << mean << " " << sigma << endl;
        }
    }
}
