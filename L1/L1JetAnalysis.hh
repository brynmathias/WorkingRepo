#include "L1Ntuple.h"
#include "hist.C"
#include "Style.C"
#include "TMath.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include <TGraphAsymmErrors.h>
#include <TF1.h>
#include <TLorentzVector.h>

class L1JetAnalysis : public L1Ntuple
{
  public :

    //constructor
  L1JetAnalysis(std::string filename) : L1Ntuple(filename) {}
  L1JetAnalysis() {}
  ~L1JetAnalysis() {}

    //main function macro : arguments can be adpated to your need
  void run(Long64_t nevents, TString outputname , bool UnCorThresholds);

  private :
  bool PassTrig(int ib,int bx);
  bool MatchJet(int RecoJetIdx);
  bool PassHLT( TString TrigBit);
  bool LooseID( int Jet );
  std::pair <int,int> ReturnMatchedJet(int RecoJetIdx);
  void BookHistos();
  double ReturnMatchedQuantity( std::pair<int,int> matchJet,int Quantity);
  virtual double deltaPhi(double phi1, double phi2);
  virtual double deltaR(double eta1, double phi1, double eta2, double phi2);
    //your private methods can be declared here
// histos
  TH1F *RefJets;
  TH1F *Bit15;
  TH1F *Bit16;
  TH1F *Bit17;
  TH1F *Bit18;
  TH1F *Bit19;
  TH1F *Bit20;
  TH1F *Bit21;
  TH1F *CandidateJets30Gev;
  TH2F *RecoVsl1HFE;
  TH2F *ResolutionEtHFE;
  TH1F *ResolutionHFE;
  TH2F *RecoVsl1HFH;
  TH2F *ResolutionEtHFH;
  TH1F *ResolutionHFH;
  TH1F *ResolutionHE;
  TH2F *ResolutionEtHE;
  TH2F *RecoVsl1HE;
  TH1F *ResolutionEE;
  TH2F *ResolutionEtEE;
  TH2F *RecoVsl1EE;
  TH1F *ResolutionHB;
  TH2F *ResolutionEtHB;
  TH2F *RecoVsl1HB;
  TH1F *ResolutionEB;
  TH2F *ResolutionEtEB;
  TH2F *RecoVsl1EB;
  TH1F *EMF;
  TH2I *timeMap;
  TH2F *ResolutionAsFnOfpT;
  TH2F *ResolutionAsFnOfeta;
};

double L1JetAnalysis::deltaPhi(double phi1, double phi2) {
  double result = phi1 - phi2;
  if(fabs(result) > 9999) return result;
  while (result > TMath::Pi()) result -= 2*TMath::Pi();
  while (result <= -TMath::Pi()) result += 2*TMath::Pi();
  return result;
}

double L1JetAnalysis::deltaR(double eta1, double phi1, double eta2, double phi2) {
  double deta = eta1 - eta2;
  double dphi = deltaPhi(phi1, phi2);
  return sqrt(deta*deta + dphi*dphi);
}


bool L1JetAnalysis::MatchJet(int RecoJetIdx){

  for(unsigned int i = 0; i < l1extra_->nCenJets; i++){
    if( deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1extra_->cenJetEta[i], l1extra_->cenJetPhi[i]) <= 0.5)
    {
      return true;
    }
  }

  for(unsigned int i = 0; i < l1extra_->nTauJets; i++){
    if( deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1extra_->tauJetEta[i], l1extra_->tauJetPhi[i]) <= 0.5)
    {
      return true;
    }
  }

  for(unsigned int i = 0; i < l1extra_->nFwdJets; i++){
    if( deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1extra_->fwdJetEta[i], l1extra_->fwdJetPhi[i]) <= 0.5)
    {
      return true;
    }
  }
  return false;
}

std::pair<int,int> L1JetAnalysis::ReturnMatchedJet(int RecoJetIdx){

  for(unsigned int i = 0; i < l1extra_->nCenJets; i++){
    if( deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1extra_->cenJetEta[i], l1extra_->cenJetPhi[i]) <= 0.5)
    {
      std::pair <int,int> matchedJet(0,i);
      return matchedJet;
    }
  }

  for(unsigned int i = 0; i < l1extra_->nTauJets; i++){
    if( deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1extra_->tauJetEta[i], l1extra_->tauJetPhi[i]) <= 0.5)
    {
      std::pair <int,int> matchedJet(1,i);
      return matchedJet;
    }
  }

  for(unsigned int i = 0; i < l1extra_->nFwdJets; i++){
    if( deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1extra_->fwdJetEta[i], l1extra_->fwdJetPhi[i]) <= 0.5)
    {
      std::pair <int,int> matchedJet(2,i);
      return matchedJet;
    }
  }
  std::pair <int,int> matchedJet(-1,-1);
  return matchedJet;
}

