#include "L1Ntuple.h"
#include "hist.C"
#include "Style.C"
#include "TMath.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include <TGraphAsymmErrors.h>
#include <TF1.h>
#include <TLorentzVector.h>
#include <iostream>
class L1JetAnalysis : public L1Ntuple
{
  public :

    //constructor
  L1JetAnalysis(std::string filename) : L1Ntuple(filename) {}
  L1JetAnalysis() {}
  ~L1JetAnalysis() {}

    //main function macro : arguments can be adpated to your need
  void run(Long64_t nevents, TString outputname ,TString TriggerBit);

  private :
  bool PassTrig(int ib,int bx);
  bool MatchJet(int RecoJetIdx);
  bool PassHLT( TString TrigBit);
  bool LooseID( int Jet );
  std::pair <int,int> ReturnMatchedJet(int RecoJetIdx);
  bool MatchEmuJet( int Jet );
  std::pair <int,int> ReturnMatchedEmuJet(int RecoJetIdx);
  double ReturnMatchedEmuQuantity( std::pair<int,int> matchJet,int Quantity);
  void BookHistos();
  double ReturnMatchedQuantity( std::pair<int,int> matchJet,int Quantity);
  virtual double deltaPhi(double phi1, double phi2);
  virtual double deltaR(double eta1, double phi1, double eta2, double phi2);
  double MaxL1Et();
  int leadingOfflineJet();
//your private methods can be declared here
// histos
  TH1F *l1JetEn;
  TH1F *RefJets;
  TH1F *Bit15;
  TH1F *Bit16;
  TH1F *Bit17;
  TH1F *Bit18;
  TH1F *Bit19;
  TH1F *Bit20;
  TH1F *Bit21;
  TH1F *l1Jet0;
  TH1F *l1Jet1;
  TH1F *l1Jet2;
  TH1F *l1Jet3;
  TH1F *l1Jet4;
  TH1F *l1Jet5;
  TH1F *dR;
  TH2F *EnCorrelation;
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
  TH2F *recoJetCorrelation;
  TH1F *EMF;
  TH2I *timeMap;
  TH2F *ResolutionAsFnOfpT;
  TH2F *ResolutionAsFnOfeta;
  TH2D *L1EtaPhiMap;
  TH2F *L1HtRecoJetCorrelation;
  TH2F *MEtCorrelation;
  TH1F *RecoHTL1100;
  TH1F *RecoHTL1150;
  TH1F *RecoHTL150;
  TH1F *RecoHT;
  TH2F *L1CorVsUnCor;
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
  double minDeltaR = 999999.;
  for(unsigned int i = 0; i < l1extra_->nCenJets; i++){
    if( deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1extra_->cenJetEta[i], l1extra_->cenJetPhi[i]) < minDeltaR)
    {
        minDeltaR = deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1extra_->cenJetEta[i], l1extra_->cenJetPhi[i]);
    }
  }

  for(unsigned int i = 0; i < l1extra_->nTauJets; i++){
    if( deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1extra_->tauJetEta[i], l1extra_->tauJetPhi[i]) < minDeltaR)
    {
      minDeltaR = deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1extra_->tauJetEta[i], l1extra_->tauJetPhi[i]);
    }
  }

  for(unsigned int i = 0; i < l1extra_->nFwdJets; i++){
    if( deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1extra_->fwdJetEta[i], l1extra_->fwdJetPhi[i]) < minDeltaR)
    {
      minDeltaR = deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1extra_->fwdJetEta[i], l1extra_->fwdJetPhi[i]);
    }
  }
  if(minDeltaR < .5){ return true; }
  else return false;
}

bool L1JetAnalysis::MatchEmuJet(int RecoJetIdx){
  double minDeltaR = 999999.;
  for(unsigned int i = 0; i < l1emuextra_->nCenJets; i++){
    if( deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1emuextra_->cenJetEta[i], l1emuextra_->cenJetPhi[i]) < minDeltaR)
    {
        minDeltaR = deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1emuextra_->cenJetEta[i], l1emuextra_->cenJetPhi[i]);
    }
  }

  for(unsigned int i = 0; i < l1emuextra_->nTauJets; i++){
    if( deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1emuextra_->tauJetEta[i], l1emuextra_->tauJetPhi[i]) < minDeltaR)
    {
      minDeltaR = deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1emuextra_->tauJetEta[i], l1emuextra_->tauJetPhi[i]);
    }
  }

  for(unsigned int i = 0; i < l1emuextra_->nFwdJets; i++){
    if( deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1emuextra_->fwdJetEta[i], l1emuextra_->fwdJetPhi[i]) < minDeltaR)
    {
      minDeltaR = deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1emuextra_->fwdJetEta[i], l1emuextra_->fwdJetPhi[i]);
    }
  }
  if(minDeltaR < .5){ return true; }
  else return false;
}



