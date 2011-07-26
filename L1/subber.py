#!/vols/sl5_exp_software/cms/slc5_amd64_gcc434/cms/cmssw-patch/CMSSW_4_1_3_patch3/external/slc5_amd64_gcc434/bin/python

import ROOT as r
import sys, os
print " Use Is python subber.py Analysis InPutFileList OutPutDir UseUnCorrectedThresholds Trigger" 
# inpu = file("./dec22List.txt","r")
# inpu = file("./incompList.txt","r")
# inpu = file("./HTDataSet.txt","r")
# inpu = file("./Pf.txt","r")
# inpu = file("./newList.txt","r")
# inpu = file("./muList.txt","r")
# inpu = file("./Un.txt","r")
# inpu = file("./Dn.txt","r")
inpu = sys.argv[2]
flist = file(inpu).readlines()
print flist
print "Will submit", len(flist) , "jobs"

for i in range(0,len(flist)):
  os.system("qsub -q hepmedium.q subScript.sh " + sys.argv[1] +" "+sys.argv[2] +" " + sys.argv[3] + " " + sys.argv[4] + " "+ sys.argv[5] + " " + str(i))
