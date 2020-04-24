#include "DataFormats/EcalDigi/interface/EcalLiteDTUSample.h"
#include <iostream>

EcalLiteDTUSample::EcalLiteDTUSample(int adc, int gainId) { theSample = (adc & 0xFFF) | ((gainId & 0x3) << 12); }

std::ostream& operator<<(std::ostream& s, const EcalLiteDTUSample& samp) {
  s << "ADC=" << samp.adc() << ", gainId=" << samp.gainId();
  return s;
}
