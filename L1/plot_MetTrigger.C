//#include "setTDRStyle.C"

Double_t errorFun(Double_t *x, Double_t *par) {

  return 0.5*par[0]*(1. + TMath::Erf( (x[0] - par[1]) / (sqrt(2.)*par[2]) )) ;
  // return par[3] + par[0] * TMath::Freq( (x[0] - par[1]) / par[2] ) * TMath::Freq( (x[0] - par[4] ) / par[5] );

}


TLegend *legend() {

  TLegend *leg2 = new TLegend(0.57,0.6,0.92,0.92);
  leg2->SetFillStyle(0);
  leg2->SetBorderSize(0);
  leg2->SetTextSize(0.04);
  leg2->SetTextFont(42);

  return leg2;

}
TH2* readHist2D(TString nameHist,TString nameFile, int rebin)
{
  TFile* file = new TFile(nameFile);
// file->ls();
// TDirectory* dir = (TDirectory*)file->Get(Dirname);
// dir->ls();
  TH2* hist = (TH2*)file->Get(nameHist);
// hist->SetLineWidth(2);
  if(rebin>0) hist->RebinX(rebin);
  hist->GetXaxis()->SetTitleSize(.055);
  hist->GetYaxis()->SetTitleSize(.055);
  hist->GetXaxis()->SetLabelSize(.05);
  hist->GetYaxis()->SetLabelSize(.05);
  // hist->SetStats(kFALSE);

  return hist;

}

TH1* readHistRef(TString nameHist,TString nameFile, int rebin)
{
  TFile* file = new TFile(nameFile);
// file->ls();
// TDirectory* dir = (TDirectory*)file->Get(Dirname);
// dir->ls();
  TH1* hist = (TH1*)file->Get(nameHist);
// hist->SetLineWidth(2);
  if(rebin>0) hist->Rebin(rebin);
  hist->GetXaxis()->SetTitleSize(.055);
  hist->GetYaxis()->SetTitleSize(.055);
  hist->GetXaxis()->SetLabelSize(.05);
  hist->GetYaxis()->SetLabelSize(.05);
  // hist->SetStats(kFALSE);

  return hist;
}

TH1* readHistSel(TString nameHist,TString nameFile, int rebin)
{
  TFile* file =  new TFile(nameFile);
// file->ls();
// TDirectory* dir = (TDirectory*)file->Get(Dirname);
// dir->ls();
  TH1* hist = (TH1*)file->Get(nameHist);
// hist->SetLineWidth(2);

  hist->SetLineColor(9); hist->SetFillColor(9);
  hist->SetFillStyle(3035);

  if(rebin>0) hist->Rebin(rebin);
  hist->GetXaxis()->SetTitleSize(.055);
  hist->GetYaxis()->SetTitleSize(.055);
  hist->GetXaxis()->SetLabelSize(.05);
  hist->GetYaxis()->SetLabelSize(.05);
  // hist->SetStats(kFALSE);

  return hist;
}


TCanvas* getaCanvas(TString name)
{

  TCanvas* aCanvas = new TCanvas(name);

  aCanvas->SetFillColor(0);
  aCanvas->SetBottomMargin(0.125);
  aCanvas->SetLeftMargin(0.125);
  aCanvas->SetFrameFillColor(0);
  aCanvas->SetFrameBorderMode(0);
  aCanvas->SetFrameLineWidth(2);

  return aCanvas;
}

void drawPair(TString hname1, TString hname2, TString filename, TString entry1, TString entry2, TString xtitle, int rebin, float xhigh) {

  TH1D *ref = readHistRef(hname1, filename, rebin);
  TH1D *sel = readHistSel(hname2, filename, rebin);

  TCanvas *c=getaCanvas(hname1+hname2);

  TLegend *leg = legend();
  leg->AddEntry(ref,entry1,"LF");
  leg->AddEntry(sel,entry2,"LF");

  gPad->SetLogy(); //RefEt->SetStats(kFALSE);
  ref->Draw("E1HIST"); sel->Draw("HISTE1SAMES");

  ref->SetFillStyle(3035); ref->SetFillColor(1);
  sel->SetFillColor(9); sel->SetLineWidth(2);
  ref->GetXaxis()->SetTitle(xtitle);
  ref->GetXaxis()->SetRangeUser(0.,xhigh);

  leg->Draw("SAME");

}

