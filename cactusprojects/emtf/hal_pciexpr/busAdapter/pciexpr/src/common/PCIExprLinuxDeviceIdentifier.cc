#include "hal/PCIExprLinuxDeviceIdentifier.hh"


HAL::PCIExprLinuxDeviceIdentifier::PCIExprLinuxDeviceIdentifier( xpci::Address deviceAddress,
						  bool swapFlag )
  : deviceConfigAddress(deviceAddress),
    swapFlag(swapFlag) { 
}

HAL::PCIExprLinuxDeviceIdentifier::~PCIExprLinuxDeviceIdentifier( ) {
}

bool HAL::PCIExprLinuxDeviceIdentifier::doSwap() const {
  return swapFlag;
}

uint32_t  HAL::PCIExprLinuxDeviceIdentifier::getPCIBusAddress( uint32_t iBar ) const
  throw (BusAdapterException) {
  return 0;

}

xpci::Address HAL::PCIExprLinuxDeviceIdentifier::getConfigAddress() {
  return deviceConfigAddress;
}

std::string HAL::PCIExprLinuxDeviceIdentifier::printString() const {
  return "nothing to print in HAL::PCIExprLinuxDeviceIdentifier::printString()";
}
