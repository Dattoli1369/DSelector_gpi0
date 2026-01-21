void Plottingpi0PStree() {

    const string filePath = "/d/grid15/angelo/AlearningRoot/gpi0_Tree.root";
    const string treeName = "gpi0__M7_B4";
    ROOT::RDataFrame flatTree_df(treeName, filePath);

    TCanvas canvas("canvas", "cPlots", 1200, 1400);

    canvas.Print("pi0plotsPS.pdf[");
    canvas.Clear();

    for (double lowerbound_TBCAL = 0; lowerbound_TBCAL < 4.0; lowerbound_TBCAL += 0.5){
    
        double upperbound_TBCAL = lowerbound_TBCAL + 0.5;
        
        auto PSlice2_TBCAL = [lowerbound_TBCAL, upperbound_TBCAL](double_t g2TBCAL, double_t g2FCAL, double g2P) {
            bool g2TBCAL_cut = (g2TBCAL != -999 && g2P >= lowerbound_TBCAL && g2P <= upperbound_TBCAL);
            return (g2TBCAL_cut || g2FCAL != -999);
        };

        auto PreviousSlice2_TBCAL = [lowerbound_TBCAL, upperbound_TBCAL](double_t g2TBCAL, double_t g2FCAL, double g2P) {
        bool g2TBCAL_cut = (g2TBCAL != -999 && g2P >= (lowerbound_TBCAL - .5) && g2P <= (upperbound_TBCAL - .5));
         return (g2TBCAL_cut || g2FCAL != -999);
        };

        auto PreviousSlice2_TBCAL_df = 
            flatTree_df.Filter(PreviousSlice2_TBCAL, {"l_g2TBCAL","l_g2FCAL","l_g2P"}, "g2P_TBCAL_cut1");


        auto PSlice2_TBCAL_df =
            flatTree_df.Filter(PSlice2_TBCAL, {"l_g2TBCAL","l_g2FCAL","l_g2P"}, "g2P_TBCAL_cut");

        double minValue_g1g2 = 0; // GeV
        double maxValue_g1g2 = .8; // GeV
        int binNumber_g1g2 = 200;
        double binWidth_g1g2 = (maxValue_g1g2 - minValue_g1g2) / binNumber_g1g2 *
                            1000.0;  // convert GeV → MeV
        
        TString name_g1g2 = Form(
            ";Mass[#gamma_{1}#gamma_{2}] (GeV); #scale[.8]{Combos / %.1f MeV}", binWidth_g1g2);
        auto hist_g1g2_cut1 = PSlice2_TBCAL_df.Histo1D({"g1g2", name_g1g2, binNumber_g1g2, minValue_g1g2, maxValue_g1g2},
                                            "l_massg1g2"
                                            );
        

        ROOT::RDF::RResultPtr<TH1D> hist_g1g2_cutPrevious1;

        if (lowerbound_TBCAL == 0) {
            hist_g1g2_cutPrevious1 = flatTree_df.Histo1D({"g1g2", name_g1g2, binNumber_g1g2, minValue_g1g2, maxValue_g1g2},
                                            "l_massg1g2"
                                            );
        }
        else {
            hist_g1g2_cutPrevious1 = PreviousSlice2_TBCAL_df.Histo1D({"g1g2", name_g1g2, binNumber_g1g2, minValue_g1g2, maxValue_g1g2},
                                            "l_massg1g2"
                                            );
        };
        
        hist_g1g2_cutPrevious1->DrawClone();
        hist_g1g2_cut1->SetLineColorAlpha(kOrange + 1, 0.85);
        hist_g1g2_cut1->DrawClone("same");
        TLatex latex;
        latex.SetNDC();
        latex.SetTextFont(42);
        latex.SetTextSize(0.03);
        latex.DrawLatex(0.3, 0.94,
        Form("g_{1}g_{2} TOF Momentum [%.1f, %.1f) GeV (g2TBCAL cut)", lowerbound_TBCAL, upperbound_TBCAL));

        canvas.Print("pi0plotsPS.pdf");
        canvas.Clear();

    };
    
    canvas.Print("pi0plotsPS.pdf]");
}