TGraphAsymmErrors* drawEff(TString hname1, TString hname2, TString filename, TString header, TString xtitle, int rebin, bool asymBin, float xhigh, int icol, int imark, TString draw) {

  //gStyle->SetOptStat(1);
gStyle->SetOptTitle(1);
  // gStyle->SetOptFit(1);
  TCanvas *c=getaCanvas(hname1+hname2+"EFF");
  TH1D *ref = readHistRef(hname1, filename, rebin);
  TH1D *sel = readHistSel(hname2, filename, rebin);

  ref->SetName(hname1+header);
  sel->SetName(hname2+header);

//  TCanvas *c=getaCanvas(hname1+hname2+"_eff");
//   gPad->SetGridy();  gPad->SetGridx();
//   gPad->SetLogx();

  TLegend *leg = legend();
  leg->SetHeader(header);

  TGraphAsymmErrors *Eff = new TGraphAsymmErrors();
  if (asymBin) {
    Double_t xbins[15]={0.,2.,4.,6.,8.,10.,12.,16.,18.
      22.,28.,34.,42.,52.,120.};

    sel->Rebin(14,"nsel",xbins);
    ref->Rebin(14,"nref",xbins);
    Eff->BayesDivide(nsel, nref);
  } else {
    Eff->BayesDivide(sel, ref);
  }

  //gStyle->SetOptStat(1);
gStyle->SetOptTitle(1);

  gStyle->SetOptFit(0);

  // TF1 *fermiFunction = new TF1("fermiFunction",errorFun,2.,100.,3);
  Double_t params[3] = {1.,20.,1.};
  // fermiFunction->SetParameters(params);
  // fermiFunction->SetParNames("#epsilon","#mu","#sigma");
  //Eff->Draw("AP");
  // Eff->Fit("fermiFunction","R0+");
  Eff->SetMarkerColor(icol);// EtEff0->SetMarkerColor(1);
  Eff->SetLineColor(icol); Eff->SetLineWidth(3);
  Eff->SetMarkerStyle(imark); Eff->SetMarkerSize(1.5);
  Eff->GetXaxis()->SetTitle(xtitle);
  Eff->GetYaxis()->SetTitle("Efficiency");
  Eff->GetYaxis()->SetRangeUser(0.,1.05);
  Eff->GetXaxis()->SetRangeUser(2.01,100.);
  // Eff->PaintFit(fermiFunction);
  TText *text = new TText(0.8,0.9,"CMS Preliminary");
  TPaveText *cms =  new TPaveText(0.3590604,0.9527972,0.7365772,0.9947552,"brNDC");
  cms->SetFillColor(0);
cms->SetBorderSize(0);
  cms->AddText("CMS Preliminary");
    cms->Draw();
  if (draw=="") {
    Eff->Draw("EAP");
  } else {
    Eff->Draw("PE");
  }
//  fermiFunction->Draw("SAME");
//   fermiFunction->SetLineColor(1);
//   fermiFunction->SetLineWidth(2);

  leg->Draw("SAME");

  TText *text = new TText(0.8,0.9,"CMS Preliminary");
  TPaveText *cms =  new TPaveText(0.3590604,0.9527972,0.7365772,0.9947552,"brNDC");
  cms->SetFillColor(0);
cms->SetBorderSize(0);
  cms->AddText("CMS Preliminary");

  return Eff;

}