std::pair<int,int> L1JetAnalysis::ReturnMatchedJet(int RecoJetIdx){
  double minDeltaR = 9999.;
  std::pair <int,int> matchedJet(-1,-1);
  //printf("Reco Jet %i is Et,EtCorr,Eta,Phi (%f ,%f, %f, %f) \n ", RecoJetIdx,recoJet_->et[RecoJetIdx],recoJet_->etCorr[RecoJetIdx] , recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx]);
  for(unsigned int i = 0; i < l1extra_->nCenJets; i++){

    //printf("CenJet Jet is Pt,Eta,Phi ( %f, %f, %f) \n ", l1extra_->cenJetEt[i] , l1extra_->cenJetEta[i], l1extra_->cenJetPhi[i]);

    if( deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1extra_->cenJetEta[i], l1extra_->cenJetPhi[i]) < minDeltaR)
    {
      minDeltaR = deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1extra_->cenJetEta[i], l1extra_->cenJetPhi[i]);
      //printf( "Delta R = %f \n", minDeltaR);
      if(minDeltaR < .5){
      matchedJet.first = 0;
      matchedJet.second = i;
      }
    }
  }
  //printf("Reco Jet is Pt,Eta,Phi ( %f, %f, %f) \n ", recoJet_->etCorr[RecoJetIdx] , recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx]);
  for(unsigned int i = 0; i < l1extra_->nTauJets; i++){
    //printf("Tau Jet is Pt,Eta,Phi ( %f, %f, %f) \n ", l1extra_->tauJetEt[i] , l1extra_->tauJetEta[i], l1extra_->tauJetPhi[i]);
 if( deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1extra_->tauJetEta[i], l1extra_->tauJetPhi[i]) < minDeltaR)
    {
      minDeltaR = deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1extra_->tauJetEta[i], l1extra_->tauJetPhi[i]);
      //printf( "Delta R = %f \n", minDeltaR);
      if(minDeltaR < .5){
      matchedJet.first = 1;
      matchedJet.second = i;
      }
    }
  }
  for(unsigned int i = 0; i < l1extra_->nFwdJets; i++){
      //printf("FWD Jet is Pt,Eta,Phi ( %f, %f, %f) \n ", l1extra_->fwdJetEt[i] , l1extra_->fwdJetEta[i], l1extra_->fwdJetPhi[i]);

    if( deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1extra_->fwdJetEta[i], l1extra_->fwdJetPhi[i]) <minDeltaR)
    {
      minDeltaR = deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1extra_->fwdJetEta[i], l1extra_->fwdJetPhi[i]);
      //printf( "Delta R = %f \n", minDeltaR);
      if(minDeltaR < .5){
      matchedJet.first = 2;
      matchedJet.second = i;
      }
    }
  }
  return matchedJet;
}


std::pair<int,int> L1JetAnalysis::ReturnMatchedEmuJet(int RecoJetIdx){
  double minDeltaR = 9999.;
  std::pair <int,int> matchedJet(-1,-1);
  //printf("Reco Jet %i is Et,EtCorr,Eta,Phi (%f ,%f, %f, %f) \n ", RecoJetIdx,recoJet_->et[RecoJetIdx],recoJet_->etCorr[RecoJetIdx] , recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx]);
  for(unsigned int i = 0; i < l1emuextra_->nCenJets; i++){

    //printf("CenJet Jet is Pt,Eta,Phi ( %f, %f, %f) \n ", l1emuextra_->cenJetEt[i] , l1emuextra_->cenJetEta[i], l1emuextra_->cenJetPhi[i]);

    if( deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1emuextra_->cenJetEta[i], l1emuextra_->cenJetPhi[i]) < minDeltaR)
    {
      minDeltaR = deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1emuextra_->cenJetEta[i], l1emuextra_->cenJetPhi[i]);
      //printf( "Delta R = %f \n", minDeltaR);
      if(minDeltaR < .5){
      matchedJet.first = 0;
      matchedJet.second = i;
      }
    }
  }
  //printf("Reco Jet is Pt,Eta,Phi ( %f, %f, %f) \n ", recoJet_->etCorr[RecoJetIdx] , recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx]);
  for(unsigned int i = 0; i < l1emuextra_->nTauJets; i++){
    //printf("Tau Jet is Pt,Eta,Phi ( %f, %f, %f) \n ", l1emuextra_->tauJetEt[i] , l1emuextra_->tauJetEta[i], l1emuextra_->tauJetPhi[i]);
 if( deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1emuextra_->tauJetEta[i], l1emuextra_->tauJetPhi[i]) < minDeltaR)
    {
      minDeltaR = deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1emuextra_->tauJetEta[i], l1emuextra_->tauJetPhi[i]);
      //printf( "Delta R = %f \n", minDeltaR);
      if(minDeltaR < .5){
      matchedJet.first = 1;
      matchedJet.second = i;
      }
    }
  }
  for(unsigned int i = 0; i < l1emuextra_->nFwdJets; i++){
      //printf("FWD Jet is Pt,Eta,Phi ( %f, %f, %f) \n ", l1emuextra_->fwdJetEt[i] , l1emuextra_->fwdJetEta[i], l1emuextra_->fwdJetPhi[i]);

    if( deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1emuextra_->fwdJetEta[i], l1emuextra_->fwdJetPhi[i]) <minDeltaR)
    {
      minDeltaR = deltaR(recoJet_->eta[RecoJetIdx], recoJet_->phi[RecoJetIdx], l1emuextra_->fwdJetEta[i], l1emuextra_->fwdJetPhi[i]);
      //printf( "Delta R = %f \n", minDeltaR);
      if(minDeltaR < .5){
      matchedJet.first = 2;
      matchedJet.second = i;
      }
    }
  }
  return matchedJet;
}



