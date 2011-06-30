#!/usr/bin/env python

'''Created by Bryn Mathias
bryn.mathias@cern.ch
'''
import errno
import array
import ROOT as Root
import math
from time import strftime
import os, commands, sys
# from PlottingFunctions import *
# Root.gROOT.SetBatch(True) # suppress the creation of canvases on the screen... much much faster if over a remote connection

Root.gROOT.ProcessLine(".L ~/WokringDir/DevVersionSUSYv2/bryn/python/tdrstyle.C+")
Root.gStyle.SetLineStyleString(11,"50 25")
Root.gROOT.SetStyle("Plain")
Root.gStyle.SetPalette(1)
def ensure_dir(path):
    try:
      os.makedirs(path)
    except OSError as exc: # Python >2.5
      if exc.errno == errno.EEXIST:
        pass
      else: raise


# Global Close List
closeList = []
intlumi = 189.

leg = Root.TLegend(0.55, 0.55, 0.85, 0.65)
leg.SetShadowColor(0)
leg.SetBorderSize(0)
leg.SetFillStyle(4100)
leg.SetFillColor(0)
leg.SetLineColor(0)

def GetAllhistos(inFile):
  temp = Root.TFile.Open(inFile)
  DirKeys = temp.GetListOfKeys()
  print DirKeys
  # histKeys =
  histList = [k.GetName() for k in (DirKeys[0].ReadObj()).GetListOfKeys()]
  return histList

def GetAllSubFiles(inFile):
  """docstring for GetAllSubFiles"""
  temp = Root.TFile.Open(inFile)
  DirKeys = temp.GetListOfKeys()
  DirList = []
  for d in DirKeys:
    DirList.append(d.GetName())
  return DirList

def GethistFromFolder(DataSetName,folder,hist,col,norm,Legend):
    a = Root.TFile.Open(DataSetName) #open the file
    closeList.append(a) # append the file to the close list
    b = a.Get(folder) #open the directory in the root file
    hist = b.Get(hist) # get your histogram b4y name
    if hist == None : hist = Root.TH1D()
    if Legend != 0:
      leg.AddEntry(hist,Legend,"LP") # add a legend entry
    hist.SetLineWidth(3)
    hist.SetLineColor(col) #set colour
    hist.SetBinContent(hist.GetNbinsX() ,hist.GetBinContent(hist.GetNbinsX())+hist.GetBinContent(hist.GetNbinsX()+1))
    hist.SetBinError(hist.GetNbinsX() ,math.sqrt((hist.GetBinError(hist.GetNbinsX()))**2 + (hist.GetBinError(hist.GetNbinsX()+1))**2))
    hist.SetBinContent(hist.GetNbinsX()+1,0)
    if norm != 0:
       hist.Scale(1/hist.Integral(0,-1)) #if not data normilse to the data by lumi, MC is by default weighted to 100pb-1, if you have changed this change here!
    return hist

def MakeDataFail(hist,low,high):
  """docstring for MakeDataFail"""
  newHist = hist.Clone()
  for bin in range(0,hist.GetNbinsX()):
    if hist.GetBinLowEdge(bin) >= low and hist.GetBinLowEdge(bin+1) <= high:
      # print "low edge" , hist.GetBinLowEdge(bin) ,"High Edge", hist.GetBinLowEdge(bin+1)
      newHist.SetBinContent(bin,hist.GetBinContent(bin))
    else: newHist.SetBinContent(bin,0.)
  # print "next hist"
  hist.Delete()
  return newHist
  pass

outdir = "./RatioPlots/"
ensure_dir(outdir)
# DataPass =    GethistFromFolder("~/Desktop/50/AK5Calo_Jets.root","AllCutscombined","HT_after_alphaT_all",1,0,"CMSSW_4_1_X")
# DataPass.Add( GethistFromFolder("~/Desktop/36/AK5Calo_Jets.root","275_325","HT_after_alphaT_all",1,0,0),275.,325.)
# DataPass.Add( GethistFromFolder("~/Desktop/43/AK5Calo_Jets.root","325_375","HT_after_alphaT_all",1,0,0) , 325.,375. )
#
# hist42 = GethistFromFolder("~/Desktop/42.root","AllCutscombined",hist,3,0,"CMSSW_4_2_X")
# hist42.Add( GethistFromFolder("~/Desktop/42_37.root","275_325",hist,3,0,0) )
# hist42.Add( GethistFromFolder("~/Desktop/42_43.root","325_375",hist,3,0,0) )