void drawEff2(TString hname1, TString hname2, TString filename, TString header, TString xtitle, int rebin, float xlow, float xhigh) {

  //gStyle->SetOptStat(1);
gStyle->SetOptTitle(1);

  // gStyle->SetOptFit(1);

  TH1D *ref = readHistRef(hname1, filename, rebin);
  TH1D *sel = readHistSel(hname2, filename, rebin);

  ref->SetName(hname1+header);
  sel->SetName(hname2+header);

  TCanvas *c=getaCanvas(hname1+hname2+"_eff");
  gPad->SetGridy();  gPad->SetGridx();

  TLegend *leg = legend();
  leg->SetHeader(header);

  TGraphAsymmErrors *Eff = new TGraphAsymmErrors();
  Eff->BayesDivide(sel, ref);

  Eff->GetYaxis()->SetRangeUser(0.5,1.04);
  Eff->Draw("APE");
  TString savename = hname1+hname2+"_eff"+".pdf";
  TPaveText *cms =  new TPaveText(0.3590604,0.9527972,0.7365772,0.9947552,"brNDC");
  cms->SetFillColor(0);
cms->SetBorderSize(0);
  cms->AddText("CMS Preliminary");
    cms->Draw();

  leg->Draw("SAME");
  c->SaveAs(savename);
}
TH2D* draw2D(TString hname, TString filename, TString header, TString xtitle,TString ytitle, int rebin,
bool col, float xlow,float xhigh, float ylow, float yhigh) {

  //gStyle->SetOptStat(1);
gStyle->SetOptTitle(1);

  TH2D *ref = readHist2D(hname, filename, rebin);
  // TString a = "2DDraw";
  // hname+=a;
  // hname+=
  TCanvas *c=getaCanvas(hname);


  gStyle->SetPalette(1);


  ref->GetXaxis()->SetTitle(xtitle);
  ref->GetYaxis()->SetTitle(ytitle);
  ref->GetXaxis()->SetRangeUser(xlow,xhigh);
  ref->GetYaxis()->SetRangeUser(ylow,yhigh);
  ref->SetStats(kFALSE);

  c->Range(-14.61988,-15.15152,102.3392,106.0606);
  c->SetFillColor(0);
  c->SetBorderMode(0);
  c->SetBorderSize(2);
  c->SetTickx(1);
  c->SetTicky(1);
  c->SetLeftMargin(0.125);
  c->SetRightMargin(0.02);
  c->SetTopMargin(0.05);
  c->SetBottomMargin(0.125);
  c->SetFrameFillStyle(0);
  c->SetFrameLineWidth(2);
  c->SetFrameBorderMode(0);
  c->SetFrameFillStyle(0);
  c->SetFrameBorderMode(0);

     // TPaletteAxis *palette = new TPaletteAxis(100.6483,0.2118635,103.8898,100.2119,ref);
     // palette->SetLabelColor(1);
     // palette->SetLabelFont(42);
     // palette->SetLabelOffset(0.007);
     // palette->SetLabelSize(0.05);
     // palette->SetTitleOffset(1);
     // palette->SetTitleSize(0.06);
     //    palette->SetFillColor(100);
     //    palette->SetFillStyle(1001);
     //    ref->GetListOfFunctions()->Add(palette,"br");
     ref->SetLineStyle(0);
     ref->GetXaxis()->SetTitle(xtitle);
     ref->GetYaxis()->SetTitle(ytitle);
     ref->Draw("COLZ");




 // if (col) {
 //    ref->Draw("COLZ");
 //  } else {
 //    ref->Draw();
 //  }
  //  pt->SetLabel("#sqrt{s}=900 GeV");
  //  pt->Draw("SAME");
     TLegend *leg = new TLegend(0.1733668,0.548951,0.4208543,0.8688811,NULL,"brNDC");
   leg->SetTextFont(62);
   leg->SetTextSize(0.04370629);
   leg->SetLineColor(0);
   leg->SetLineStyle(1);
   leg->SetLineWidth(0);
   leg->SetBorderSize(0);
   leg->SetFillColor(19);
   leg->SetFillStyle(0);
   TLegendEntry *entry=leg->AddEntry("NULL",header,"h");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(62);
   leg->Draw();
         TLatex *   tex = new TLatex(0.178392,0.8461538,"CMS preliminary 2010");
      tex->SetNDC();
         tex->SetLineWidth(2);
         tex->Draw();
            tex = new TLatex(0.178392,0.7867133,"#sqrt{s} = 7 TeV");
      tex->SetNDC();
         tex->SetLineWidth(2);
         tex->Draw();

  c->SaveAs(hname+".pdf");
  c->SaveAs(hname+".C");
  return ref;
}

