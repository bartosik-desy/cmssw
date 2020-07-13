#include "SimCalorimetry/EcalSimAlgos/interface/EcalSimParameterMap_Ph2.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include <iostream>

/** Relevant constants are:
  4.5 photoelectrons per MeV (J. Nash's slides)
  APD gain 50, but analog signal stays in GeV
  Account for excess noise factor
 */

EcalSimParameterMap_Ph2::EcalSimParameterMap_Ph2()
    : theBarrelParameters(2250., 1. / 2250., 1., 0, 16, 6, true, true) {}  //,

EcalSimParameterMap_Ph2::EcalSimParameterMap_Ph2(double simHitToPhotoelectronsBarrel,
                                                 double photoelectronsToAnalogBarrel,
                                                 double samplingFactor,
                                                 double timePhase,
                                                 int readoutFrameSize,
                                                 int binOfMaximum,
                                                 bool doPhotostatistics,
                                                 bool syncPhase)
    : theBarrelParameters(simHitToPhotoelectronsBarrel,
                          photoelectronsToAnalogBarrel,
                          samplingFactor,
                          timePhase,
                          readoutFrameSize,
                          binOfMaximum,
                          doPhotostatistics,
                          syncPhase) {}  

const CaloSimParameters& EcalSimParameterMap_Ph2::simParameters(const DetId& detId) const {
  return theBarrelParameters;
}
