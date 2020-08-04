#include "CondFormats/EcalObjects/interface/EcalPulseShapes.h"

EcalPulseShapeT::EcalPulseShapeT() {
  for (int s = 0; s < TEMPLATESAMPLES; ++s)
    pdfval[s] = 0.;
}