TH1D* project2D(TString hname, TString filename, TString header, TString xtitle,TString ytitle,int rebin, int startingBin, int endingBin, float xlow,float xhigh) {


  TH2D *h1 = readHist2D(hname, filename, rebin);
  TString a = "Project_";
  a+=hname;
  a+=startingBin;
  cout <<     h1->GetNbinsX() << endl;
  TH1D *h2 = h1->TH2D::ProjectionY(a,startingBin,endingBin,"e");

  TCanvas *c=getaCanvas(a);

  TLegend *leg = legend();
  leg->SetHeader(header);

  gStyle->SetPalette(1);

  h2->GetXaxis()->SetTitle(xtitle);
  h2->GetYaxis()->SetTitle(ytitle);
  h2->GetXaxis()->SetRangeUser(xlow,xhigh);

  h2->GetYaxis()->SetRangeUser(-5.,5.);
  h2->Draw("h");

    //  pt->SetLabel("#sqrt{s}=900 GeV");
    //  pt->Draw("SAME");
  leg->Draw("SAME");
  gPad->SetGridy();  gPad->SetGridx();
  a+= TString(".pdf");
  TPaveText *cms =  new TPaveText(0.3590604,0.9527972,0.7365772,0.9947552,"brNDC");
  cms->SetFillColor(0);
cms->SetBorderSize(0);
  cms->AddText("CMS Preliminary");
    cms->Draw();
  cout << a << endl;
  c->SaveAs(a);

  return h2;
}

void Draw1D(TString hname, TString filename, TString header, TString xtitle, TString ytitle, int rebin, int startingBin, int endingBin){
  //gStyle->SetOptStat(1);
gStyle->SetOptTitle(1);

  TFile* file =  new TFile(filename);
  TH1* hist = (TH1*)file->Get(hname);
  TCanvas *c =getaCanvas(hname);
  if(endingBin != 0){
    hist->GetXaxis()->SetRangeUser(startingBin,endingBin);
  }

  hist->SetLineColor(1);
  hist->GetXaxis()->SetTitle(xtitle);
  hist->GetYaxis()->SetTitle(ytitle);
  hist->SetFillColor(kBlue);



  hist->Draw("hE");
  TLegend *leg = new TLegend(0.580402,0.5192308,0.8278894,0.8391608,NULL,"brNDC");
     leg->SetTextFont(62);
     leg->SetTextSize(0.04370629);
     leg->SetLineColor(0);
     leg->SetLineStyle(1);
     leg->SetLineWidth(0);
     leg->SetBorderSize(0);
     leg->SetFillColor(19);
     leg->SetFillStyle(0);
     TLegendEntry *entry=leg->AddEntry("NULL",header,"h");
     entry->SetLineColor(1);
     entry->SetLineStyle(1);
     entry->SetLineWidth(1);
     entry->SetMarkerColor(1);
     entry->SetMarkerStyle(21);
     entry->SetMarkerSize(1);
     entry->SetTextFont(62);
     leg->Draw();
     tex = new TLatex(0.5866834,0.8286713,"CMS preliminary 2010");
  tex->SetNDC();
     tex->SetLineWidth(2);
     tex->Draw();
     tex = new TLatex(0.5879397,0.7744755,"#sqrt{s} = 7 TeV");
  tex->SetNDC();
     tex->SetTextAngle(359.4438);
     tex->SetLineWidth(2);
     tex->Draw();
  c->SaveAs(hname+".pdf");
  c->SaveAs(hname+".C");

}


