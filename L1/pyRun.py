import ROOT as r
import sys

#inpu = file("./dec22List.txt","r")
inpu = file("./incompList.txt","r")
#inpu = file("./HTDataSet.txt","r")

# inpu = file("./Pf.txt","r")
# inpu = file("./newList.txt","r")
# inpu = file("./muList.txt","r")
#inpu = file("./Un.txt","r")
#inpu = file("./Dn.txt","r")
flist = inpu.readlines()
idx = sys.argv[1]
Analysis = sys.argv[2]

f =  flist[int(idx)]
print Analysis
outDir = sys.argv[3]
#r.gROOT.ProcessLine(".x ~/cms03/CMSSW_4_2_4/src/UserCode/L1TriggerDPG/macros/initL1Analysis.C")
fi = f.strip()
print "file \ " ,fi , "outFile name" , str(outDir)+"/"+str(fi).rpartition("/")[2]
#r.gROOT.ProcessLine(".L ./"+str(Analysis) +".C+")
#if Analysis == "L1EnergySumAnalysis": m = r.L1EnergySumAnalysis()
#if Analysis == "L1JetAnalysis" : m = r.L1JetAnalysis()
#if Analysis == "L1JetAnalysis_2011" : m = r.L1JetAnalysis_2011()
#m.Open(fi)
#m.run(-1,str(outDir)+"/"+str(fi).rpartition("/")[2])
#m.Delete()