bool L1JetAnalysis::PassTrig(int bit, int bx) {
  if(bit<64) { if((gt_->tw1[bx]>>bit)&1) { return true; } else return false; }
  if(bit<128) { if((gt_->tw2[bx]>>(bit-64))&1) { return true;} else return false;}
  else { if((gt_->tt[bx]>>(bit-1000))&1) { return true;} else return false;}
}


double L1JetAnalysis::ReturnMatchedQuantity( std::pair<int,int> matchJet,int Quantity){
  if(Quantity == 1){
    if(matchJet.first==0){
      return l1extra_->cenJetEt[matchJet.second];
    }
    if(matchJet.first==1){
      return l1extra_->tauJetEt[matchJet.second];
    }
    if(matchJet.first ==2 ){
      return l1extra_->fwdJetEt[matchJet.second];
    }
    if(matchJet.first==-1){
      return false;}
    }

    if(Quantity==2){
      switch(matchJet.first){
        if(matchJet.first==0){
          return l1extra_->cenJetEta[matchJet.second];
        }
        if(matchJet.first==1){
          return l1extra_->tauJetEta[matchJet.second];
        }
        if(matchJet.first==2){
          return l1extra_->fwdJetEta[matchJet.second];
        }
        if(matchJet.first==-1){
          return false;
        }
      }
    }

    if(Quantity==3){
      switch(matchJet.first){
        if(matchJet.first==0){
          return l1extra_->cenJetPhi[matchJet.second];
        }
        if(matchJet.first==1){
          return l1extra_->tauJetPhi[matchJet.second];
        }
        if(matchJet.first==2){
          return l1extra_->fwdJetPhi[matchJet.second];
        }
        if(matchJet.first==-1){
          return false;
        }
      }
    }
    return false;
  }

  bool L1JetAnalysis::PassHLT( TString TrigBit){
  return  std::find(event_->hlt.begin(),event_->hlt.end(),TrigBit)!=event_->hlt.end();
  }

  bool L1JetAnalysis::LooseID( int Jet ){
    if( ( recoJet_->eEMF[Jet]>0.01  ) &&  ( recoJet_->n90hits[Jet] > 1 ) && ( recoJet_->fHPD[Jet] < 0.98 ) ) return true;
    else return false;
  }




// int L1JetAnalysis::ReturnRecoObjectMatchedL1(int CenJetIndex,int Collection){
//
//   if(Collection == 0){
//     for(unsigned int i = 0; i < (recoJet_->et).size(); i++){
//       if((l1extra_->cenJetEta).size() > 0 && deltaR(recoJet_->eta[i], recoJet_->phi[i], l1extra_->cenJetEta[CenJetIndex], l1extra_->cenJetPhi[CenJetIndex]) <= 0.5)
//       {
//         return i;
//         }else return -1;
//       }
//     }else if (Collection == 1){
//
//       for(unsigned int i = 0; i < (recoJet_->et).size(); i++){
//         if( (l1extra_->tauJetEta).size() >0 && deltaR(recoJet_->eta[i], recoJet_->phi[i], l1extra_->tauJetEta[CenJetIndex], l1extra_->tauJetPhi[CenJetIndex]) <= 0.5)
//         {
//           return i;
//           }else return -1;
//         }
//       }else if ( Collection == 2 ){
//         for(unsigned int i = 0; i < (recoJet_->et).size(); i++){
//           if( (l1extra_->fwdJetEta).size() >0 && deltaR(recoJet_->eta[i], recoJet_->phi[i], l1extra_->fwdJetEta[CenJetIndex], l1extra_->fwdJetPhi[CenJetIndex]) <= 0.5)
//           {
//             return i;
//             }else return -1;
//           }
//         }
//       return -1;
//    }
//
//
//       int L1JetAnalysis::MaxL1Type(){
//
//         double a = 0.;
//         double b = 0.;
//         double c = 0.;
//
//       if((l1extra_->cenJetEt).size() > 0) a = l1extra_->cenJetEt[0];
//       if((l1extra_->tauJetEt).size() > 0) b = l1extra_->tauJetEt[0];
//       if((l1extra_->fwdJetEt).size() > 0) c = l1extra_->fwdJetEt[0];
//
//
//         if(max(a,b) >c){
//           return ( a >b ? 0 : 1 );
//         }
//         else return 2;
//       }
//
//
//       double L1JetAnalysis::MaxL1Et(){
//         double a = 0.;
//         double b = 0.;
//         double c = 0.;
//
//       if((l1extra_->cenJetEt).size() > 0) a = l1extra_->cenJetEt[0];
//       if((l1extra_->tauJetEt).size() > 0) b = l1extra_->tauJetEt[0];
//       if((l1extra_->fwdJetEt).size() > 0) c = l1extra_->fwdJetEt[0];
//         if(max(a,b) >c){
//           return ( a > b ? a : b );
//         }
//         else return c;
//       }



