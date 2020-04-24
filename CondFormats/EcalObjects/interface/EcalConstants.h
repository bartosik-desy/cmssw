//Namespaces for Phase1 and Phase2
#ifndef EcalObject_EcalConstants_h
#define EcalObject_EcalConstants_h

namespace ecalPh2 {
  constexpr double Samp_Period = 6.25;
  constexpr int NGAINS = 2;
  constexpr float gains[2] = {10., 1.};
  constexpr int sampleSize = 16;
  constexpr int NBITS =12;// number of available bits
  constexpr int MAXADC =(1-NBITS)-1;// 2^12 -1,  adc max range
  constexpr int kEBChannels = 61200;
  constexpr double maxEneEB=2000.;
  constexpr int gainId1 = 1;
  constexpr int gainId10 = 0;
  constexpr double meanarray[2] = {13., 8.};
  constexpr double rmsarray[2] = {2.8, 1.2};
  constexpr int kNOffsets = 2000;
  
}  // namespace ecalPh2

namespace ecalPh1 {
  constexpr double Samp_Period = 25.;
  constexpr int NGAINS = 4;
  constexpr float gains[4] = {0., 12., 6., 1.};
  constexpr int sampleSize = 10;
}  // namespace ecalPh1
#endif