TH1D* profile2D(TString hname, TString filename, TString header, TString xtitle,TString ytitle,int rebin, int startingBin, int endingBin, float xlow,float xhigh) {


  TH2D *p1 = readHist2D(hname, filename, rebin);
  TString a = "Profile_";
  a+=hname;
  a+=startingBin;
  TCanvas *c=getaCanvas(a);
  TH1D *p2 = p1->TH2D::ProfileX(a,startingBin,endingBin,"e");

  gPad->SetGridy();  gPad->SetGridx();
  TLegend *leg = legend();
  leg->SetHeader(header);

  gStyle->SetPalette(1);

  p2->GetXaxis()->SetTitle(xtitle);
  p2->GetYaxis()->SetTitle(ytitle);
      // p2->GetXaxis()->SetRangeUser(xlow,xhigh);
  p2->Draw("o s");

      //  pt->SetLabel("#sqrt{s}=900 GeV");
      //  pt->Draw("SAME");
  leg->Draw("SAME");
  a+= TString(".pdf");
  TPaveText *cms =  new TPaveText(0.3590604,0.9527972,0.7365772,0.9947552,"brNDC");
  cms->SetFillColor(0);
cms->SetBorderSize(0);
  cms->AddText("CMS Preliminary");
    cms->Draw();
  c->SaveAs(a);
  cout << a << endl;
  return p2;
}



