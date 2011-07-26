#!/usr/bin/env python

'''Created by Bryn Mathias
bryn.mathias@cern.ch
'''
import errno

import ROOT as Root
import math
from time import strftime
import os, commands
import array
Root.gROOT.SetStyle("Plain") #To set plain bkgds for slides
Root.gStyle.SetTitleBorderSize(0)
Root.gStyle.SetCanvasBorderMode(0)
Root.gStyle.SetCanvasColor(0)#Sets canvas colour white
Root.gStyle.SetOptStat(1110)#set no title on Stat box
Root.gStyle.SetLabelOffset(0.001)
Root.gStyle.SetLabelSize(0.003)
Root.gStyle.SetLabelSize(0.005,"Y")#Y axis
Root.gStyle.SetLabelSize(0.1,"X")#Y axis
Root.gStyle.SetTitleSize(0.06)
Root.gStyle.SetTitleW(0.7)
Root.gStyle.SetTitleH(0.07)
Root.gStyle.SetOptTitle(1)
Root.gStyle.SetOptStat(0)
Root.gStyle.SetOptFit(1)
Root.gStyle.SetAxisColor(1, "XYZ");
Root.gStyle.SetStripDecimals(Root.kTRUE);
Root.gStyle.SetTickLength(0.03, "XYZ");
Root.gStyle.SetNdivisions(510, "XYZ");
Root.gStyle.SetPadTickX(1);
Root.gStyle.SetPadTickY(1);
Root.gStyle.SetLabelColor(1, "XYZ");
Root.gStyle.SetLabelFont(42, "XYZ");
Root.gStyle.SetLabelOffset(0.01, "XYZ");
Root.gStyle.SetLabelSize(0.05, "XYZ");
Root.gStyle.SetHatchesLineWidth(2)
# from PlottingFunctions import *
# Root.gROOT.SetBatch(True) # suppress the creation of canvases on the screen.. much much faster if over a remote connection
# Root.gROOT.SetStyle("Plain") #To set plain bkgds for slides
# Root.gROOT.ProcessLine(".L ./Jets30/tdrstyle.C+")
Root.gStyle.SetPalette(1)
# Root.gROOT.ProcessLine(".L ./errorFun.C+")
intlumi =35.0 #4.433 #inv pico barns

def GetHist(DataSetName,folder,hist,col,norm,Legend):
    a = Root.TFile.Open(DataSetName) #open the file
    # closeList.append(a) # append the file to the close list
    # b = a.Get(folder) #open the directory in the root file
    Hist = a.Get(hist) # get your histogram by name
    if Hist == None : Hist = Root.TH1D()
    if Legend != 0:
      leg.AddEntry(Hist,Legend,"LP") # add a legend entry
    Hist.SetLineWidth(3)
    Hist.SetLineColor(col) #set colour
    Hist.SetBinContent(Hist.GetNbinsX() ,Hist.GetBinContent(Hist.GetNbinsX())+Hist.GetBinContent(Hist.GetNbinsX()+1))
    Hist.SetBinError(Hist.GetNbinsX() ,math.sqrt((Hist.GetBinError(Hist.GetNbinsX()))**2 + (Hist.GetBinError(Hist.GetNbinsX()+1))**2))
    Hist.SetBinContent(Hist.GetNbinsX()+1,0)
    if norm != 0:
       Hist.Scale(1.) #if not data normilse to the data by lumi, MC is by default weighted to 100pb-1, if you have changed this change here!
    return Hist

leg = Root.TLegend(0.1, 0.7, 0.4, 1.0)
leg.SetShadowColor(0)
leg.SetBorderSize(0)
leg.SetFillStyle(4100)
leg.SetFillColor(0)
leg.SetLineColor(0)
leg.SetShadowColor(0)
leg.SetBorderSize(0)
leg.SetFillStyle(4100)
leg.SetFillColor(0)
leg.SetLineColor(0)
prelim = Root.TLatex(0.15,0.92,"#scale[0.8]{CMS}")
prelim.SetNDC()
lumi = Root.TLatex(0.45,.82,"#scale[0.8]{#int L dt = 35 pb^{-1}, #sqrt{s} = 7 TeV}")
lumi.SetNDC()
c1 = Root.TCanvas("canvas","canname",1400,900)
mainPad = Root.TPad("","",0.01,0.25,0.99,0.99);
mainPad.SetNumber(1);
mainPad.SetBottomMargin(0.5);
mainPad.Draw();

