#include "hal/CAENDeviceIdentifier.hh"
#include "CAENVMElib.h"

HAL::CAENDeviceIdentifier::CAENDeviceIdentifier( uint32_t adapterHandle,
                                            bool doSwapping )  
    throw (BusAdapterException)
    : adapterHandle_( adapterHandle ),
      doSwapping_( doSwapping ){
}

HAL::CAENDeviceIdentifier::~CAENDeviceIdentifier(){
}

const long HAL::CAENDeviceIdentifier::getAdapterHandle() const {
    return adapterHandle_;
}

const bool HAL::CAENDeviceIdentifier::doSwapping() const {
    return doSwapping_;
}

std::string HAL::CAENDeviceIdentifier::printString() const {
    return "there is nothing to print in HAL::CAENDeviceIdentifier::print()";
}
