#ifndef EcalPulseShapes_h
#define EcalPulseShapes_h

#include "CondFormats/Serialization/interface/Serializable.h"

#include "CondFormats/EcalObjects/interface/EcalCondObjectContainer.h"

template <size_t nsamples> struct EcalPulseShapeT {
public:
  static const size_t TEMPLATESAMPLES = nsamples;

  EcalPulseShapeT() {
    for (size_t s = 0; s < TEMPLATESAMPLES; ++s) pdfval[s] = 0.;
  };

  float pdfval[TEMPLATESAMPLES];

  float val(size_t isample) const { return pdfval[isample]; }

  COND_SERIALIZABLE;
};


typedef EcalPulseShapeT<12> EcalPulseShape;
typedef EcalPulseShapeT<16> EcalPhase2PulseShape;

typedef EcalCondObjectContainer<EcalPulseShape> EcalPulseShapesMap;
typedef EcalPulseShapesMap::const_iterator EcalPulseShapesMapIterator;
typedef EcalPulseShapesMap EcalPulseShapes;

typedef EcalCondObjectContainer<EcalPhase2PulseShape> EcalPhase2PulseShapesMap;
typedef EcalPhase2PulseShapesMap::const_iterator EcalPhase2PulseShapesMapIterator;
typedef EcalPhase2PulseShapesMap EcalPhase2PulseShapes;


#endif
