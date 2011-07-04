// -----------------------------------------------------------------------------
/*
  Arguments to createPlot():
  - path to input files
  - canvas title
  - histogram title
  - histogram directory
  - rebin histograms?
  - normalize all histograms to unit area?
  - logarithmic y-scale
  - output file
*/
int overlay() {

  // Path to input files
  TString path = "./";

  // Output file
  TFile* output = new TFile( "Plots.root", "RECREATE" );
  if ( !output || output->IsZombie() ) { return -1; }

  if (1) {
    // createPlot( path, "Project sum et all", "Project_SumEt reconstuction1", "", false, true, false, output );
    // createPlot( path, "Project sum et 20", "Project_SumEt reconstuction24", "", false, true, false, output );
    // createPlot( path, "Project met all", "Project_Met reconstuction1", "", false, true, false, output );
    // createPlot( path, "Project met 20", "Project_Met reconstuction24", "", false, true, false, output );
    // createPlot( path, "Profile sum et all", "Profile_SumEt reconstuction1", "", false, true, false, output );
    // createPlot( path, "Profile sum et 20", "Profile_SumEt reconstuction24", "", false, true, false, output );
    // createPlot( path, "Profile met all", "Profile_Met reconstuction1", "", false, true, false, output );
    // createPlot( path, "Profile met 20", "Profile_Met reconstuction24", "", false, true, false, output );
    //
    // createPlot( path, "Project HT all", "Project_HT reconstuction1", "", false, true, false, output );
    // createPlot( path, "Project HT 20", "Project_HT reconstuction24", "", false, true, false, output );
    // createPlot( path, "Project mht all", "Project_Mht reconstuction1", "", false, true, false, output );
    // createPlot( path, "Project mht 20", "Project_Mht reconstuction24", "", false, true, false, output );
    // createPlot( path, "Profile HT all", "Profile_HT reconstuction1", "", false, true, false, output );
    // createPlot( path, "Profile HT 20", "Profile_HT reconstuction24", "", false, true, false, output );
    // createPlot( path, "Profile mht all", "Profile_Mht reconstuction1", "", false, true, false, output );
    // createPlot( path, "Profile mht 20", "Profile_Mht reconstuction24", "", false, true, false, output );
    // createPlot( path, "1DSumET", "Project_Reco Vs L1 SumEt1", "", false, true, false, output );
    // createPlot( path, "DeltaEToverETMean", "EtRes_Et_1", "", false, true, false, output );
    // createPlot( path, "DeltaEToverETsigma", "EtRes_Et_2", "", false, true, false, output );
    // createPlot( path, "DeltaEToverETChisquared", "EtRes_Et_chi2", "", false, true, false, output );
      // createPlot( path, "Turn on curves", "JetTurnOn", "", false, true, false, output );






    createPlot(path, "Resolution Hadronic Barrel","Profile_ResolutionEtHB1", "",false,true,false,output);
    createPlot(path, "Resolution Hadronic EndCap","Profile_ResolutionEtHE1", "",false,true,false,output);
    createPlot(path, "Resolution Electron EndCap","Profile_ResolutionEtEE1", "",false,true,false,output);
    createPlot(path, "Resolution Electron Barrel","Profile_ResolutionEtEB1", "",false,true,false,output);
    // createPlot(path, "Resolution Hadronic Barrel","ResolutionHB", "",false,true,false,output);
    // createPlot(path, "Resolution Hadronic EndCap","ResolutionHE", "",false,true,false,output);
    // createPlot(path, "Resolution Electron Barrel","ResolutionEB", "",false,true,false,output);
    // createPlot(path, "Resolution Electron EndCap","ResolutionEE", "",false,true,false,output);
    // createPlot(path, "Resolution Hadronic Barrel","ResolutionHB", "",false,true,false,output);



}

  if (0) {
    createPlot( path, "LeadingJetEta", "Eta_1", "Hadronic", 10, false, true, output );
    createPlot( path, "SecondJetEta", "Eta_2", "Hadronic", 10, false, true, output );
    createPlot( path, "DeltaEta", "DeltaEta_1", "Hadronic", 10, false, true, output );
    createPlot( path, "DeltaR", "DeltaR_1", "Hadronic", 10, false, true, output );
    createPlot( path, "LeadingJetEtaNorm", "Eta_1", "Hadronic", 10, true, true, output );
    createPlot( path, "SecondJetEtaNorm", "Eta_2", "Hadronic", 10, true, true, output );
    createPlot( path, "DeltaEtaNorm", "DeltaEta_1", "Hadronic", 10, true, true, output );
    createPlot( path, "DeltaRNorm", "DeltaR_1", "Hadronic", 10, true, true, output );
  }

  output->Write();
  output->Close();
  delete output;

}

