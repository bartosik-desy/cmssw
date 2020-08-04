#ifndef DataFormats_EcalDigi_EcalDataFrame_Ph2_h
#define DataFormats_EcalDigi_EcalDataFrame_Ph2_h
#include "DataFormats/EcalDigi/interface/EcalLiteDTUSample.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/Common/interface/DataFrame.h"
#include "CondFormats/EcalObjects/interface/EcalConstants.h"

/** \class EcalDataFrame_Ph2
      
*/
class EcalDataFrame_Ph2 {
public:
  EcalDataFrame_Ph2() {}
  // EcalDataFrame_Ph2(DetId i) :  m_data(i) {}
  EcalDataFrame_Ph2(edm::DataFrame const& iframe) : m_data(iframe) {}

  virtual ~EcalDataFrame_Ph2() {}

  DetId id() const { return m_data.id(); }

  int size() const { return m_data.size(); }

  EcalLiteDTUSample operator[](int i) const { return m_data[i]; }
  EcalLiteDTUSample sample(int i) const { return m_data[i]; }

  // get the leading sample (the first non saturated sample)
  // starting from the fourth sample
  // (it relies on the fact that the unpaker will discard fixed gain0 DataFrame)
  // .. sample numbering: [0, 9]
  // .. return -1 in case of no saturation
  int lastUnsaturatedSample() const;
  // just the boolean method
  bool isSaturated() const { return (lastUnsaturatedSample() != -1); }

  // FIXME (shall we throw??)
  void setSize(int) {}
  // void setPresamples(int ps);
  void setSample(int i, EcalLiteDTUSample sam) { m_data[i] = sam; }

  //bool hasSwitchToGain6() const;
  //bool hasSwitchToGain1() const;

  static const int MAXSAMPLES = ecalPh2::sampleSize;

  edm::DataFrame const& frame() const { return m_data; }
  edm::DataFrame& frame() { return m_data; }

private:
  edm::DataFrame m_data;
};

#endif
