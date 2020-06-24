#ifndef EcalPulseShapes_h
#define EcalPulseShapes_h

#include "CondFormats/Serialization/interface/Serializable.h"

#include "CondFormats/EcalObjects/interface/EcalCondObjectContainer.h"

struct EcalPulseShape <template size_t nsamples>{
public:
  static const int TEMPLATESAMPLES = nsamples;

  EcalPulseShape();

  float pdfval[TEMPLATESAMPLES];

  float val(int isample) const { return pdfval[isample]; }

  COND_SERIALIZABLE;
};

typedef EcalCondObjectContainer<EcalPulseShape<12>> EcalPulseShapesMap;
typedef EcalPulseShapesMap::const_iterator EcalPulseShapesMapIterator;
typedef EcalPulseShapesMap EcalPulseShapes;

typedef EcalCondObjectContainer<EcalPulseShape<16>> EcalPhase2PulseShapesMap;
typedef EcalPhase2PulseShapesMap::const_iterator EcalPhase2PulseShapesMapIterator;
typedef EcalPhase2PulseShapesMap EcalPhase2PulseShapes;


#endif
