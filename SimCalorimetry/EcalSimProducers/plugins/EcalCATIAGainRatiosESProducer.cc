#include <memory>
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/ESProductHost.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CondFormats/DataRecord/interface/EcalCATIAGainRatiosRcd.h"
#include "CondFormats/EcalObjects/interface/EcalCATIAGainRatios.h"
#include "CondFormats/EcalObjects/src/classes.h"
//
// class declaration
//
const int kEBChannels = ecalPh2::kEBChannels;

class EcalCATIAGainRatiosESProducer : public edm::ESProducer {
public:
  EcalCATIAGainRatiosESProducer(const edm::ParameterSet& iConfig);

  typedef std::unique_ptr<EcalCATIAGainRatios> ReturnType;

  ReturnType produce(const EcalCATIAGainRatiosRcd& iRecord);

private:
  edm::ParameterSet pset_;
};

EcalCATIAGainRatiosESProducer::EcalCATIAGainRatiosESProducer(const edm::ParameterSet& iConfig) : pset_(iConfig) {
  setWhatProduced(this);
}
////
EcalCATIAGainRatiosESProducer::ReturnType EcalCATIAGainRatiosESProducer::produce(const EcalCATIAGainRatiosRcd& iRecord) {
  auto prod = std::make_unique<EcalCATIAGainRatios>();
  for (int iChannel = 0; iChannel < kEBChannels; iChannel++) {
    EBDetId myEBDetId = EBDetId::unhashIndex(iChannel);
    float val = 10.;
    prod->setValue(myEBDetId.rawId(), val);
  }

  return prod;
}

//Define this as a plug-in
DEFINE_FWK_EVENTSETUP_MODULE(EcalCATIAGainRatiosESProducer);
