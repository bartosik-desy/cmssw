#ifndef DataFormats_EcalDigi_EBDataFrame_Ph2_h
#define DataFormats_EcalDigi_EBDataFrame_Ph2_h

#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDigi/interface/EcalDataFrame_Ph2.h"
#include <iosfwd>

/** \class EBDataFrame_Ph2
      
*/
class EBDataFrame_Ph2 : public EcalDataFrame_Ph2 {
public:
  typedef EBDetId key_type;  ///< For the sorted collection
  typedef EcalDataFrame_Ph2 Base;

  EBDataFrame_Ph2() {}
  // EBDataFrame(DetId i) :  Base(i) {}
  EBDataFrame_Ph2(edm::DataFrame const& base) : Base(base) {}
  EBDataFrame_Ph2(EcalDataFrame_Ph2 const& base) : Base(base) {}

  /** estimator for a signal being a spike
   *  based on ratios between 4th, 5th and 6th sample
   */
  float spikeEstimator() const;

  ~EBDataFrame_Ph2() override {}

  key_type id() const { return Base::id(); }
};

std::ostream& operator<<(std::ostream&, const EBDataFrame_Ph2&);

#endif
