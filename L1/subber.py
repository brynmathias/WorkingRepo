#!/vols/sl5_exp_software/cms/slc5_amd64_gcc434/cms/cmssw-patch/CMSSW_4_1_3_patch3/external/slc5_amd64_gcc434/bin/python

import ROOT as r
import sys, os
from optparse import OptionParser
print " Use Is python subber.py Analysis InPutFileList OutPutDir UseUnCorrectedThresholds Trigger"
# inpu = file("./dec22List.txt","r")
# inpu = file("./incompList.txt","r")
# inpu = file("./HTDataSet.txt","r")
# inpu = file("./Pf.txt","r")
# inpu = file("./newList.txt","r")
# inpu = file("./muList.txt","r")
# inpu = file("./Un.txt","r")
# inpu = file("./Dn.txt","r")



class Analysis(object):
  """docstring for Analysis"""
  def __init__(self, arg):
    super(Analysis, self).__init__()
    self.arg = arg

  def Parser(self):
    """docstring for Parser"""
    parser = OptionParser()
    parser.add_option("-a","--analysis", dest = "analysis", help = " Choose which analysis we load into root")
    parser.add_option("-U","--uncorrected",dest="UNCORR",default = False, help = "Choose to use uncorrected offline Jet energies",type = "bool")
    parser.add_option("-o","--outputDir",dest = "OUTDIR", default = "./tmp/", help = "Choose the output dir", type = "string")
    parser.add_option("-i","--input",dest = "INPUTFILE", help = "Choose input file list",type = "string")
    parser.add_option("-n","--nFilesPerJob", default = 1, help = "Choose how many files to run per sub job",type = "int")
    parser.add_option("-t","--trigger",default = " ", help = "Choose trigger path", type = "string")

    def Run(job):
      f = (file(self.input).readlines())[job]
      """docstring for Run"""
      r.gROOT.ProcessLine(".x ~/cms03/CMSSW_4_2_5/src/UserCode/L1TriggerDPG/macros/initL1Analysis.C")
      r.gROOT.ProcessLine(".L "+str(self.analysis+"+"))
      if self.analysis == "L1EnergySumAnalysis": m = r.L1EnergySumAnalysis()
      if self.analysis == "L1JetAnalysis.C" : m = r.L1JetAnalysis()
      m.Open(f[0:-1])
      m.run(-1,self.outputDir+"/"+str(fi).rpartition("/")[2], self.uncorrected,Trigger)
      pass

for i in range(0,len(flist)):
  os.system("qsub -q hepmedium.q subScript.sh " + sys.argv[1] +" "+sys.argv[2] +" " + sys.argv[3] + " " + sys.argv[4] + " "+ sys.argv[5] + " " + str(i))

source /home/hep/bm409/cms03/CMSSW_4_2_5/src/UserCode/L1TriggerDPG/scripts/setup.sh
cd /home/hep/bm409/cms03/cafScripts/
python ./pyRun.py $@














