#include "hal/GeneralHardwareAddress.hh"

HAL::GeneralHardwareAddress::GeneralHardwareAddress( uint64_t address,
						     uint64_t dataWidth ) 
  : address_(address),
    dataWidth_(dataWidth) {
}

HAL::GeneralHardwareAddress::~GeneralHardwareAddress() {
}

uint64_t HAL::GeneralHardwareAddress::getAddress() const {
  return address_;
}

uint64_t HAL::GeneralHardwareAddress::getComputedAddress( const std::vector<uint64_t>& baseaddresses ) const {
  return (address_ + baseaddresses[mapId_] );
}

uint32_t HAL::GeneralHardwareAddress::getComputedAddress( const std::vector<uint32_t>& baseaddresses ) const {
  return ((uint32_t)address_ + baseaddresses[mapId_] );
}

uint64_t HAL::GeneralHardwareAddress::getDataWidth() const {
  return dataWidth_;
}

enum HAL::AddressSpace HAL::GeneralHardwareAddress::getAddressSpace() const {
  return addressSpace_;
}

uint64_t HAL::GeneralHardwareAddress::getMapId() const {
  return mapId_;
}

bool HAL::GeneralHardwareAddress::isConfigSpace() const {
  return ( addressSpace_ == (HAL::AddressSpace)CONFIGURATION );  
}

bool HAL::GeneralHardwareAddress::isMemorySpace() const {
  return ( addressSpace_ == (HAL::AddressSpace)MEMORY ) ;
}
