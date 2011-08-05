import ROOT as r
import sys


inpu = sys.argv[2]
Analysis = sys.argv[1]
outFile = sys.argv[3]
useUnCor = sys.argv[4]
outDir = sys.argv[3]
Trigger = sys.argv[5]

flist = file(inpu).readlines()
print Analysis, inpu, outFile, useUnCor, Trigger
idx = sys.argv[6]
#print int(idx) , "INDEX!!!!"
f = flist[int(idx)]
fi = f.strip()
print "file" , [f][0:-1] , "outdir" , str(outDir)+"/"+str(fi).rpartition("/")[2]

print Analysis , inpu, outFile, useUnCor, Trigger, idx
r
#m.run(910000,str(outFile),bool(useUnCor))
#m.Delete()






def Run():
  """docstring for Run"""
  r.gROOT.ProcessLine(".x ~/cms03/CMSSW_4_2_5/src/UserCode/L1TriggerDPG/macros/initL1Analysis.C")
  r.gROOT.ProcessLine(".L "+str(self.Analysis+"+"))
  if Analysis == "L1EnergySumAnalysis": m = r.L1EnergySumAnalysis()
  if Analysis == "L1JetAnalysis.C" : m = r.L1JetAnalysis()
  m.Open(f[0:-1])
  m.run(-1,str(outDir)+"/"+str(fi).rpartition("/")[2], bool(useUnCor),Trigger)
  pass