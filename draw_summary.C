void draw_summary()
{
    auto file = new TFile("data/RUN511.ch.root","read");
    auto tree = (TTree*) file -> Get("channels");

    auto cvs = new TCanvas("cvs","cvs",1200,750);
    cvs -> Divide(2,2);
    int i = 1;
    for (auto var : {"adc","det","dch"})
    {
        cout << i << " " << var << endl;
        cvs -> cd(i);
        tree -> Draw(var);
        i++;
    }
}