prompt =  GethistFromFolder("~/Desktop/VersionCompare/50/Prompt.root","AllCutscombined","BiasedDeltaPhi_after_alphaT_55_all",1,0,"Prompt")
prompt.Add(GethistFromFolder("~/Desktop/VersionCompare/36/Prompt.root","275_325","BiasedDeltaPhi_after_alphaT_55_all",1,0,0))
prompt.Add(GethistFromFolder("~/Desktop/VersionCompare/43/Prompt.root","325_375","BiasedDeltaPhi_after_alphaT_55_all",1,0,0))
ReReco =  GethistFromFolder("~/Desktop/VersionCompare/50/ReReco.root","AllCutscombined","BiasedDeltaPhi_after_alphaT_55_all",3,0,"ReReco")
ReReco.Add(GethistFromFolder("~/Desktop/VersionCompare/36/ReReco.root","275_325","BiasedDeltaPhi_after_alphaT_55_all",3,0,0))
ReReco.Add(GethistFromFolder("~/Desktop/VersionCompare/43/ReReco.root","325_375","BiasedDeltaPhi_after_alphaT_55_all",3,0,0))

EWK =   GethistFromFolder("~/Desktop/VersionCompare/50/AK5Calo_TTbar.root","AllCutscombined","BiasedDeltaPhi_after_alphaT_55_all",6,0,"EWK")
EWK.Add(GethistFromFolder("~/Desktop/VersionCompare/36/AK5Calo_TTbar.root","275_325","BiasedDeltaPhi_after_alphaT_55_all",1,0,0))
EWK.Add(GethistFromFolder("~/Desktop/VersionCompare/43/AK5Calo_TTbar.root","325_375","BiasedDeltaPhi_after_alphaT_55_all",1,0,0))
EWK.Add(GethistFromFolder("~/Desktop/VersionCompare/50/AK5Calo_SingleTop.root","AllCutscombined","BiasedDeltaPhi_after_alphaT_55_all",1,0,0))
EWK.Add(GethistFromFolder("~/Desktop/VersionCompare/36/AK5Calo_SingleTop.root","275_325","BiasedDeltaPhi_after_alphaT_55_all",1,0,0))
EWK.Add(GethistFromFolder("~/Desktop/VersionCompare/43/AK5Calo_SingleTop.root","325_375","BiasedDeltaPhi_after_alphaT_55_all",1,0,0))
EWK.Add(GethistFromFolder("~/Desktop/VersionCompare/50/AK5Calo_WJets.root","AllCutscombined","BiasedDeltaPhi_after_alphaT_55_all",1,0,0))
EWK.Add(GethistFromFolder("~/Desktop/VersionCompare/36/AK5Calo_WJets.root","275_325","BiasedDeltaPhi_after_alphaT_55_all",1,0,0))
EWK.Add(GethistFromFolder("~/Desktop/VersionCompare/43/AK5Calo_WJets.root","325_375","BiasedDeltaPhi_after_alphaT_55_all",1,0,0))
EWK.Add(GethistFromFolder("~/Desktop/VersionCompare/50/AK5Calo_Zinv.root","AllCutscombined","BiasedDeltaPhi_after_alphaT_55_all",1,0,0))
EWK.Add(GethistFromFolder("~/Desktop/VersionCompare/36/AK5Calo_Zinv.root","275_325","BiasedDeltaPhi_after_alphaT_55_all",1,0,0))
EWK.Add(GethistFromFolder("~/Desktop/VersionCompare/43/AK5Calo_Zinv.root","325_375","BiasedDeltaPhi_after_alphaT_55_all",1,0,0))

