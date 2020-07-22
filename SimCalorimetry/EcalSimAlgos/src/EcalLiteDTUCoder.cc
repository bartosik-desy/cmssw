#include "SimCalorimetry/EcalSimAlgos/interface/EcalLiteDTUCoder.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "SimGeneral/NoiseGenerators/interface/CorrelatedNoisifier.h"
#include "DataFormats/EcalDigi/interface/EcalLiteDTUSample.h"
#include "DataFormats/EcalDigi/interface/EcalDataFrame_Ph2.h"

#include "CondFormats/EcalObjects/interface/EcalConstants.h"

#include <iostream>

/////////////////////
//NOTE:
//TO BE FINALISED
//Simona Cometti, [20.03.20 12:05]
//[In reply to Dario Soldi]
//In the Lite DTU v1 the gain 1 samples were 8.
//The saturated one was the sample 5:
//Two samples before and  5 after the one saturated.
//In the Lite DTU v1.2 there is the possibility of having  8 samples or 16 samples with gain 1
//When the gain 1 is selected we have:
//5 samples before the saturated sample and 10 samples after.
//When we have N consecutive saturated samples, the window at gain 1 is extended of N samples after (still to be implemented - to be double checked in the Cometti's PhD thesis)
const float DTUVersion = 1.0;

EcalLiteDTUCoder::EcalLiteDTUCoder(bool addNoise,
                                   bool PreMix1,
                                   EcalLiteDTUCoder::Noisifier* ebCorrNoise0,
                                   EcalLiteDTUCoder::Noisifier* ebCorrNoise1)
    : m_peds(nullptr),
      m_gainRatios(nullptr),
      m_intercals(nullptr),
      m_maxEneEB(ecalPh2::maxEneEB),  // Maximum for CATIA: LSB gain 10: 0.048 MeV
      m_addNoise(addNoise),
      m_PreMix1(PreMix1)

{
  m_ebCorrNoise[0] = ebCorrNoise0;
  m_ebCorrNoise[1] = ebCorrNoise1;
}

EcalLiteDTUCoder::~EcalLiteDTUCoder() {}

void EcalLiteDTUCoder::setFullScaleEnergy(double EBscale) { m_maxEneEB = ecalPh2::maxEneEB; }

void EcalLiteDTUCoder::setPedestals(const EcalLiteDTUPedestalsMap* pedestals) { m_peds = pedestals; }

void EcalLiteDTUCoder::setGainRatios(const EcalCATIAGainRatios* gainRatios) { m_gainRatios = gainRatios; }

void EcalLiteDTUCoder::setIntercalibConstants(const EcalIntercalibConstantsMC* ical) { m_intercals = ical; }

double EcalLiteDTUCoder::fullScaleEnergy(const DetId& detId) const { return m_maxEneEB; }

void EcalLiteDTUCoder::analogToDigital(CLHEP::HepRandomEngine* engine,
                                       const EcalSamples& clf,
                                       EcalDataFrame_Ph2& df) const {
  df.setSize(clf.size());
  encode(clf, df, engine);
}

