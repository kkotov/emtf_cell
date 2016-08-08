#include "hal/PCIDevice.hh"
#include <sstream>
#include <iomanip>

HAL::PCIDevice::PCIDevice( HAL::PCIAddressTable & addressTable,
			   HAL::PCIBusAdapterInterface & pciBusAdapter,
			   uint32_t vendorID,
			   uint32_t deviceID,
			   uint32_t index,
			   bool swapFlag )
  : HAL::HardwareDevice( addressTable ),
    pciAddressTable( addressTable ),
    pciBusAdapter( pciBusAdapter ) {

  pciBusAdapter.findDeviceByVendor( vendorID, deviceID, index, pciAddressTable,
				    &deviceIdentifierPtr, baseRegisters, swapFlag );
}

HAL::PCIDevice::PCIDevice( HAL::PCIAddressTable & addressTable,
                      HAL::PCIBusAdapterInterface & pciBusAdapter,
                      uint32_t vendorID,
                      uint32_t deviceID,
                      uint32_t index,
                      std::vector<uint32_t>& baseAddresses,
                      uint32_t command )
  : HAL::HardwareDevice( addressTable ),
    pciAddressTable( addressTable ),
    pciBusAdapter( pciBusAdapter ) {

  baseRegisters.clear();
  baseRegisters.insert(baseRegisters.begin(),baseAddresses.begin(),baseAddresses.end());
  pciBusAdapter.findDeviceByVendor( vendorID, deviceID, index, pciAddressTable,
                            &deviceIdentifierPtr, baseRegisters );
  pciBusAdapter.configWrite(*deviceIdentifierPtr,
                            0x00000004,
                            command);
}

HAL::PCIDevice::PCIDevice( HAL::PCIAddressTable & addressTable,
                      HAL::PCIBusAdapterInterface & pciBusAdapter,
                      uint32_t vendorID,
                      uint32_t deviceID,
                      uint32_t index,
                      std::vector<uint64_t>& baseAddresses,
                      uint64_t command )
  : HAL::HardwareDevice( addressTable ),
    pciAddressTable( addressTable ),
    pciBusAdapter( pciBusAdapter ) {

  baseRegisters = baseAddresses;
  pciBusAdapter.findDeviceByVendor( vendorID, deviceID, index, pciAddressTable,
                            &deviceIdentifierPtr, baseRegisters );
  pciBusAdapter.configWrite64(*deviceIdentifierPtr,
                            0x00000004,
                            command);
}


HAL::PCIDevice::PCIDevice( HAL::PCIAddressTable & addressTable,
                      HAL::PCIBusAdapterInterface & pciBusAdapter,
                      const PCILocationIdentifier& location,
		      bool swapFlag )
  : HAL::HardwareDevice( addressTable ),
    pciAddressTable( addressTable ),
    pciBusAdapter( pciBusAdapter ) {

  pciBusAdapter.findDeviceByBus( location.busID, location.slotID, location.functionID, 
				 pciAddressTable, &deviceIdentifierPtr, baseRegisters, swapFlag );
}

HAL::PCIDevice::~PCIDevice() {
  pciBusAdapter.closeDevice( deviceIdentifierPtr );
}

