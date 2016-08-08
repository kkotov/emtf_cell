#include "hal/VMEConfigurationSpaceDevice.hh"
#include <sstream>

HAL::VMEConfigurationSpaceDevice::VMEConfigurationSpaceDevice( HAL::VMEAddressTable & vmeAddressTable,
							       HAL::VMEBusAdapterInterface & vmeBusAdapter ) 
  : HAL::VMEDevice( vmeAddressTable, vmeBusAdapter, 0x00 ) {
}

// nothing to be done here.
HAL::VMEConfigurationSpaceDevice::~VMEConfigurationSpaceDevice() {
}


void 
HAL::VMEConfigurationSpaceDevice::hardwareRead( const HAL::GeneralHardwareAddress& vmeAddress, 
						uint32_t* result,
						uint32_t offset ) const
  throw ( HAL::BusAdapterException ) {
 
  uint32_t myresult = 0;
  uint32_t newByte;
  for ( uint32_t i = 0; i < vmeAddress.getDataWidth(); i++ ) {
    vmeBusAdapter.read( deviceIdentifierPtr,
                        vmeAddress.getAddress() + offset,
                        0x2fUL, 1UL, &newByte );
    newByte &= 0xff;
    offset += 4;
    myresult = ((myresult << 8) & 0xffffff00) + newByte;
  }
  *result = myresult;
}

void 
HAL::VMEConfigurationSpaceDevice::hardwareWrite( const HAL::GeneralHardwareAddress& vmeAddress, 
						 uint32_t data,
						 uint32_t offset ) const
  throw ( HAL::BusAdapterException ) {

  uint32_t newByte;
  for ( int i = (vmeAddress.getDataWidth() - 1); i >= 0; i-- ) {
    newByte = (data >> (i*8)) & 0xff;
    vmeBusAdapter.write( deviceIdentifierPtr,
                         vmeAddress.getAddress() + offset,
                         0x2fUL, 1UL, newByte );
    offset += 4;
  }
}

void 
HAL::VMEConfigurationSpaceDevice::hardwareReadBlock( const HAL::GeneralHardwareAddress& address,
						     uint32_t length,
						     char *buffer,
						     HalAddressIncrement addressBehaviour,
						     uint32_t offset ) const
  throw (HAL::UnsupportedException) {
  std::stringstream text;
  text << "Block transfers are not supported for VME64(x) configuration space\n"
       << "    (HAL::VMEConfigurationSpaceDevice::hardwareReadBlock)" << std::ends;
  throw( HAL::UnsupportedException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
}

void 
HAL::VMEConfigurationSpaceDevice::hardwareWriteBlock( const HAL::GeneralHardwareAddress& address,
						      uint32_t length,
						      char *buffer,
						      HalAddressIncrement addressBehaviour,
						      uint32_t offset) const
  throw (HAL::UnsupportedException) {
  std::stringstream text;
  text << "Block transfers are not supported for VME64(x) configuration space\n"
       << "    (HAL::VMEConfigurationSpaceDevice::hardwareReadBlock)" << std::ends;
  throw( HAL::UnsupportedException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
}
