#ifndef EcalLiteDTUPedestals_h
#define EcalLiteDTUPedestals_h

#include "CondFormats/Serialization/interface/Serializable.h"
#include "CondFormats/EcalObjects/interface/EcalCondObjectContainer.h"
#include "CondFormats/EcalObjects/interface/EcalConstants.h"

//const int NGAINS = 2;

//const int gainId1 = 1;
//const int gainId10 = 0;

class EcalLiteDTUPed {
public:
  double meanarray[2] = {ecalPh2::meanarray[0],ecalPh2::meanarray[1]};
  double rmsarray[2] = {ecalPh2::rmsarray[0],ecalPh2::rmsarray[1]};
  
   

  int setMean(int i, float value) {
    if (i >= ecalPh2::NGAINS || i < 0)
      return -1;
    else
      meanarray[i] = value;
    return 1;
  }

  int setRMS(int i, float value) {
    if (i >= ecalPh2::NGAINS || i < 0)
      return -1;
    else
      rmsarray[i] = value;
    return 1;
  }

  float mean(int i) const { return ecalPh2::meanarray[i]; }

  float rms(int i) const { return ecalPh2::rmsarray[i]; }

  COND_SERIALIZABLE;
};

typedef EcalCondObjectContainer<EcalLiteDTUPed> EcalLiteDTUPedestalsMap;
typedef EcalLiteDTUPedestalsMap::const_iterator EcalLiteDTUPedestalsMapIterator;
typedef EcalLiteDTUPedestalsMap EcalLiteDTUPedestals;

#endif
