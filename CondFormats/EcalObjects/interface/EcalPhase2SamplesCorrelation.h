#ifndef CondFormats_EcalObjects_EcalSamplesCorrelation_HH
#define CondFormats_EcalObjects_EcalSamplesCorrelation_HH

// -*- C++ -*-
//
// Author:      Jean Fay
// Created:     Monday 24 Nov 2014
//

#include "CondFormats/Serialization/interface/Serializable.h"

#include "DataFormats/Math/interface/Matrix.h"
#include <iostream>
#include <vector>

class EcalSamplesCorrelation {
public:
  EcalSamplesCorrelation();
  EcalSamplesCorrelation(const EcalSamplesCorrelation& aset);
  ~EcalSamplesCorrelation();

  std::vector<double> EBG10SamplesCorrelation;
  std::vector<double> EBG1SamplesCorrelation;

  void print(std::ostream& o) const;

  COND_SERIALIZABLE;
};

#endif
