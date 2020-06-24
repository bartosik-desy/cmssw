#ifndef RecoLocalCalo_EcalRecProducers_EcalUncalibRecHitRecWorkerGlobal_hh
#define RecoLocalCalo_EcalRecProducers_EcalUncalibRecHitRecWorkerGlobal_hh

/** \class EcalUncalibRecHitRecGlobalAlgo                                                                                                                                           
 *  Template used to compute amplitude, pedestal using a weights method                                                                                                            
 *                           time using a ratio method                                                                                                                             
 *                           chi2 using express method  
 *
 *  \author R. Bruneliere - A. Zabi
 */

#include "RecoLocalCalo/EcalRecProducers/interface/EcalUncalibRecHitWorkerBaseClass.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalUncalibRecHitMultiFitAlgo.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalUncalibRecHitTimeWeightsAlgo.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalUncalibRecHitRecChi2Algo.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalUncalibRecHitRatioMethodAlgo.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CondFormats/EcalObjects/interface/EcalTimeCalibConstants.h"
#include "CondFormats/EcalObjects/interface/EcalTimeOffsetConstant.h"
#include "CondFormats/EcalObjects/interface/EcalLiteDTUPedestals.h"
#include "CondFormats/EcalObjects/interface
/EcalCATIAGainRatios.h"
#include "CondFormats/EcalObjects/interface/EcalWeightXtalGroups.h"
#include "CondFormats/EcalObjects/interface/EcalTBWeights.h"
#include "CondFormats/EcalObjects/interface/EcalSampleMask.h"
#include "CondFormats/EcalObjects/interface/EcalTimeBiasCorrections.h"
#include "CondFormats/EcalObjects/interface/EcalPhase2SamplesCorrelation.h"
#include "CondFormats/EcalObjects/interface/EcalPhase2PulseShapes.h"
#include "CondFormats/EcalObjects/interface/EcalPhase2PulseCovariances.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EigenMatrixTypes.h"

namespace edm {
  class Event;
  class EventSetup;
  class ParameterSet;
  class ParameterSetDescription;
}  // namespace edm

class EcalPhase2UncalibRecHitWorkerMultiFit {
public:
  EcalUncalibRecHitWorkerMultiFit(const edm::ParameterSet&, edm::ConsumesCollector& c);
  EcalUncalibRecHitWorkerMultiFit(){};
  ~EcalUncalibRecHitWorkerMultiFit() override{};

private:
  void set(const edm::EventSetup& es) override;
  void set(const edm::Event& evt) override;
  void run(const edm::Event& evt, const EcalDigiCollectionPh2& digis, EcalUncalibratedRecHitCollection& result) override;

public:
  edm::ParameterSetDescription getAlgoDescription() override;

private:
  edm::ESHandle<EcalLiteDTUPedestals> peds;
  edm::ESHandle<EcalCATIAGainRatios> gains;
  edm::ESHandle<EcalSamplesCorrelation> noisecovariances;
  edm::ESHandle<EcalPhase2PulseShapes> pulseshapes;
  edm::ESHandle<EcalPhase2PulseCovariances> pulsecovariances;

  double timeCorrection(float ampli, const std::vector<float>& amplitudeBins, const std::vector<float>& shiftBins);

  const SampleMatrix& noisecor(bool barrel, int gain) const { return noisecors_[barrel ? 1 : 0][gain]; }
  const SampleMatrixGainArray& noisecor(bool barrel) const { return noisecors_[barrel ? 1 : 0]; }

  // multifit method
  std::array<ecal2:SampleMatrixGainArray, 2> noisecors_;
  BXVector activeBX;
  bool ampErrorCalculation_;
  bool useLumiInfoRunHeader_;
  EcalUncalibRecHitMultiFitAlgo multiFitMethod_;

  int bunchSpacingManual_;
  edm::EDGetTokenT<unsigned int> bunchSpacing_;

  // determine which of the samples must actually be used by ECAL local reco
  edm::ESHandle<EcalSampleMask> sampleMaskHand_;

  // time algorithm to be used to set the jitter and its uncertainty
  enum TimeAlgo { noMethod, ratioMethod, weightsMethod };
  TimeAlgo timealgo_ = noMethod;

  // time weights method
  edm::ESHandle<EcalWeightXtalGroups> grps;
  edm::ESHandle<EcalTBWeights> wgts;
  const EcalWeightSet::EcalWeightMatrix* weights[2];
  EcalUncalibRecHitTimeWeightsAlgo<EBDataFrame> weightsMethod_barrel_;
  
  bool doPrefitEB_;
  double prefitMaxChiSqEB_;
  bool dynamicPedestalsEB_;
  bool mitigateBadSamplesEB_;
  bool gainSwitchUseMaxSampleEB_;
  bool selectiveBadSampleCriteriaEB_;
  double addPedestalUncertaintyEB_;
  bool simplifiedNoiseModelForGainSwitch_;

  // ratio method
  std::vector<double> EBtimeFitParameters_;
  std::vector<double> EBamplitudeFitParameters_;
  std::pair<double, double> EBtimeFitLimits_;
  
  EcalUncalibRecHitRatioMethodAlgo<EBDataFrame_Ph2> ratioMethod_barrel_;
  

  double EBtimeConstantTerm_;
  double EBtimeNconst_;
  double outOfTimeThreshG10pEB_;
  double outOfTimeThreshG1mEB_;
  double outOfTimeThreshG10pEB_;
  double outOfTimeThreshG1mEB_;
  double amplitudeThreshEB_;
  double ebSpikeThresh_;

  edm::ESHandle<EcalTimeBiasCorrections> timeCorrBias_;

  edm::ESHandle<EcalTimeCalibConstants> itime;
  edm::ESHandle<EcalTimeOffsetConstant> offtime;
  std::vector<double> ebPulseShape_;


  // chi2 thresholds for flags settings
  bool kPoorRecoFlagEB_;
  double chi2ThreshEB_;

};

#endif
