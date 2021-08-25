#ifndef EcalPulseSymmCovariances_h
#define EcalPulseSymmCovariances_h

#include "CondFormats/Serialization/interface/Serializable.h"

#include "CondFormats/EcalObjects/interface/EcalPulseShapes.h"
#include "CondFormats/EcalObjects/interface/EcalCondObjectContainer.h"

struct EcalPulseSymmCovariance {
public:
  static const int TEMPLATESAMPLES = EcalPulseShape::TEMPLATESAMPLES;

  EcalPulseSymmCovariance();

  float covval[TEMPLATESAMPLES * (TEMPLATESAMPLES + 1) / 2];

  float val(int i, int j) const {
    int k = -1;
    if (j >= i)
      k = j + (TEMPLATESAMPLES - 1) * i;
    else
      k = i + (TEMPLATESAMPLES - 1) * j;
    return covval[k];
  }

  COND_SERIALIZABLE;
};

typedef EcalCondObjectContainer<EcalPulseSymmCovariance> EcalPulseSymmCovariancesMap;
typedef EcalPulseSymmCovariancesMap::const_iterator EcalPulseSymmCovariancesMapIterator;
typedef EcalPulseSymmCovariancesMap EcalPulseSymmCovariances;


struct EcalPh2PulseSymmCovariance: public EcalPulseSymmCovariance {
public:
  static const int TEMPLATESAMPLES = EcalPh2PulseShape::TEMPLATESAMPLES;
};

typedef EcalCondObjectContainer<EcalPulseSymmCovariance> EcalPh2PulseSymmCovariancesMap;
typedef EcalPh2PulseSymmCovariancesMap::const_iterator EcalPh2PulseSymmCovariancesMapIterator;
typedef EcalPh2PulseSymmCovariancesMap EcalPh2PulseSymmCovariances;

#endif