BitList=["Bit15","Bit16","Bit17","Bit18","Bit19","Bit21","Bit20","Bit21",]
CorThresholds =   [16.,20.,36.,52.,68.,92.]
UnCorThresholds = [6.,10.,20.,30.,40.,60.]


# Conversion Table:
# L1 Uncorrected   L1 Corrected Threshold
# 6U                  16
# 8U                  16
# 10U                 20
# 14U                 28
# 18U                 32
# 20U                 36
# 30U                 52
# 40U                 68
# 50U                 80
# 60U                 92
# 90U                 128
def errorFun(x, par):
  return 0.5*par[0]*(1. + Root.TMath.Erf( (x[0] - par[1]) / (math.sqrt(2.)*par[2]) ))
def reBiner(Hist,minimum):
  """docstring for reBiner"""
  upArray = []
  nBins = -1
  for bin in range(0,Hist.GetNbinsX()):
    binC = 0
    if Hist.GetBinContent(bin) > minimum:
      upArray.append(Hist.GetBinLowEdge(bin+1))
      nBins+=1
    else:
      binC += Hist.GetBinContent(bin)
      if binC > minimum:
        upArray.append(Hist.GetBinLowEdge(bin+1))
        nBins+=1
        binC = 0
  upArray.append(Hist.GetBinLowEdge(Hist.GetNbinsX()))
  nBins+=1
  rebinList = array.array('d',upArray)
  print upArray
  print nBins
  return (nBins,rebinList)


# First Make Turn on curves
def MakeTurnOn(CorrHist = None, UnCorrHist = None , BitList = [] ,CorThresholds = [], UnCorThresholds = []):
  out = []
  for Trig,Cor,UnCor in zip(BitList,CorThresholds,UnCorThresholds):
    Nom = GetHist(CorrHist,"/",Trig,0,0,"Jet Threshold = %f"%(Cor))
    DeNom = GetHist(CorrHist,"/","RefJet",0,0,0)
    (i,bins)= reBiner(Nom,1.)
    a = Nom.Rebin(i,"a",bins)
    b = DeNom.Rebin(i,"b",bins)
    mg = Root.TMultiGraph()
    TurnOn = Root.TGraphAsymmErrors()
    TurnOn.Divide(a,b)
    TurnOn.SetMarkerColor(4)
    TurnOn.SetMarkerStyle(30)
    mg.Add(TurnOn)
    fermiFunction = Root.TF1("fermiFunction",errorFun,0.,1000.,3)
    fermiFunction.SetParameters(1.00,Cor,1.)
    fermiFunction.SetParNames("#epsilon","#mu","#sigma")
    TurnOn.Fit(fermiFunction,"%f"%(Cor),"%f"%(Cor),0.,100.)
    fermiFunction.SetLineColor(5)
    # fermiFunction.Draw("same")
    out.append(TurnOn)
    if UnCorrHist != None:
      Nom = GetHist(UnCorrHist,"/",Trig,0,0,"Jet Threshold = %f"%(UnCor))
      DeNom = GetHist(UnCorrHist,"/","RefJet",0,0,0)
      TurnOn2 = Root.TGraphAsymmErrors()
      (i,bins)= reBiner(Nom,20.)
      c = Nom.Rebin(i,"c",bins)
      d = DeNom.Rebin(i,"d",bins)
      TurnOn2.Divide(c,d)
      # TurnOn.Draw("ap same")
      mg.Add(TurnOn2)
      # TurnOn2.SetMarkerColor(6)
      TurnOn2.SetMarkerStyle(32)
      TurnOn2.GetXaxis().SetRangeUser(0.,100.)
      fermiFunction2 = Root.TF1("fermiFunction2",errorFun,0.,1000.,3)
      fermiFunction2.SetParameters(1.00,UnCor,1.)
      fermiFunction2.SetParNames("#epsilon","#mu","#sigma")
      TurnOn.Fit(fermiFunction,"%f"%(UnCor),"%i"%(int(UnCor)),0.,100.)
      # fermiFunction2.Draw("same")
    mg.Draw("ap")
    mg.GetXaxis().SetRangeUser(0.,100.)
    c1.SaveAs("TurnOnFor_%i.pdf"%(int(Cor)))
    c1.Clear()
    leg.Clear()
  return out


a = MakeTurnOn(CorrHist = "May22ReReo_MinBias.root", UnCorrHist = None, BitList = BitList,
                CorThresholds = CorThresholds, UnCorThresholds = UnCorThresholds)