prompt.Rebin(4)
ReReco.Rebin(4)
EWK.Rebin(4)

c1 = Root.TCanvas("canvas","canname",1200,1200)

c1.cd()
prompt.DrawNormalized("h")
ReReco.DrawNormalized("psame")
EWK.DrawNormalized("hsame")

leg.Draw("same")
c1.SaveAs("./PromptVsReRecoBiasDphiCombinedFiles.pdf")

# fit = Root.TF1()
#
# c1 = Root.TCanvas("canvas","canname",1200,1200)
# # mainPad = Root.TPad("","",0.01,0.25,0.99,0.99)
# graph =Root.TGraphAsymmErrors()
# rebin = array.array("d",[275.,325.,375.,475.,575.,675.,775.,875.,2000.])
# a = DataPass.Rebin(8,"a",rebin)
# b = DataFail.Rebin(8,"b",rebin)
# c1.SetLogy()
# for bin in range(0,a.GetNbinsX()+1):
#   print "Pass Low",a.GetBinLowEdge(bin),"high", a.GetBinLowEdge(bin+1), "Content", a.GetBinContent(bin)
#   print "Fail Low",b.GetBinLowEdge(bin),"high", b.GetBinLowEdge(bin+1), "Content", b.GetBinContent(bin)
#   if b.GetBinContent(bin) > 0.:
#     print "Ratio",a.GetBinLowEdge(bin),"high", a.GetBinLowEdge(bin+1), "Content", a.GetBinContent(bin)/b.GetBinContent(bin)
#
# # for bin in range(0, DataPass.GetNbinsX()+1 ):
# #   print "Low",DataPass.GetBinLowEdge(bin),"high", DataPass.GetBinLowEdge(bin+1), "COntent", DataPass.GetBinContent(bin)
# #   print "Low",DataFail.GetBinLowEdge(bin),"high", DataFail.GetBinLowEdge(bin+1), "COntent", DataFail.GetBinContent(bin)
# #   if DataFail.GetBinContent(bin) > 0.:
# #     print "Low",DataFail.GetBinLowEdge(bin),"high", DataFail.GetBinLowEdge(bin+1), "COntent", DataPass.GetBinContent(bin)/DataFail.GetBinContent(bin)
# graph.Divide(a,b)
# a.Draw("hist")
# c1.SaveAs(outdir+"/passingAlphaT.pdf")
# b.Draw("hist")
# c1.SaveAs(outdir+"/FailingAlphaT.pdf")
# a.Divide(b)
# FPlot = Root.TGraphAsymmErrors(a)
#
#
# inf = float('inf')
#
# for bin in range (1,a.GetNbinsX()+1):
#
#   # if FPlot.GetErrorYhigh(bin)  > 0.  : FPlot.SetPointEYhigh(bin,graph.GetErrorYhigh(bin))
#   # if FPlot.GetErrorYlow(bin)  >0.   : FPlot.SetPointEYlow(bin, graph.GetErrorYhigh(bin))
#   print "error should be on point" , bin, "to", "YUpEror",graph.GetErrorYhigh(bin),"LowYError",graph.GetErrorYlow(bin)
#   print "error is be on point" , bin, "to", "YUpEror",FPlot.GetErrorYhigh(bin),"LowYError",FPlot.GetErrorYlow(bin)
#   #
#   # FPlot.SetPointEYlow(bin,graph.GetErrorYlow(bin))
# c1.Clear()
#
# # FPlot.GetYaxis().SetRangeUser(0.0000000000,0.0000)
# FPlot.Draw("ap")
# FPlot.GetYaxis().SetTitle("R(#alpha_{T})")
# FPlot.GetXaxis().SetTitle("H_{T}")
# # FPlot.GetYaxis().SetRangeUser(0.0000000000,0.0000)
# c1.Update()
# # hist41Proj2.Draw("hsame")
# # hist41.DrawNormalized("hsame")
# # hist42.GetXaxis().SetTitle("NJets")
# c1.SaveAs(outdir+"/FinalPlot.pdf")
# raw_input()
#
# leg.Clear()
# for a in closeList:
#   a.Close()