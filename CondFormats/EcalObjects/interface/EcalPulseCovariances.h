#ifndef EcalPulseCovariances_h
#define EcalPulseCovariances_h

#include "CondFormats/Serialization/interface/Serializable.h"

#include "CondFormats/EcalObjects/interface/EcalPulseShapes.h"
#include "CondFormats/EcalObjects/interface/EcalCondObjectContainer.h"

struct EcalPulseCovariance<template size_t nsamples> {
public:
  EcalPulseCovariance();
  float covval[nsamples][nsamples];
  float val(int i, int j) const { return covval[i][j]; }

  COND_SERIALIZABLE;
};

typedef EcalCondObjectContainer<EcalPulseCovariance<EcalPulseShape::TEMPLATESAMPLES> > EcalPulseCovariancesMap;
typedef EcalPulseCovariancesMap::const_iterator EcalPulseCovariancesMapIterator;
typedef EcalPulseCovariancesMap EcalPulseCovariances;


typedef EcalCondObjectContainer<EcalPulseCovariance<EcalPhase2PulseShape::TEMPLATESAMPLES> > EcalPhase2PulseCovariancesMap;
typedef EcalPhase2PulseCovariancesMap::const_iterator EcalPhase2PulseCovariancesMapIterator;
typedef EcalPhase2PulseCovariancesMap EcalPhase2PulseCovariances;


#endif