void plot_MetTrigger() {

  // TString ifile="L1AnalyzerMC.root";
  TFile* output = new TFile( "PlotsMC.root", "RECREATE" );

  // gROOT.ProcessLine(".L /Users/bryn/.root/tdrstyle.C");

  // setTDRStyle();
  gROOT->ForceStyle();


  TString ifile="L1Jets_MC38.root";
  TFile* output = new TFile( "JetsMCPlotME.root", "RECREATE" );
  output->cd();
  int m_etNBins = 50; int m_etaNBins = 22; int m_phiNBins = 20;
  double m_etMin = 0.; double m_etaMin = -5.; double m_phiMin = -3.3161256;
  double m_etMax = 100.; double m_etaMax = 5.; double m_phiMax = 3.3161256;

  // gROOT->SetStyle("tdrStyle");
  gROOT->ForceStyle();
  //gStyle->SetOptStat(1);
gStyle->SetOptTitle(1);
  // gStyle->SetOptFit(1);
  // gStyle->SetStats(kTRUE);

  TCanvas *c=getaCanvas("_eff");
  gPad->SetGridy();  gPad->SetGridx();
  gPad->SetLogx();

  TH1D *ResolutionEtHE  =  profile2D("ResolutionEtHE",ifile,"Hadronic EndCap Resolution","#frac{L1-Reco}{Reco} E_{T}","",  -1,1,-1,-2.,2.);
  output->cd();
  ResolutionEtHE->Write();                                                                                                 -1,1,-1,-2.,2.);
  TH1D *ResolutionEtHB  =  profile2D("ResolutionEtHB",ifile,"Hadronic Barrel Resolution","#frac{L1-Reco}{Reco} E_{T}","",  -1,1,-1,-2.,2.);
  output->cd();                                                                                                            -1,1,-1,-2.,2.);
  ResolutionEtHB->Write();                                                                                                 -1,1,-1,-2.,2.);
  TH1D *ResolutionEtEB  =  profile2D("ResolutionEtEB",ifile,"Electron barrel Resolution","#frac{L1-Reco}{Reco} E_{T}","",  -1,1,-1,-2.,2.);
  output->cd();                                                                                                            -1,1,-1,-2.,2.);
  ResolutionEtEB->Write();                                                                                                 -1,1,-1,-2.,2.);
  TH1D *ResolutionEtEE  =  profile2D("ResolutionEtEE",ifile,"Electron Endcap Resolution","#frac{L1-Reco}{Reco} E_{T}","",  -1,1,-1,-2.,2.);
  output->cd();
  ResolutionEtEE->Write();
  return;
  TH1D *SumEtProfile =  profile2D("SumEt reconstuction",ifile,"SumEt Profile","SumEt(GeV)","#frac{L1-Reco}{Reco} E_{T}", -1,1,-1,-2.,2.);
  output->cd();
  SumEtProfile->Write();

  profile2D("Met reconstuction",ifile,"met Profile","MET(GeV)","", -1,1,-1,-2.,2.);


  draw2D("Met reconstuction",ifile,"met Resolution","MET(GeV)","#frac{L1-Reco}{Reco} #slash{E}_{T}", -1,1,-2.,2.,0.,50.);

  return;
 // //
 // //  TGraphAsymmErrors *eff1 = drawEff("RefEt","mRefEt",ifile1, "L1_SingleJet6U","E_{T}, GeV",2,1,100.,1,24,"");
 // //   eff1->SetName("efficiency1");
 //   TGraphAsymmErrors *eff = drawEff("RefEt","mRefEt",ifile, "L1_SingleJet10U","E_{T}, GeV",2,1,100.,2,29,"SAME");
 //   eff->SetName("efficiency2");
 // //  TGraphAsymmErrors *eff3 = drawEff("RefEt","mRefEt",ifile3, "L1_SingleJet20U","E_{T}, GeV",2,1,100.,4,23,"SAME");
 // //   eff3->SetName("efficiency3");
 //
 //   // drawPair("RefEt","mRefEt",ifile, "all jets", "jets (&& L1_SingleJet10)", "E_{T}, GeV",1,100.);
 //   //
 //   // drawEff2("RefEta","mRefEta",ifile, "L1_SingleJet6U","jet #eta",0,-3.,3.);
 //   // drawEff2("RefPhi","mRefPhi",ifile, "L1_SingleJet6U","jet #phi",0,0.,3.14);
 //   //
 //   // draw2D("mRefEtaPhi",ifile,"matched jets","#eta","#phi",-1,1,-3.,3.,-3.14,3.14);
 //   // draw2D("RefEtaPhi",ifile,"unmatched jets","#eta","#phi",-1,1,-3.,3.,-3.14,3.14);
 //
 //
 //
   TCanvas *b1=getaCanvas("b1");

   double stops[5] = {0.00, 0.34, 0.61, 0.84, 1.00};
   double    red[5]   = {0.00, 0.00, 0.87, 1.00, 0.51};
   double    green[5] = {0.00, 0.81, 1.00, 0.20, 0.00};
   double    blue[5]  = {0.51, 1.00, 0.12, 0.00, 0.00};