multi = Root.TMultiGraph()
for b in a:
  multi.Add(b)
multi.Draw("alp")
c1.SaveAs("Multi.pdf")










#
#
#
#
#
# for hist in cfList:
#     leg.Clear()
#     June14_Alleta_allJets = GetHist("dec22ReReco.root","/",hist,1,0,"2010")
#     promptReco = GetHist("muon.root","/",hist,3,0,"2011")
#     promptReco.SetFillColor(0)
#     print type(June14_Alleta_allJets)
#     print type(promptReco)
#     if "TH2F" in str(type(June14_Alleta_allJets)):
#       c1.SetLogy(0)
#       c1.cd()
#       if "ResolutionAsFnOfeta" == hist:
#         June14_Alleta_allJets.GetYaxis().SetTitle("(Reco - L1 )/Reco (E_{T})")
#         June14_Alleta_allJets.GetXaxis().SetTitle("Reco #eta")
#       if "ResolutionAsFnOfpT" == hist:
#         June14_Alleta_allJets.GetYaxis().SetTitle("(Reco - L1 )/Reco (E_{T})")
#         June14_Alleta_allJets.GetXaxis().SetTitle("Reco E_{T}")
#       if "ResolutionAsFnOfeta" == hist:
#         promptReco.GetYaxis().SetTitle("(Reco - L1 )/Reco (E_{T})")
#         promptReco.GetXaxis().SetTitle("Reco #eta")
#       if "ResolutionAsFnOfpT" == hist:
#         promptReco.GetYaxis().SetTitle("(Reco - L1 )/Reco (E_{T})")
#         promptReco.GetXaxis().SetTitle("Reco E_{T}")
#
#       June14_Alleta_allJets.GetXaxis().SetRangeUser(0,150)
#       if hist == "ResolutionAsFnOfeta" : June14_Alleta_allJets.GetXaxis().SetRangeUser(-5.,5.)
#       if "RecoVs" in hist :June14_Alleta_allJets.GetYaxis().SetRangeUser(0,150)
#       June14_Alleta_allJets.Draw("COLZ")
#       c1.SaveAs("./muon//"+hist+"_June14ReReco.pdf")
#       c1.Clear()
#       promptReco.GetXaxis().SetRangeUser(0,150)
#       if hist == "ResolutionAsFnOfeta" : promptReco.GetXaxis().SetRangeUser(-5.,5.)
#       if "RecoVs" in hist :promptReco.GetYaxis().SetRangeUser(0,150)
#       promptReco.Draw("COLZ")
#       c1.SaveAs("./muon//"+hist+"_promptReco.pdf")
#       c1.Clear()
#       # Make Profile Histos
#       h1 = Root.TH1D()
#       h2 = Root.TH1D()
#       # h3 = Root.TH1D()
#       h1 = promptReco.ProfileX(hist+"_Profile_PromptReco",0,-1,"");
#       h2 = June14_Alleta_allJets.ProfileX(hist+"_Profile",0,-1,"");
#       # h3 = #nick.ProfileX(hist+"_Profile_#nick",0,-1,"");
#       h1.GetYaxis().SetRangeUser(-2.,2.)
#       h2.GetYaxis().SetRangeUser(-2.,2.)
#       # h3.GetYaxis().SetRangeUser(-2.,2.)
#       h1.SetLineColor(2)
#       h2.SetLineColor(2)
#       # h3.SetLineColor(2)
#       h1.Draw("")
#       c1.SaveAs("./muon//Profile_"+hist+"PromptReco.pdf")
#       c1.Clear()
#       h2.Draw("")
#       c1.SaveAs("./muon//Profile_"+hist+"_June14ReReco.pdf")
#     if "TH2F" not in str(type(June14_Alleta_allJets)):
#       if "Bit" in hist or "RefJet" == hist:
#         June14_Alleta_allJets.GetXaxis().SetRangeUser(0.,150.)
#         promptReco.GetXaxis().SetRangeUser(0.,150.)
#       if "ResolutionAsFnOfeta" == hist:
#         June14_Alleta_allJets.GetYaxis().SetTitle("(Reco - L1 )/Reco (E_{T})")
#         June14_Alleta_allJets.GetXaxis().SetTitle("Reco #eta")
#       if "ResolutionAsFnOfpT" == hist:
#         June14_Alleta_allJets.GetYaxis().SetTitle("(Reco - L1 )/Reco (E_{T})")
#         June14_Alleta_allJets.GetXaxis().SetTitle("Reco E_{T}")
#       if "RefJet" == hist:
#        June14_Alleta_allJets.GetXaxis().SetRangeUser(0.,100.)
#        promptReco.GetXaxis().SetRangeUser(0.,100.)
#        c1.SetLogy()
#       promptReco.SetFillColor(0)
#       June14_Alleta_allJets.SetFillColor(0)
#       June14_Alleta_allJets.DrawNormalized("hist")
#       promptReco.DrawNormalized("histsame")
#       c1.Update()
#       leg.Draw("same")
#       c1.SaveAs("./muon//Compare"+hist+".pdf")
#
#
#
# c1.SetLogy(0)
#
# c1.Clear()
#
# fit1Param = [6.,22.,35.,42.,75.]
# fit2Param = [15.,35.,50.,70.,90.]
#
# for a,b,c,d,e in zip(UnCor,Corr,UnCorName,fit1Param,fit2Param):
#   refHist_old = GetHist("dec22ReReco.root","/","RefJet",1,0,"reference collection")
#   Jet20U_old =  GetHist("dec22ReReco.root","/",a,1,0,"Jet20U")
#   refHist_new = GetHist("muon.root","/","RefJet",1,0,"reference collection")
#   Jet20U_new =  GetHist("muon.root","/",a,1,0,"Jet20U")
#   refHist_old.Rebin(4)
#   Jet20U_old.Rebin(4)
#   refHist_new.Rebin(4)
#   Jet20U_new.Rebin(4)
#   Jet20TurnOn = Root.TGraphAsymmErrors()
#   Jet20TurnOn.Divide(Jet20U_old,refHist_old)
#   Jet36TurnOn = Root.TGraphAsymmErrors()
#   Jet36TurnOn.Divide(Jet20U_new,refHist_new)
#   ##nickTurnOn = Root.TGraphAsymmErrors()
#   ##nickTurnOn.Divide(#nickTrig,#nickRef)
#   # def fit function
#   fermiFunction.SetParameters(1.00,d,1.)
#   fermiFunction.SetParNames("#epsilon","#mu","#sigma")
#
#   fermiFunction2 = Root.TF1("fermiFunction",errorFun,0.,1000.,3)
#   fermiFunction2.SetParameters(1.00,e,1.)
#   fermiFunction2.SetParNames("#epsilon","#mu","#sigma")
#
#   # Do the fitting
#   multiGraph = Root.TMultiGraph()
#   Root.gPad.SetGridy()
#   Root.gPad.SetGridx()
#   leg.Clear()
#   leg.SetShadowColor(0)
#   leg.SetBorderSize(0)
#   # leg.SetFillStyle(4100)
#   leg.SetFillColor(0)
#   leg.SetLineColor(0)
#   Jet20TurnOn.SetMarkerStyle(20)
#   Jet36TurnOn.SetMarkerStyle(21)
#   ##nickTurnOn.SetMarkerStyle(22)
#   ##nickTurnOn.SetMarkerColor(6)
#   ##nickTurnOn.SetLineColor(6)
#
#   Jet36TurnOn.GetXaxis().SetTitle("E_{T} UnCorrected")
#   Jet20TurnOn.GetXaxis().SetTitle("E_{T} UnCorrected")
#   multiGraph.Add(Jet20TurnOn)
#   multiGraph.Add(Jet36TurnOn)
#   # multiGraph.Add(##nickTurnOn)
#   leg.AddEntry(Jet20TurnOn,"2010","lp")
#   leg.AddEntry(Jet36TurnOn,"2011","lp")
#   # leg.AddEntry(##nickTurnOn,"#nick","pl")
#   Jet36TurnOn.SetLineColor(2)
#   Jet36TurnOn.SetMarkerColor(2)
#   c1.cd()
#   Jet20TurnOn.Draw("alp")
#   Jet20TurnOn.Fit(fermiFunction,"","",0.,100.)
#   Jet36TurnOn.Fit(fermiFunction2,"","",0.,100.)
#   # ##nickTurnOn.Fit(fermiFunction,"","",0.,100.)
#
#   Jet36TurnOn.GetXaxis().SetRangeUser(0.,150.)
#   Jet20TurnOn.GetXaxis().SetRangeUser(0.,150.)
#   Jet36TurnOn.GetYaxis().SetRangeUser(0.,1.6)
#   Jet20TurnOn.GetYaxis().SetRangeUser(0.,1.6)
#   ##nickTurnOn.GetYaxis().SetRangeUser(0.,1.)
#   # print Jet20TurnOn.GetListOfFunctions()
#   Root.gPad.Update()
#   # Jet20Stats = Jet20TurnOn.FindObject("stats")
#   Jet36TurnOn.Draw("alp")
#   Root.gPad.Update()
#   # print Jet20TurnOn.GetListOfFunctions().FindObject("stats")
#   # for i in Jet20TurnOn.GetListOfFunctions():
#     # print i.GetName()
#   # Jet36Stats = Jet36TurnOn.FindObject("stats");
#   # print Jet20Stats
#   # print Jet36Stats
#   # Jet20Stats.SetX1NDC(0.65)
#   # Jet36Stats.SetX2NDC(0.5)
#   # print type(Jet20Stats)
#   c1.Clear()
#   # c1.SetLogx()
#
#   Jet20TurnOn.GetXaxis().SetTitle("E_{T} UnCorrected GeV")
#   Jet20TurnOn.GetYaxis().SetRangeUser(0.,1.6)
#   Jet20TurnOn.Draw("ap")
#   c1.Update()
#   c1.SaveAs("./muon/"+a+"_Rereco.pdf")
#   c1.Clear()
#   # c1.SetLogx()
#   Jet36TurnOn.GetYaxis().SetRangeUser(0.,1.6)
#   Jet36TurnOn.GetXaxis().SetTitle("E_{T} Corrected GeV")
#   Jet36TurnOn.Draw("ap")
#   c1.Update()
#   c1.SaveAs("./muon/"+a+"_Promptreco.pdf")
#   c1.Clear()
#   # c1.SetLogx()
#   multiGraph.Draw("ap")
#   multiGraph.GetXaxis().SetRangeUser(5.,150.)
#   multiGraph.GetYaxis().SetRangeUser(0.,1.5)
#   multiGraph.GetXaxis().SetTitle("E_{T} ")
#   # Jet20Stats.Draw("same")
#   # Jet36Stats.Draw("samez")
#   leg.Draw("same")
#   c1.Update()
#   c1.SaveAs("./muon//"+a+b+".pdf")
# raw_input("")
#
#
#
# # RefJets              = new TH1F("RefJet", "RefJetEt",1000,0.,1000);
# # SingleJet16          = new TH1F("Bit15", "JetEt",1000,0.,1000);
# # SingleJet36          = new TH1F("Bit16", "JetEt",1000,0.,1000);
# # SingleJet52          = new TH1F("Bit17", "JetEt",1000,0.,1000);
# # SingleJet68          = new TH1F("Bit18", "JetEt",1000,0.,1000);
# # SingleJet92          = new TH1F("Bit19", "JetEt",1000,0.,1000);
# # SingleJet128         = new TH1F("Bit20", "JetEt",1000,0.,1000);
# # SingleJet80_Central  = new TH1F("Bit21", "JetEt",1000,0.,1000);
# # if(ReturnMatchedQuantity(matchedJet,Et)> 16.){ SingleJet16        ->Fill(recoJet_->et[0],wgt); } // now goes to SingleJet16
# # if(ReturnMatchedQuantity(matchedJet,Et)> 36.){ SingleJet36        ->Fill(recoJet_->et[0],wgt); } // now goes to SingleJet36
# # if(ReturnMatchedQuantity(matchedJet,Et)> 52.){ SingleJet52        ->Fill(recoJet_->et[0],wgt); } // now goes to SingleJet52
# # if(ReturnMatchedQuantity(matchedJet,Et)> 68.){ SingleJet68        ->Fill(recoJet_->et[0],wgt); } // now goes to SingleJet68
# # if(ReturnMatchedQuantity(matchedJet,Et)> 92.){ SingleJet92        ->Fill(recoJet_->et[0],wgt); } // now goes to SingleJet92
# # if(ReturnMatchedQuantity(matchedJet,Et)> 128.){ SingleJet128      ->Fill(recoJet_->et[0],wgt); } // now goes to SingleJet128
# # "Bit15","Bit17","Bit18","Bit19","Bit21"
# # Make Plot for Manfred:
# refHist=GetHist("muon.root","/","RefJet",1,0,"reference collection")
#
#
#
#
# c1.SaveAs("./muon/nologX_run2011.pdf")
# raw_input()

