{
gROOT->ProcessLine(".x /afs/cern.ch/cms/CAF/CMSCOMM/COMM_TRIGGER/l1analysis/cmssw/CMSSW_3_6_1_patch4/src/UserCode/L1TriggerDPG/macros/initL1Analysis.C");
//gROOT->ProcessLine(".x /afs/cern.ch/cms/CAF/CMSCOMM/COMM_TRIGGER/l1analysis/cmssw/CMSSW_3_6_1_patch4/src/UserCode/L1TriggerDPG/macros/initL1Analysis.C");
gROOT->ProcessLine(".L /afs/cern.ch/cms/CAF/CMSCOMM/COMM_TRIGGER/bm409/L1JetAnalysis.C+");
//L1JetAnalysis m("/tmp/bm409/L1Tree.root");
cout << " Have imported L1JetAnalysis!!!" <<endl;
L1JetAnalysis m("/castor/cern.ch/user/b/bm409/L1Tree.root");
//m.OpenWithList("./Express_L1JetNtuple_160383-160957_list.txt");
m.run(1000000,1500000,"./file3.root");
}