//
TColor::CreateGradientColorTable(5, stops, red, green, blue, 100);
gStyle->SetNumberContours(100);

   TH2D *can = readHist2D("mRefEtaPhi", ifile, 1);
     TH2D *ref = readHist2D("allRefEtaPhi",ifile, 1);

    // can->Draw();
      double etabins[23] = {
       -5.000,
       -4.500,
       -4.000,
       -3.500,
       -3.000,
       -2.6,
       -1.740,
       -1.392,
       -1.044,
       -0.695,
       -0.348,
       0.000,
       0.348,
       0.695,
       1.044,
       1.392,
       1.740,
       2.6,
       3.000,
       3.500,
       4.000,
       4.500,
       5.000};

     TH2D *eff_etaphi = new TH2D("effetaphi","",22,etabins, 18, -1*TMath::Pi()-0.5, TMath::Pi()-0.5);
     eff_etaphi->Divide(can,ref,1,1);//->Draw("COLZ");
       // mRefEt->Rebin(14,"mRef",xbins);
     eff_etaphi->SetStats(kFALSE);
     eff_etaphi->SetTitle(";#eta;#phi (rad)");
     eff_etaphi->GetYaxis()->SetRangeUser(-1*TMath::Pi(),TMath::Pi());
     eff_etaphi->GetXaxis()->SetRangeUser(-2.6.,2.59);

    eff_etaphi->SetMaximum(1.0);
      eff_etaphi->SetMinimum(0.9);
     eff_etaphi->Draw("COLZ");
     TLatex *text = new TLatex(0.28,0.7,"CMS preliminary 2010");
      TLatex *text2 = new TLatex(0.28,0.4,"#sqrt{s} = 7 TeV");

      TLatex *text3 = new TLatex(0.28,0.4,"Zero suppressed in scale");
      text3.SetNDC();
      text3->Draw("SAME");
     text->Draw("same");
     text2->Draw("same");
     // eff_etaphi->SaveAs("eff_etapha.pdf");
     b1->SaveAs("EtaPhiEff.pdf");
     b1->SaveAs("EtaPhiEff.C");
    // return;
 //
 //   // draw2D("Met reconstuction",ifile,"met Resolution","MET(GeV)","", -1,1,0.,50.,-2.,2.);
 //   //  return;
 //   draw2D("corEt",ifile,"L1 vs reco p_{T} correlation","reco Jet p_{T}","L1 Jet p_{T}",-1,
 //     2,1,100.,0.,100.);
 //
 //   drawPair("RefEMFrac","mRefEMFrac",ifile,"unmatched jets","matched jets","jet EMF",10,1.01);
 //   drawPair("RefN90","mRefN90",ifile,"unmatched jets","matched jets","N90",1,50.);
 //
 //   draw2D("mRefEMFEt",ifile,"matched jets","jet EMF","jet E_{T}, GeV",10,1,-10.,10.,0.,100.);
 //   draw2D("RefEMFEt",ifile,"unmatched jets","jet EMF","jet E_{T}, GeV",10,1,-10.,10.,0.,100.);
 //
 //   draw2D("mRefN90Et",ifile,"matched jets","jet N90","jet E_{T}, GeV",-1,1,-10,50.,0.,100.);
 //   draw2D("RefN90Et",ifile,"unmatched jets","jet N90","jet E_{T}, GeV",-1,1,-10.,50.,0.,100.);
 //
 //   draw2D("dRet",ifile,"matched jets","#Delta R(L1 - reco)","jet E_{T}, GeV",1,1,0.,1.,0.,100.);
 //
 //
 //   // draw2D("SumEt reconstuction",ifile,"SumEt Resolution","SumEt(GeV)","", -1,1,0.,300.,-2.,2.);
 //
 //   /*
 //
 // // ECAL
 // drawPair("RefEEmEB","mRefEEmEB",ifile,"unmatched jets","matched jets","Em energy (EB), GeV",4,150.);
 // drawPair("RefEEmEE","mRefEEmEE",ifile,"unmatched jets","matched jets","Em energy (EE), GeV",4,150.);
 //
 // // HCAL
 // drawPair("RefEHadHB","mRefEHadHB",ifile,"unmatched jets","matched jets","hadronic E (HB), GeV",4,150.);
 // drawPair("RefEHadHE","mRefEHadHE",ifile,"unmatched jets","matched jets","hadronic E (HE), GeV",4,150.);
 // drawPair("RefEHadHO","mRefEHadHO",ifile,"unmatched jets","matched jets","hadronic E (HO), GeV",4,150.);
 // drawPair("RefEHadHF","mRefEHadHF",ifile,"unmatched jets","matched jets","hadronic E (HF), GeV",4,150.);
 // */


output->Write();
output->Close();
delete output;


}
