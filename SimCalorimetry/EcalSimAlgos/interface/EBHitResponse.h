#ifndef EcalSimAlgos_EBHitResponse_h
#define EcalSimAlgos_EBHitResponse_h

#include "CalibFormats/CaloObjects/interface/CaloTSamples.h"
#include "SimCalorimetry/EcalSimAlgos/interface/EcalHitResponse.h"
#include "CondFormats/EcalObjects/interface/EcalIntercalibConstantsMC.h"
#include "CondFormats/EcalObjects/interface/EcalConstants.h"

class APDSimParameters;

namespace CLHEP {
  class HepRandomEngine;
}

template class EBHitResponseImpl<size_t nsamples, size_t noffsets, double sampleperiod > : public EcalHitResponse {
public:
  typedef CaloTSamples<float, nsamples> EBSamples;

  typedef std::vector<double> VecD;

  enum { kNOffsets = noffsets };

  constexpr double kSamplePeriod = sampleperiod;
  
  EBHitResponseImpl(const CaloVSimParameterMap* parameterMap,
                const CaloVShape* shape,
                bool apdOnly,
                const APDSimParameters* apdPars,
                const CaloVShape* apdShape);

  ~EBHitResponseImpl() override;

  void initialize(CLHEP::HepRandomEngine*);

  virtual bool keepBlank() const { return false; }

  void setIntercal(const EcalIntercalibConstantsMC* ical);

  void add(const PCaloHit& hit, CLHEP::HepRandomEngine*) override;

  void initializeHits() override;

  void finalizeHits() override;

  void run(MixCollection<PCaloHit>& hits, CLHEP::HepRandomEngine*) override;

  unsigned int samplesSize() const override;

  EcalSamples* operator[](unsigned int i) override;

  const EcalSamples* operator[](unsigned int i) const override;

protected:
  unsigned int samplesSizeAll() const override;

  EcalSamples* vSamAll(unsigned int i) override;

  const EcalSamples* vSamAll(unsigned int i) const override;

  EcalSamples* vSam(unsigned int i) override;

  void putAPDSignal(const DetId& detId, double npe, double time);

private:
  const VecD& offsets() const { return m_timeOffVec; }

  const double nonlFunc(double enr) const {
    return (pelo > enr ? pext : (pehi > enr ? nonlFunc1(enr) : pfac * atan(log10(enr - pehi + 0.00001)) + poff));
  }

  const double nonlFunc1(double energy) const {
    const double enr(log10(energy));
    const double enr2(enr * enr);
    const double enr3(enr2 * enr);
    return (pcub * enr3 + pqua * enr2 + plin * enr + pcon);
  }

  const APDSimParameters* apdParameters() const;
  const CaloVShape* apdShape() const;

  double apdSignalAmplitude(const PCaloHit& hit, CLHEP::HepRandomEngine*) const;

  void findIntercalibConstant(const DetId& detId, double& icalconst) const;

  const bool m_apdOnly;
  const APDSimParameters* m_apdPars;
  const CaloVShape* m_apdShape;
  const EcalIntercalibConstantsMC* m_intercal;

  std::vector<double> m_timeOffVec;

  std::vector<double> m_apdNpeVec;
  std::vector<double> m_apdTimeVec;

  const double pcub, pqua, plin, pcon, pelo, pehi, pasy, pext, poff, pfac;

  std::vector<EBSamples> m_vSam;

  bool m_isInitialized;
};


typedef EBHitResponseImpl<ecalPh1::sampleSize,2000,EcalHitResponse::BUNCHSPACE> EBHitResponse;
typedef EBHitResponseImpl<ecalPh2::sampleSize,ecalPh2::kNOffsets,ecalPh2::Samp_Period> EBHitResponse_Ph2;

#endif