void EcalLiteDTUCoder::encode(const EcalSamples& ecalSamples,
                              EcalDataFrame_Ph2& df,
                              CLHEP::HepRandomEngine* engine) const {
  const unsigned int nSamples(ecalSamples.size());

  DetId detId = ecalSamples.id();
  double Emax = fullScaleEnergy(detId);

  //N Gains set to 2 in the .h
  double pedestals[ecalPh2::NGAINS];
  double widths[ecalPh2::NGAINS];
  double LSB[ecalPh2::NGAINS];
  double trueRMS[ecalPh2::NGAINS];

  double icalconst = 1.;
  findIntercalibConstant(detId, icalconst);

  for (unsigned int igain(0); igain < ecalPh2::NGAINS; ++igain) {
    // fill in the pedestal and width
    findPedestal(detId, igain, pedestals[igain], widths[igain]);
    // insert an absolute value in the trueRMS
    trueRMS[igain] = std::sqrt(std::abs(widths[igain] * widths[igain] - 1. / 12.));

    LSB[igain] = Emax / (ecalPh2::MAXADC * ecalPh2::gains[igain]);
  }

  CaloSamples noiseframe[ecalPh2::NGAINS] = {
      CaloSamples(detId, nSamples),
      CaloSamples(detId, nSamples),
  };

  const Noisifier* noisy[ecalPh2::NGAINS] = {m_ebCorrNoise[0], m_ebCorrNoise[1]};

  if (m_addNoise) {
    for (unsigned int ig=0; ig < ecalPh2::NGAINS; ++ig) {
      noisy[ig]->noisify(noiseframe[ig], engine);
    }
  }

  bool isSaturated[ecalPh2::NGAINS] = {false, false};
  std::vector<std::vector<int>> adctrace(nSamples);
  unsigned int saturatedSample[ecalPh2::NGAINS] = {0, 0};

  for (unsigned int i(0); i != nSamples; ++i)
    adctrace[i].resize(ecalPh2::NGAINS);

  for (unsigned int igain = 0; igain < ecalPh2::NGAINS; ++igain) {
    for (unsigned int i(0); i != nSamples; ++i) {
      adctrace[i][igain] = -1;
    }
  }

  // fill ADC trace in gain 0 (x10) and gain 1 (x1)
  for (unsigned int igain = 0; igain < ecalPh2::NGAINS; ++igain) {
    for (unsigned int i(0); i != nSamples; ++i) {
      double asignal = 0;
      if (!m_PreMix1) {
        asignal = pedestals[igain] + ecalSamples[i] / (LSB[igain] * icalconst) + trueRMS[igain] * noiseframe[igain][i];
        //Analog signal value for each sample in ADC.
        //It is corrected by the intercalibration constants

      } else {
        //  no noise nor pedestal when premixing
        asignal = ecalSamples[i] / (LSB[igain] * icalconst);
      }
      int isignal = asignal;

      unsigned int adc = asignal - (double)isignal < 0.5 ? isignal : isignal + 1;

      if (adc > ecalPh2::MAXADC) {
        adc = ecalPh2::MAXADC;
        isSaturated[igain] = true;
        saturatedSample[igain] = i;
      }

      if (isSaturated[0] && igain == 0) {
        break;  // gain 0 (x10) channel is saturated, readout will use gain 1 (x10)
      } else {
        adctrace[i][igain] = adc;
      }  // for adc
    }
    if (!isSaturated[0]) {
      break;  //  gain 0 (x10) is not saturated, so don't bother with gain 1
    }
  }  // for igain

  int igain = 0;
  const int previousSaturatedSamples = 5;
  const int nextSaturatedSamples = 10;
  // Note: we assume that Pileup generates small signals, and we will not saturate when adding pedestals
  for (unsigned int j = 0; j < nSamples; ++j) {
    if (isSaturated[0] and j >= (saturatedSample[0] - previousSaturatedSamples) and
        j < (saturatedSample[0] + nextSaturatedSamples)) {
      igain = 1;
    } else {
      igain = 0;
    }
    df.setSample(j, EcalLiteDTUSample(adctrace[j][igain], igain));
  }
}

void EcalLiteDTUCoder::findPedestal(const DetId& detId, int gainId, double& ped, double& width) const {
  EcalLiteDTUPedestalsMap::const_iterator itped = m_peds->getMap().find(detId);
  ped = (*itped).mean(gainId);
  width = (*itped).rms(gainId);

  LogDebug("EcalLiteDTUCoder") << "Pedestals for " << detId.rawId() << " gain range " << gainId << " : \n"
                               << "Mean = " << ped << " rms = " << width;
}

void EcalLiteDTUCoder::findIntercalibConstant(const DetId& detId, double& icalconst) const {
  EcalIntercalibConstantMC thisconst = 1.;
  // find intercalib constant for this xtal
  const EcalIntercalibConstantMCMap& icalMap = m_intercals->getMap();
  EcalIntercalibConstantMCMap::const_iterator icalit = icalMap.find(detId);

  thisconst = (*icalit);
  if (icalconst == 0.)
    thisconst = 1.;

  icalconst = thisconst;
}
