#include "DataFormats/EcalDigi/interface/EcalDataFrame_Ph2.h"
#include "CondFormats/EcalObjects/interface/EcalConstants.h"
#include <iostream>

int EcalDataFrame_Ph2::lastUnsaturatedSample() const {
  int cnt = 0;
  for (size_t i = 3; i < m_data.size(); ++i) {
    cnt = 0;
    for (size_t j = i; j < (i + 5) && j < m_data.size(); ++j) {
      //if (((EcalLiteDTUSample)m_data[j]).gainId() == EcalMgpaBitwiseGain0)
      if (((EcalLiteDTUSample)m_data[j]).gainId() == ecalPh2::gainId1) {
        ++cnt;
      }
    }
    if (cnt == 5)
      return i - 1;  // the last unsaturated sample
  }
  return -1;  // no saturation found
}