// -----------------------------------------------------------------------------
//
TCanvas* createPlot( TString path,
         TString canvas_name,
         TString name,
         TString dirmame,
         int rebin,
         bool norm,
         bool log,
         TDirectory* file )
{

  // SetSomeStyles();

  // Create legend
  TLegend* legend = new TLegend( 0.75, 0.65, 0.92, 0.92, NULL, "brNDC" );
  legend->SetFillColor(0);
  legend->SetLineColor(0);

  // Create canvas
  TCanvas* aCanvas = createCanvas( canvas_name, file, log );

  // Create histograms
  // TGraph* DataClean     = getHisto( path, name, "Jet6.root", dirmame, rebin );

if(1){
  TH1* Data  = getHisto( path, name, "JetsDataPlotME.root", dirmame, rebin );
  TH1* MC = getHisto( path, name, "JetsMCPlotME.root", dirmame, rebin );

}


if(0){
  TH1* Data  = getHisto( path, name, "L1Jets_Data38.root", dirmame, rebin );
  TH1* MC = getHisto( path, name, "L1Jets_MC38.root", dirmame, rebin );
}



  // Line colour and fill
  if ( Data ) Data->SetLineColor(1);
  if ( Data ) Data->SetFillColor(1);
  if ( Data ) Data->SetFillStyle(3003);
  if ( Data ) Data->SetMarkerStyle(20);
  if ( Data ) Data->SetMarkerColor(1);
  // if ( DataClean ) DataClean->SetLineColor(6);
  // if ( DataClean ) DataClean->SetFillColor(6);
  // if ( DataClean ) DataClean->SetFillStyle(3003);
  // if ( DataClean ) DataClean->SetMarkerStyle(25);
  // if ( DataClean ) DataClean->SetMarkerColor(6);
  if ( MC ) MC->SetLineColor(2);
  if ( MC ) MC->SetLineStyle(1);
  if ( MC ) MC->SetLineWidth(2);
  if ( MC ) MC->SetMarkerStyle(23);
  if ( MC ) MC->SetMarkerColor(2);
  // Populate legend
  legend->AddEntry( Data, " Data", "P" );
  // legend->AddEntry( DataClean, " DataCleaned", "P" );
  legend->AddEntry( MC, " MC", "P" );

  // Calc maximum number of entries
  double aMax = 0.;
  if ( Data->GetMaximum()     > aMax ) { aMax = Data->GetMaximum(); }

  if ( MC->GetMaximum() > aMax ) { aMax = MC->GetMaximum(); }


  // Calc minimum number of entries
  double aMin = 1.e12;
  if ( Data->GetMinimum(1.e-12)     < aMin ) { aMin = Data->GetMinimum(1.e-12); }

  if ( MC->GetMinimum(1.e-12) < aMin ) { aMin = MC->GetMinimum(1.e-12); }
  // Data->GetYaxis()->SetRangeUser(-0.6,2.0);


  if ( Data ) Data->GetYaxis()->SetTitleOffset(1.43);
  if ( Data ) Data->GetYaxis()->SetTitleSize(0.06);
  if ( Data ) Data->GetXaxis()->SetTitleSize(0.06);
  if ( Data ) Data->GetXaxis()->SetTitleOffset(0.9);
  // Data->SumW2();
  // MC->SumW2();
  // DataClean->SumW2();
  // if ( log ) {
  //   if ( Data ) Data->SetMaximum( aMax * 10. );
  //   if ( Data ) Data->SetMinimum( aMin * 0.1 );
  // } else {
  //   if ( Data ) Data->SetMaximum( aMax * 1.1 );
  //   if ( Data ) Data->SetMinimum( aMin * 0.9 );
  // }

  if ( norm ) {
    if ( Data ) Data->Draw("Phist");

    if ( MC->GetEntries() > 0. ) {
  MC->Scale(Data->Integral()/MC->Integral());
  MC->Draw("lsamehist");
  // DataClean->Draw("Psame");
  }

}else{ Data->Draw("EPhist");
  MC->Draw("EPLsamehist");
  }
  file->cd();
  legend->Draw("same");
  aCanvas->SaveAs( std::string(canvas_name+".pdf").c_str() );
  aCanvas->Write();
  return aCanvas;

}

// -----------------------------------------------------------------------------
//
TH1* getHisto( TString path,
         TString nameHist,
         TString nameFile,
         TString Dirname,
         int rebin ) {
  TString name = path + nameFile;
  TFile* file =  new TFile(name);
  TDirectory* dir = (TDirectory*)file->Get(Dirname);
  TH1* hist = (TH1*)file->Get(nameHist);
  if (!hist) {
    std::cout << " name: " << nameHist
        << " file: " << nameFile
        << " dir: " <<  Dirname
        << std::endl;
    abort();

  }
  hist->SetLineWidth(1);
  if ( rebin > 0 ) { hist->Rebin(rebin); }
  hist->GetXaxis()->SetTitleSize(0.055);
  hist->GetYaxis()->SetTitleSize(0.055);
  hist->GetXaxis()->SetLabelSize(0.05);
  hist->GetYaxis()->SetLabelSize(0.05);
  hist->SetStats(kFALSE);
  return hist;
}

// -----------------------------------------------------------------------------
//
TCanvas* createCanvas(TString name,TDirectory* afile, bool log)
{
  afile->cd();
  TCanvas* aCanvas = new TCanvas(name);
  gStyle->SetOptFit(1);
  gStyle->SetOptStat(0);
  gPad->SetGridx();   gPad->SetGridy();
  aCanvas->Range(-288.2483,-2.138147,1344.235,7.918939);
  aCanvas->SetFillColor(0);
  aCanvas->SetBorderMode(0);
  aCanvas->SetBorderSize(2);
  if ( log == true)aCanvas->SetLogy();
  aCanvas->SetLeftMargin(0.1765705);
  aCanvas->SetRightMargin(0.05772496);
  aCanvas->SetTopMargin(0.04778761);
  aCanvas->SetBottomMargin(0.1256637);
  aCanvas->SetFrameFillStyle(0);
  aCanvas->SetFrameLineWidth(2);
  aCanvas->SetFrameBorderMode(0);
  aCanvas->SetFrameFillStyle(0);
  aCanvas->SetFrameLineWidth(2);
  aCanvas->SetFrameBorderMode(0);


  return aCanvas;
}


