#include "hal/PCILinuxDeviceIdentifier.hh"


HAL::PCILinuxDeviceIdentifier::PCILinuxDeviceIdentifier( xpci::Address deviceAddress,
						  bool swapFlag )
  : deviceConfigAddress(deviceAddress),
    swapFlag(swapFlag) { 
}

HAL::PCILinuxDeviceIdentifier::~PCILinuxDeviceIdentifier( ) {
}

bool HAL::PCILinuxDeviceIdentifier::doSwap() const {
  return swapFlag;
}

uint64_t  HAL::PCILinuxDeviceIdentifier::getPCIBusAddress( uint64_t iBar ) const
  throw (BusAdapterException) {
  return 0;

}

xpci::Address HAL::PCILinuxDeviceIdentifier::getConfigAddress() {
  return deviceConfigAddress;
}

std::string HAL::PCILinuxDeviceIdentifier::printString() const {
  return "nothing to print in HAL::PCILinuxDeviceIdentifier::printString()";
}
