#!/usr/bin/env python

'''Created by Bryn Mathias
bryn.mathias@cern.ch
'''
import errno

import ROOT as Root
import math
from time import strftime
import os, commands, sys
import array
# from PlottingFunctions import *
Root.gROOT.SetBatch(True) # suppress the creation of canvases on the screen... much much faster if over a remote connection

# Root.gROOT.ProcessLine(".L ./tdrstyle.C+")
# Root.gStyle.SetLineStyleString(11,"50 25")
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

leg = Root.TLegend(0.65, 0.45, 0.97, 0.65)
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
       hist.Scale(intlumi/100.) #if not data normilse to the data by lumi, MC is by default weighted to 100pb-1, if you have changed this change here!
    return hist


outdirTmp = "./MHTX_Triggers/MHT/"
histList = ["HT_all","AlphaT_all","MHT_all","HT_2","AlphaT_2","MHT_2","HT_3","AlphaT_3","MHT_3"]

for Thresh in ["50","43","36"]:
  for hist in histList:
    print Thresh, hist
    outdir = outdirTmp
    outdir = outdirTmp+Thresh
    print outdir
    ensure_dir(outdir)
    HT_Trigger =    GethistFromFolder("./MuHad_MHT_"+Thresh+".root","HT_Trigger",hist,1,0,"Data")
    Cross_Trigger = GethistFromFolder("./MuHad_MHT_"+Thresh+".root","Cross_Trigger",hist,3,0,"+5GeV")
    HT_Trigger.GetXaxis().SetRangeUser(0.4,1.5)
    Cross_Trigger.GetXaxis().SetRangeUser(0.4,1.5)
    if "MHT" in hist:
      HT_Trigger.GetXaxis().SetRangeUser(0.,450.)
      Cross_Trigger.GetXaxis().SetRangeUser(0.,450.)
    c1 = Root.TCanvas("canvas"+hist+Thresh,"canname"+hist+Thresh,1200,1200)
    c1.cd(1)
    c1.SetLogy()
    HT_Trigger.Draw()
    Cross_Trigger.Draw("same")
    c1.SaveAs(outdir+"/"+hist+"RAW.pdf")
    c1.Clear()
    c1.SetLogy(0)
    TurnOn = Root.TGraphAsymmErrors()
    TurnOn.Divide(Cross_Trigger,HT_Trigger)
    TurnOn.Draw("alp")
    if "AlphaT_" in hist:
      TurnOn.GetXaxis().SetRangeUser(0.4,0.7)
      TurnOn.GetXaxis().SetTitle("#alpha_{T}")

    c1.SaveAs(outdir+"/"+hist+".png")
    c1.SaveAs(outdir+"/"+hist+".pdf")
    graph =Root.TGraphAsymmErrors()
    rebin = array.array("d",[0.,0.55,HT_Trigger.GetBinLowEdge(HT_Trigger.GetNbinsX())+HT_Trigger.GetBinWidth(1)])
    a = HT_Trigger.Rebin(2,"a",rebin)
    b = Cross_Trigger.Rebin(2,"b",rebin)
    a.Draw("h")
    b.Draw("sameh")
    c1.SetLogy()
    c1.SaveAs(outdir+"/"+hist+"ReBin.pdf")
    c1.Clear()
    graph.Divide(b,a)
    graph.Draw("ap")
    graph.GetYaxis().SetRangeUser(0.,1.)
    graph.SetMarkerColor(3)
    graph.SetLineColor(3)
    graph.SetLineWidth(3)
    c1.SetLogy(0)
    c1.SaveAs(outdir+"/"+hist+"Cumlative.pdf")
    b.Divide(a)
    b.Draw("h")
    b.GetYaxis().SetRangeUser(0.,1.)
    c1.SaveAs(outdir+"/"+hist+"ReBinRatio.pdf")
    leg.Clear()
    for a in closeList:
      a.Close()


a = GethistFromFolder("~/Desktop/AK5Calo_LM0_SUSY_sftsht_7TeV_pythia6_Spring11_PU_S1_START311_V1G1_v1.root","LMXTest","EffectiveMass_after_alphaT_55_all",1,0,"Data")
b = GethistFromFolder("~/Desktop/AK5Calo_LM1_SUSY_sftsht_7TeV_pythia6_Spring11_PU_S1_START311_V1G1_v1.root","LMXTest","EffectiveMass_after_alphaT_55_all",1,0,"Data")

print a.Integral(0,-1)
print b.Integral(0,-1)