uint64_t HAL::PCIDevice::getItemAddress( std::string item ) const 
  throw ( HAL::NoSuchItemException,
	  HAL::IllegalOperationException ) {
  const HAL::GeneralHardwareAddress& pciAddress = 
    pciAddressTable.getGeneralHardwareAddress( item );
  if ( ! pciAddress.isMemorySpace() ) {
    std::string text = "Operation only allowed for Memory Space Items\n     (HAL::PCIDevice::getItemAddress)";
    throw (HAL::IllegalOperationException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }
  uint64_t address = pciAddress.getAddress();
  uint64_t barId = pciAddress.getMapId();
  return (address + baseRegisters[barId]);
}

void HAL::PCIDevice::hardwareWrite( const HAL::GeneralHardwareAddress& pciAddress, 
				    uint32_t data,
				    uint32_t offset ) const
  throw (HAL::IllegalValueException,
         HAL::BusAdapterException) {
    if ( pciAddress.isConfigSpace() ) {
      pciBusAdapter.configWrite( *deviceIdentifierPtr, pciAddress.getAddress() + offset, data );
    } else {
      pciBusAdapter.write( *deviceIdentifierPtr, 
                           pciAddress.getComputedAddress( baseRegisters) + offset,
                           data);
    }
}

void HAL::PCIDevice::hardwareWrite64( const HAL::GeneralHardwareAddress& pciAddress, 
				    uint64_t data,
				    uint64_t offset ) const
  throw (HAL::IllegalValueException,
         HAL::BusAdapterException) {
    if ( pciAddress.isConfigSpace() ) {
      pciBusAdapter.configWrite( *deviceIdentifierPtr, pciAddress.getAddress() + offset, data );
    } else {
      pciBusAdapter.write64( *deviceIdentifierPtr, 
                           pciAddress.getComputedAddress( baseRegisters) + offset,
                           data);
    }
}

void HAL::PCIDevice::hardwareRead( const HAL::GeneralHardwareAddress& pciAddress, 
				   uint32_t* result,
				   uint32_t offset ) const
  throw (HAL::IllegalValueException,
         HAL::BusAdapterException) {

  if ( pciAddress.isConfigSpace() ) {
    pciBusAdapter.configRead( *deviceIdentifierPtr, pciAddress.getAddress() + offset, result );
  } else {
    pciBusAdapter.read( *deviceIdentifierPtr, 
                        pciAddress.getComputedAddress( baseRegisters) + offset,
                        result);
  }
}

void HAL::PCIDevice::hardwareRead64( const HAL::GeneralHardwareAddress& pciAddress, 
				   uint64_t* result,
				   uint64_t offset ) const
  throw (HAL::IllegalValueException,
         HAL::BusAdapterException) {

  if ( pciAddress.isConfigSpace() ) {
    pciBusAdapter.configRead64( *deviceIdentifierPtr, pciAddress.getAddress() + offset, result );
  } else {
    pciBusAdapter.read64( *deviceIdentifierPtr, 
                        pciAddress.getComputedAddress( baseRegisters) + offset,
                        result);
  }
}

void HAL::PCIDevice::hardwareWriteBlock( const HAL::GeneralHardwareAddress& pciAddress,
					 uint32_t length,
					 char *buffer,
					 HalAddressIncrement addressBehaviour,
					 uint32_t offset) const
  throw (HAL::IllegalValueException, 
         HAL::UnsupportedException, 
         HAL::IllegalOperationException,
         HAL::BusAdapterException) {

  if ( !pciAddress.isMemorySpace() ) {
    std::string text = "Only implemented for Memory Space\n     (HAL::PCIDevice::writeBlock)";
    throw( HAL::IllegalOperationException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }

  if ( (length) % sizeof(uint32_t) != 0 ) {
    std::string text = "the length is not a multiple of the data width!\n     (HAL::PCIDevice::writeBlock)";
    throw( HAL::IllegalValueException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }

  uint32_t barId = pciAddress.getMapId();
  if ( barId < 0 || barId > 6 ) {
    std::stringstream text;
    text << "barId out of range :" << barId
         << "\n    must be between 0 and 5"
         << "\n    (HAL::PCIDevice::writeBlock)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
  if ( baseRegisters[barId]  == 0 ) {
    std::stringstream text;
    text << "barId " << std::dec << barId 
         << " cannot be used since it is not mapped.\n"
         << "    This is because no item with this barId is in the HAL::AddressTable."
         << "    (HAL::PCIDevice::writeBlock)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }

  uint32_t startAddress = pciAddress.getAddress() + baseRegisters[barId] + offset;
  if ( (startAddress) % sizeof(uint32_t) != 0 ) {
    std::stringstream text;
    text  << "the startaddress must be aligned for the data width " 
          << std::dec << sizeof(uint32_t) << ".\n"
          << "     But the start address is " 
          << std::hex << std::setw(8) << std::setfill('0') << startAddress 
          << "\n     (HAL::PCIDevice::writeBlock)" << std::ends;
    throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  pciBusAdapter.writeBlock( *deviceIdentifierPtr, 
                            startAddress,
                            length,
                            buffer,
                            addressBehaviour);
}

void HAL::PCIDevice::hardwareWriteBlock64( const HAL::GeneralHardwareAddress& pciAddress,
					 uint64_t length,
					 char *buffer,
					 HalAddressIncrement addressBehaviour,
					 uint64_t offset) const
  throw (HAL::IllegalValueException, 
         HAL::UnsupportedException, 
         HAL::IllegalOperationException,
         HAL::BusAdapterException) {

  if ( !pciAddress.isMemorySpace() ) {
    std::string text = "Only implemented for Memory Space\n     (HAL::PCIDevice::writeBlock64)";
    throw( HAL::IllegalOperationException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }

  if ( (length) % sizeof(uint64_t) != 0 ) {
    std::string text = "the length is not a multiple of the data width!\n     (HAL::PCIDevice::writeBlock64)";
    throw( HAL::IllegalValueException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }

  uint64_t barId = pciAddress.getMapId();
  if ( barId < 0 || barId > 6 ) {
    std::stringstream text;
    text << "barId out of range :" << barId
         << "\n    must be between 0 and 5"
         << "\n    (HAL::PCIDevice::writeBlock64)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
  if ( baseRegisters[barId]  == 0 ) {
    std::stringstream text;
    text << "barId " << std::dec << barId 
         << " cannot be used since it is not mapped.\n"
         << "    This is because no item with this barId is in the HAL::AddressTable."
         << "    (HAL::PCIDevice::writeBlock64)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }

  uint64_t startAddress = pciAddress.getAddress() + baseRegisters[barId] + offset;
  if ( (startAddress) % sizeof(uint64_t) != 0 ) {
    std::stringstream text;
    text  << "the startaddress must be aligned for the data width " 
          << std::dec << sizeof(uint64_t) << ".\n"
          << "     But the start address is " 
          << std::hex << std::setw(8) << std::setfill('0') << startAddress 
          << "\n     (HAL::PCIDevice::writeBlock64)" << std::ends;
    throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  pciBusAdapter.writeBlock64( *deviceIdentifierPtr, 
                            startAddress,
                            length,
                            buffer,
                            addressBehaviour);
}



void HAL::PCIDevice::hardwareReadBlock( const HAL::GeneralHardwareAddress& pciAddress,
					uint32_t length,
					char *buffer,
					HalAddressIncrement addressBehaviour,
					uint32_t offset) const
  throw (HAL::IllegalValueException, 
         HAL::UnsupportedException, 
         HAL::IllegalOperationException,
         HAL::BusAdapterException) {

  if ( !pciAddress.isMemorySpace() ) {
    std::string text = "Only implemented for Memory Space\n     (HAL::PCIDevice::readBlock)";
    throw( HAL::IllegalOperationException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }

  if ( (length) % sizeof(uint32_t) != 0 ) {
    std::string text = "the length is not a multiple of the data width!\n     (HAL::PCIDevice::readBlock)";
    throw( HAL::IllegalValueException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }

  uint32_t barId = pciAddress.getMapId();
  if ( barId < 0 || barId > 6 ) {
    std::stringstream text;
    text << "barId out of range :" << barId
         << "\n    must be between 0 and 5"
         << "\n    (HAL::PCIDevice::readBlock)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
  if ( baseRegisters[barId]  == 0 ) {
    std::stringstream text;
    text << "barId " << std::dec << barId 
         << " cannot be used since it is not mapped.\n"
         << "    This is because no item with this barId is in the HAL::AddressTable."
         << "    (HAL::PCIDevice::readBlock)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }

  uint32_t startAddress = pciAddress.getAddress() + baseRegisters[barId] + offset;
  if ( (startAddress) % sizeof(uint32_t) != 0 ) {
    std::stringstream text;
    text << "the startaddress must be aligned for the data width " 
         << std::dec << sizeof(uint32_t) << ".\n"
         << "     But the start address is " 
         << std::hex << std::setw(8) << std::setfill('0') << startAddress 
         << "\n     (HAL::PCIDevice::readBlock)" << std::ends;
    throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  pciBusAdapter.readBlock( *deviceIdentifierPtr, 
                           startAddress,
                           length,
                           buffer,
                           addressBehaviour);
}

void HAL::PCIDevice::hardwareReadBlock64( const HAL::GeneralHardwareAddress& pciAddress,
					uint64_t length,
					char *buffer,
					HalAddressIncrement addressBehaviour,
					uint64_t offset) const
  throw (HAL::IllegalValueException, 
         HAL::UnsupportedException, 
         HAL::IllegalOperationException,
         HAL::BusAdapterException) {

  if ( !pciAddress.isMemorySpace() ) {
    std::string text = "Only implemented for Memory Space\n     (HAL::PCIDevice::readBlock64)";
    throw( HAL::IllegalOperationException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }

  if ( (length) % sizeof(uint64_t) != 0 ) {
    std::string text = "the length is not a multiple of the data width!\n     (HAL::PCIDevice::readBlock64)";
    throw( HAL::IllegalValueException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }

  uint64_t barId = pciAddress.getMapId();
  if ( barId < 0 || barId > 6 ) {
    std::stringstream text;
    text << "barId out of range :" << barId
         << "\n    must be between 0 and 5"
         << "\n    (HAL::PCIDevice::readBlock64)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
  if ( baseRegisters[barId]  == 0 ) {
    std::stringstream text;
    text << "barId " << std::dec << barId 
         << " cannot be used since it is not mapped.\n"
         << "    This is because no item with this barId is in the HAL::AddressTable."
         << "    (HAL::PCIDevice::readBlock64)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }

  uint64_t startAddress = pciAddress.getAddress() + baseRegisters[barId] + offset;
  if ( (startAddress) % sizeof(uint64_t) != 0 ) {
    std::stringstream text;
    text << "the startaddress must be aligned for the data width " 
         << std::dec << sizeof(uint64_t) << ".\n"
         << "     But the start address is " 
         << std::hex << std::setw(8) << std::setfill('0') << startAddress 
         << "\n     (HAL::PCIDevice::readBlock64)" << std::ends;
    throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  pciBusAdapter.readBlock64( *deviceIdentifierPtr, 
                           startAddress,
                           length,
                           buffer,
                           addressBehaviour);
}


void HAL::PCIDevice::memoryWrite( uint32_t address,
				  uint32_t barId,
				  uint32_t data ) const
  throw (HAL::IllegalValueException,
         HAL::BusAdapterException) {
  if ( barId < 0 || barId > 6 ) {
    std::stringstream text;
    text << "barId out of range :" << barId
         << "\n    must be between 0 and 5"
         << "\n    (HAL::PCIDevice::memoryWrite)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
  if ( baseRegisters[barId]  == 0 ) {
    std::stringstream text;
    text << "barId " << std::dec << barId 
         << " cannot be used since it is not mapped.\n"
         << "    This is because no item with this barId is in the HAL::AddressTable."
         << "    (HAL::PCIDevice::memoryWrite)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  pciBusAdapter.write( *deviceIdentifierPtr, address + baseRegisters[barId], data );
}

void HAL::PCIDevice::memoryWrite64( uint64_t address,
				  uint64_t barId,
				  uint64_t data ) const
  throw (HAL::IllegalValueException,
         HAL::BusAdapterException) {
  if ( barId < 0 || barId > 6 ) {
    std::stringstream text;
    text << "barId out of range :" << barId
         << "\n    must be between 0 and 5"
         << "\n    (HAL::PCIDevice::memoryWrite64)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
  if ( baseRegisters[barId]  == 0 ) {
    std::stringstream text;
    text << "barId " << std::dec << barId 
         << " cannot be used since it is not mapped.\n"
         << "    This is because no item with this barId is in the HAL::AddressTable."
         << "    (HAL::PCIDevice::memoryWrite64)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  pciBusAdapter.write64( *deviceIdentifierPtr, address + baseRegisters[barId], data );
}


void HAL::PCIDevice::memoryRead( uint32_t address,
				 uint32_t barId,
				 uint32_t* result ) const 
  throw (HAL::IllegalValueException,
         HAL::BusAdapterException) {
  if ( barId < 0 || barId > 6 ) {
    std::stringstream text;
    text << "barId out of range :" << barId
         << "\n    must be between 0 and 5"
         << "\n    (HAL::PCIDevice::memoryRead)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
  if ( baseRegisters[barId]  == 0 ) {
    std::stringstream text;
    text << "barId " << std::dec << barId 
         << " cannot be used since it is not mapped.\n"
         << "    This is because no item with this barId is in the HAL::AddressTable.\n"
         << "    (HAL::PCIDevice::memoryRead)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  pciBusAdapter.read( *deviceIdentifierPtr, address + baseRegisters[barId], result );
}


void HAL::PCIDevice::memoryRead64( uint64_t address,
				 uint64_t barId,
				 uint64_t* result ) const 
  throw (HAL::IllegalValueException,
         HAL::BusAdapterException) {
  if ( barId < 0 || barId > 6 ) {
    std::stringstream text;
    text << "barId out of range :" << barId
         << "\n    must be between 0 and 5"
         << "\n    (HAL::PCIDevice::memoryRead64)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
  if ( baseRegisters[barId]  == 0 ) {
    std::stringstream text;
    text << "barId " << std::dec << barId 
         << " cannot be used since it is not mapped.\n"
         << "    This is because no item with this barId is in the HAL::AddressTable.\n"
         << "    (HAL::PCIDevice::memoryRead64)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  pciBusAdapter.read64( *deviceIdentifierPtr, address + baseRegisters[barId], result );
}


uint64_t HAL::PCIDevice::getPCIBusAddress( uint64_t iBar )
  throw (HAL::BusAdapterException) {
  return deviceIdentifierPtr->getPCIBusAddress( iBar );
}


// Hannes Sakulin: To accelerate the JAL (not to be used by users !!!)
void HAL::PCIDevice::configWrite( uint32_t address,
                             uint32_t data ) const
  throw (BusAdapterException) {
  pciBusAdapter.configWrite( *deviceIdentifierPtr, address, data );
}

void HAL::PCIDevice::configWrite64( uint64_t address,
                             uint64_t data ) const
  throw (BusAdapterException) {
  pciBusAdapter.configWrite( *deviceIdentifierPtr, address, data );
}


// Hannes Sakulin: To accelerate the JAL (not to be used by users !!!)
void HAL::PCIDevice::configRead( uint32_t address,
                            uint32_t* result ) const 
  throw (BusAdapterException) {
  pciBusAdapter.configRead( *deviceIdentifierPtr, address, result );
}

void HAL::PCIDevice::configRead64( uint64_t address,
                            uint64_t* result ) const 
  throw (BusAdapterException) {
  pciBusAdapter.configRead64( *deviceIdentifierPtr, address, result );
}