bool L1JetAnalysis::PassTrig(int bit, int bx) {
  if(bit<64) { if((gt_->tw1[bx]>>bit)&1) { return true; } else return false; }
  if(bit<128) { if((gt_->tw2[bx]>>(bit-64))&1) { return true;} else return false;}
  else { if((gt_->tt[bx]>>(bit-1000))&1) { return true;} else return false;}
}


double L1JetAnalysis::ReturnMatchedQuantity( std::pair<int,int> matchJet,int Quantity){
  if(Quantity == 1){
    if(matchJet.first== 0){
      return l1extra_->cenJetEt[matchJet.second];
    }
    if(matchJet.first== 1){
      //cout << " matched jet ET is " << l1extra_->tauJetEt[matchJet.second] << endl;
      return l1extra_->tauJetEt[matchJet.second];
    }
    if(matchJet.first == 2 ){
      return l1extra_->fwdJetEt[matchJet.second];
    }
    if(matchJet.first==-1){
      return false;}
    }

    if(Quantity==2){
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

    if(Quantity==3){
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
    return false;
  }

double L1JetAnalysis::ReturnMatchedEmuQuantity( std::pair<int,int> matchJet,int Quantity){
  if(Quantity == 1){
    if(matchJet.first== 0){
      return l1emuextra_->cenJetEt[matchJet.second];
    }
    if(matchJet.first== 1){
      //cout << " matched jet ET is " << l1emuextra_->tauJetEt[matchJet.second] << endl;
      return l1emuextra_->tauJetEt[matchJet.second];
    }
    if(matchJet.first == 2 ){
      return l1emuextra_->fwdJetEt[matchJet.second];
    }
    if(matchJet.first==-1){
      return false;}
    }

    if(Quantity==2){
        if(matchJet.first==0){
          return l1emuextra_->cenJetEta[matchJet.second];
        }
        if(matchJet.first==1){
          return l1emuextra_->tauJetEta[matchJet.second];
        }
        if(matchJet.first==2){
          return l1emuextra_->fwdJetEta[matchJet.second];
        }
        if(matchJet.first==-1){
          return false;
        }
    }

    if(Quantity==3){
        if(matchJet.first==0){
          return l1emuextra_->cenJetPhi[matchJet.second];
        }
        if(matchJet.first==1){
          return l1emuextra_->tauJetPhi[matchJet.second];
        }
        if(matchJet.first==2){
          return l1emuextra_->fwdJetPhi[matchJet.second];
        }
        if(matchJet.first==-1){
          return false;
        }
    }
    return false;
  }

  bool L1JetAnalysis::PassHLT( TString TrigBit){
  std::vector<TString>::iterator TrigList = event_->hlt.begin();
  std::vector<TString>::iterator TrigEnd = event_->hlt.end();
  TString ne = TrigBit.Chop();
  for( ; TrigList!=TrigEnd ; ++ TrigList){
    if( TrigList->Contains( ne ) ) {
     //std::cout << *TrigList << " Compared  " << ne << std::endl;
     return true; }
     }
  return false;
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
       double L1JetAnalysis::MaxL1Et(){
         double a = 0.;
         double b = 0.;
         double c = 0.;

       if((l1extra_->cenJetEt).size() > 0) a = l1extra_->cenJetEt[0];
       if((l1extra_->tauJetEt).size() > 0) b = l1extra_->tauJetEt[0];
       if((l1extra_->fwdJetEt).size() > 0) c = l1extra_->fwdJetEt[0];
         if(max(a,b) >c){
           return ( a > b ? a : b );
         }
         else return c;
       }

      int L1JetAnalysis::leadingOfflineJet(){
        double maxEn = 0.;
        int index = -1;

        for(int i = 0; i < (recoJet_->etCorr).size(); ++i)
        {
          if(recoJet_->etCorr[i] > maxEn){
            // //printf("Highest RecoJet is %d , with an Et of %f \n",i,recoJet_->etCorr[i]);
            maxEn = recoJet_->etCorr[i];
            index = i;
          }
        }
        return index;
      }

