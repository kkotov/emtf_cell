#include "PCIExprDevice.hh"
#include <sstream>
#include <iomanip>

HAL::PCIExprDevice::PCIExprDevice( HAL::PCIAddressTable & addressTable,
			   HAL::PCIExprLinuxBusAdapterInterface & pciBusAdapter,
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

HAL::PCIExprDevice::PCIExprDevice( HAL::PCIAddressTable & addressTable,
                      HAL::PCIExprLinuxBusAdapterInterface & pciBusAdapter,
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
  pciBusAdapter.configWrite(*deviceIdentifierPtr,
                            0x00000004,
                            command);
}

HAL::PCIExprDevice::PCIExprDevice( HAL::PCIAddressTable & addressTable,
                      HAL::PCIExprLinuxBusAdapterInterface & pciBusAdapter,
                      const PCILocationIdentifier& location,
		      bool swapFlag )
  : HAL::HardwareDevice( addressTable ),
    pciAddressTable( addressTable ),
    pciBusAdapter( pciBusAdapter ) {

  pciBusAdapter.findDeviceByBus( location.busID, location.slotID, location.functionID, 
				 pciAddressTable, &deviceIdentifierPtr, baseRegisters, swapFlag );
}

/* MTF6/7 specific */
HAL::PCIExprDevice::PCIExprDevice( HAL::PCIAddressTable & addressTable,
			   HAL::PCIExprLinuxBusAdapterInterface & pciBusAdapter,
			   uint32_t index )
  : HAL::HardwareDevice( addressTable ),
    pciAddressTable( addressTable ),
    pciBusAdapter( pciBusAdapter ) {

  pciBusAdapter.findDeviceByIndex( index );
  /*
    The current MTF6/7 driver is automatically using BAR0 and BAR1 spaces. 
    BAR0 window is reserved for single registers.
    BAR1 is a large memory window that only works with block transfers. 
    The driver decides which window to use based on the transfer size. 
    Adding the bar numbering just for completeness, but it is not currently used

    Setting all the BAR register to null values as the driver know them anyway
  */
  baseRegisters.clear();
  baseRegisters.push_back(0x0); //BAR0
  baseRegisters.push_back(0x10); //BAR1... otherwise the memory reading will complain: the address 0x10 is never used though
  baseRegisters.push_back(0x0);
  baseRegisters.push_back(0x0);
  baseRegisters.push_back(0x0);
  baseRegisters.push_back(0x0);
}

HAL::PCIExprDevice::~PCIExprDevice() {
  pciBusAdapter.closeDevice( deviceIdentifierPtr );
}

uint64_t HAL::PCIExprDevice::getItemAddress( std::string item ) const 
  throw ( HAL::NoSuchItemException,
	  HAL::IllegalOperationException ) {
  const HAL::GeneralHardwareAddress& pciAddress = 
    pciAddressTable.getGeneralHardwareAddress( item );
  if ( ! pciAddress.isMemorySpace() ) {
    std::string text = "Operation only allowed for Memory Space Items\n     (HAL::PCIExprDevice::getItemAddress)";
    throw (HAL::IllegalOperationException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }
  uint64_t address = pciAddress.getAddress();
  uint64_t barId = pciAddress.getMapId();
  return (address + baseRegisters[barId]);
}

void HAL::PCIExprDevice::hardwareWrite( const HAL::GeneralHardwareAddress& pciAddress, 
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

void HAL::PCIExprDevice::hardwareWrite64( const HAL::GeneralHardwareAddress& pciAddress, 
				    uint64_t data,
				    uint64_t offset ) const
  throw (HAL::IllegalValueException,
         HAL::BusAdapterException) {
    if ( pciAddress.isConfigSpace() ) {
      pciBusAdapter.configWrite64( *deviceIdentifierPtr, pciAddress.getAddress() + offset, data );
    } else {
      pciBusAdapter.write64( *deviceIdentifierPtr, 
                           pciAddress.getComputedAddress( baseRegisters) + offset,
                           data);
    }
}

void HAL::PCIExprDevice::hardwareRead( const HAL::GeneralHardwareAddress& pciAddress, 
				   uint32_t* result,
				   uint32_t offset ) const
  throw (HAL::IllegalValueException,
         HAL::BusAdapterException) {

  if ( pciAddress.isConfigSpace() ) {
    pciBusAdapter.configRead( *deviceIdentifierPtr, pciAddress.getAddress() + offset, result );
  } else {
    pciAddress.getComputedAddress( baseRegisters ); 
    
    pciBusAdapter.read( *deviceIdentifierPtr, 
                        pciAddress.getComputedAddress( baseRegisters) + offset,
                        result);
  }
}

void HAL::PCIExprDevice::hardwareRead64( const HAL::GeneralHardwareAddress& pciAddress, 
				   uint64_t* result,
				   uint64_t offset ) const
  throw (HAL::IllegalValueException,
         HAL::BusAdapterException) {

  if ( pciAddress.isConfigSpace() ) {
    pciBusAdapter.configRead64( *deviceIdentifierPtr, pciAddress.getAddress() + offset, result );
  } else {
    pciAddress.getComputedAddress( baseRegisters ); 
    
    pciBusAdapter.read64( *deviceIdentifierPtr, 
                        pciAddress.getComputedAddress( baseRegisters) + offset,
                        result);
  }
}


void HAL::PCIExprDevice::hardwareWriteBlock( const HAL::GeneralHardwareAddress& pciAddress,
					 uint32_t length,
					 char *buffer,
					 HalAddressIncrement addressBehaviour,
					 uint32_t offset) const
  throw (HAL::IllegalValueException, 
         HAL::UnsupportedException, 
         HAL::IllegalOperationException,
         HAL::BusAdapterException) {

  if ( !pciAddress.isMemorySpace() ) {
    std::string text = "Only implemented for Memory Space\n     (HAL::PCIExprDevice::writeBlock)";
    throw( HAL::IllegalOperationException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }

  if ( (length) % sizeof(uint32_t) != 0 ) {
    std::string text = "the length is not a multiple of the data width!\n     (HAL::PCIExprDevice::writeBlock)";
    throw( HAL::IllegalValueException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }

  uint32_t barId = pciAddress.getMapId();
  if ( barId < 0 || barId > 6 ) {
    std::stringstream text;
    text << "barId out of range :" << barId
         << "\n    must be between 0 and 5"
         << "\n    (HAL::PCIExprDevice::writeBlock)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
  if ( baseRegisters[barId]  == 0 ) {
    std::stringstream text;
    text << "barId " << std::dec << barId 
         << " cannot be used since it is not mapped.\n"
         << "    This is because no item with this barId is in the HAL::AddressTable."
         << "    (HAL::PCIExprDevice::writeBlock)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }

  uint32_t startAddress = pciAddress.getAddress() + baseRegisters[barId] + offset;
  if ( (startAddress) % sizeof(uint32_t) != 0 ) {
    std::stringstream text;
    text  << "the startaddress must be aligned for the data width " 
          << std::dec << sizeof(uint32_t) << ".\n"
          << "     But the start address is " 
          << std::hex << std::setw(8) << std::setfill('0') << startAddress 
          << "\n     (HAL::PCIExprDevice::writeBlock)" << std::ends;
    throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  pciBusAdapter.writeBlock( *deviceIdentifierPtr, 
                            startAddress,
                            length,
                            buffer,
                            addressBehaviour);
}

void HAL::PCIExprDevice::hardwareWriteBlock64( const HAL::GeneralHardwareAddress& pciAddress,
					 uint64_t length,
					 char *buffer,
					 HalAddressIncrement addressBehaviour,
					 uint64_t offset) const
  throw (HAL::IllegalValueException, 
         HAL::UnsupportedException, 
         HAL::IllegalOperationException,
         HAL::BusAdapterException) {

  if ( !pciAddress.isMemorySpace() ) {
    std::string text = "Only implemented for Memory Space\n     (HAL::PCIExprDevice::writeBlock64)";
    throw( HAL::IllegalOperationException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }

  if ( (length) % sizeof(uint64_t) != 0 ) {
    std::string text = "the length is not a multiple of the data width!\n     (HAL::PCIExprDevice::writeBlock64)";
    throw( HAL::IllegalValueException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }

  uint64_t barId = pciAddress.getMapId();
  if ( barId < 0 || barId > 6 ) {
    std::stringstream text;
    text << "barId out of range :" << barId
         << "\n    must be between 0 and 5"
         << "\n    (HAL::PCIExprDevice::writeBlock64)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
  if ( baseRegisters[barId]  == 0 ) {
    std::stringstream text;
    text << "barId " << std::dec << barId 
         << " cannot be used since it is not mapped.\n"
         << "    This is because no item with this barId is in the HAL::AddressTable."
         << "    (HAL::PCIExprDevice::writeBlock64)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }

  uint64_t startAddress = pciAddress.getAddress() + baseRegisters[barId] + offset;
  if ( (startAddress) % sizeof(uint64_t) != 0 ) {
    std::stringstream text;
    text  << "the startaddress must be aligned for the data width " 
          << std::dec << sizeof(uint64_t) << ".\n"
          << "     But the start address is " 
          << std::hex << std::setw(16) << std::setfill('0') << startAddress 
          << "\n     (HAL::PCIExprDevice::writeBlock64)" << std::ends;
    throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  pciBusAdapter.writeBlock64( *deviceIdentifierPtr, 
                            startAddress,
                            length,
                            buffer,
                            addressBehaviour);
}


void HAL::PCIExprDevice::hardwareReadBlock( const HAL::GeneralHardwareAddress& pciAddress,
					uint32_t length,
					char *buffer,
					HalAddressIncrement addressBehaviour,
					uint32_t offset) const
  throw (HAL::IllegalValueException, 
         HAL::UnsupportedException, 
         HAL::IllegalOperationException,
         HAL::BusAdapterException) {

  if ( !pciAddress.isMemorySpace() ) {
    std::string text = "Only implemented for Memory Space\n     (HAL::PCIExprDevice::readBlock)";
    throw( HAL::IllegalOperationException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }

  if ( (length) % sizeof(uint32_t) != 0 ) {
    std::string text = "the length is not a multiple of the data width!\n     (HAL::PCIExprDevice::readBlock)";
    throw( HAL::IllegalValueException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }

  uint32_t barId = pciAddress.getMapId();
  if ( barId < 0 || barId > 6 ) {
    std::stringstream text;
    text << "barId out of range :" << barId
         << "\n    must be between 0 and 5"
         << "\n    (HAL::PCIExprDevice::readBlock)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
  if ( baseRegisters[barId]  == 0 ) {
    std::stringstream text;
    text << "barId " << std::dec << barId 
         << " cannot be used since it is not mapped.\n"
         << "    This is because no item with this barId is in the HAL::AddressTable."
         << "    (HAL::PCIExprDevice::readBlock)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }

  uint32_t startAddress = pciAddress.getAddress() + baseRegisters[barId] + offset;
  if ( (startAddress) % sizeof(uint32_t) != 0 ) {
    std::stringstream text;
    text << "the startaddress must be aligned for the data width " 
         << std::dec << sizeof(uint32_t) << ".\n"
         << "     But the start address is " 
         << std::hex << std::setw(8) << std::setfill('0') << startAddress 
         << "\n     (HAL::PCIExprDevice::readBlock)" << std::ends;
    throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  pciBusAdapter.readBlock( *deviceIdentifierPtr, 
                           startAddress,
                           length,
                           buffer,
                           addressBehaviour);
}

void HAL::PCIExprDevice::hardwareReadBlock64( const HAL::GeneralHardwareAddress& pciAddress,
					uint64_t length,
					char *buffer,
					HalAddressIncrement addressBehaviour,
					uint64_t offset) const
  throw (HAL::IllegalValueException, 
         HAL::UnsupportedException, 
         HAL::IllegalOperationException,
         HAL::BusAdapterException) {

  if ( !pciAddress.isMemorySpace() ) {
    std::string text = "Only implemented for Memory Space\n     (HAL::PCIExprDevice::readBlock64)";
    throw( HAL::IllegalOperationException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }

  if ( (length) % sizeof(uint64_t) != 0 ) {
    std::string text = "the length is not a multiple of the data width!\n     (HAL::PCIExprDevice::readBlock64)";
    throw( HAL::IllegalValueException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }

  uint64_t barId = pciAddress.getMapId();
  if ( barId < 0 || barId > 6 ) {
    std::stringstream text;
    text << "barId out of range :" << barId
         << "\n    must be between 0 and 5"
         << "\n    (HAL::PCIExprDevice::readBlock64)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
  if ( baseRegisters[barId]  == 0 ) {
    std::stringstream text;
    text << "barId " << std::dec << barId 
         << " cannot be used since it is not mapped.\n"
         << "    This is because no item with this barId is in the HAL::AddressTable."
         << "    (HAL::PCIExprDevice::readBlock64)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }

  uint64_t startAddress = pciAddress.getAddress() + baseRegisters[barId] + offset;
  if ( (startAddress) % sizeof(uint64_t) != 0 ) {
    std::stringstream text;
    text << "the startaddress must be aligned for the data width " 
         << std::dec << sizeof(uint64_t) << ".\n"
         << "     But the start address is " 
         << std::hex << std::setw(16) << std::setfill('0') << startAddress 
         << "\n     (HAL::PCIExprDevice::readBlock64)" << std::ends;
    throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  pciBusAdapter.readBlock64( *deviceIdentifierPtr, 
                           startAddress,
                           length,
                           buffer,
                           addressBehaviour);
}


void HAL::PCIExprDevice::memoryWrite( uint32_t address,
				  uint32_t barId,
				  uint32_t data ) const
  throw (HAL::IllegalValueException,
         HAL::BusAdapterException) {
  if ( barId < 0 || barId > 6 ) {
    std::stringstream text;
    text << "barId out of range :" << barId
         << "\n    must be between 0 and 5"
         << "\n    (HAL::PCIExprDevice::memoryWrite)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
  if ( baseRegisters[barId]  == 0 ) {
    std::stringstream text;
    text << "barId " << std::dec << barId 
         << " cannot be used since it is not mapped.\n"
         << "    This is because no item with this barId is in the HAL::AddressTable."
         << "    (HAL::PCIExprDevice::memoryWrite)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  pciBusAdapter.write( *deviceIdentifierPtr, address + baseRegisters[barId], data );
}

void HAL::PCIExprDevice::memoryWrite64( uint64_t address,
				  uint64_t barId,
				  uint64_t data ) const
  throw (HAL::IllegalValueException,
         HAL::BusAdapterException) {
  if ( barId < 0 || barId > 6 ) {
    std::stringstream text;
    text << "barId out of range :" << barId
         << "\n    must be between 0 and 5"
         << "\n    (HAL::PCIExprDevice::memoryWrite64)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
  if ( baseRegisters[barId]  == 0 ) {
    std::stringstream text;
    text << "barId " << std::dec << barId 
         << " cannot be used since it is not mapped.\n"
         << "    This is because no item with this barId is in the HAL::AddressTable."
         << "    (HAL::PCIExprDevice::memoryWrite64)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  pciBusAdapter.write64( *deviceIdentifierPtr, address + baseRegisters[barId], data );
}


void HAL::PCIExprDevice::memoryRead( uint32_t address,
				 uint32_t barId,
				 uint32_t* result ) const 
  throw (HAL::IllegalValueException,
         HAL::BusAdapterException) {
  if ( barId < 0 || barId > 6 ) {
    std::stringstream text;
    text << "barId out of range :" << barId
         << "\n    must be between 0 and 5"
         << "\n    (HAL::PCIExprDevice::memoryRead)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
  if ( baseRegisters[barId]  == 0 ) {
    std::stringstream text;
    text << "barId " << std::dec << barId 
         << " cannot be used since it is not mapped.\n"
         << "    This is because no item with this barId is in the HAL::AddressTable.\n"
         << "    (HAL::PCIExprDevice::memoryRead)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  pciBusAdapter.read( *deviceIdentifierPtr, address + baseRegisters[barId], result );
}

void HAL::PCIExprDevice::memoryRead64( uint64_t address,
				 uint64_t barId,
				 uint64_t* result ) const 
  throw (HAL::IllegalValueException,
         HAL::BusAdapterException) {
  if ( barId < 0 || barId > 6 ) {
    std::stringstream text;
    text << "barId out of range :" << barId
         << "\n    must be between 0 and 5"
         << "\n    (HAL::PCIExprDevice::memoryRead64)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
  if ( baseRegisters[barId]  == 0 ) {
    std::stringstream text;
    text << "barId " << std::dec << barId 
         << " cannot be used since it is not mapped.\n"
         << "    This is because no item with this barId is in the HAL::AddressTable.\n"
         << "    (HAL::PCIExprDevice::memoryRead64)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  pciBusAdapter.read64( *deviceIdentifierPtr, address + baseRegisters[barId], result );
}


uint64_t HAL::PCIExprDevice::getPCIBusAddress( uint64_t iBar )
  throw (HAL::BusAdapterException) {
  return deviceIdentifierPtr->getPCIBusAddress( iBar );
}


// Hannes Sakulin: To accelerate the JAL (not to be used by users !!!)
void HAL::PCIExprDevice::configWrite( uint32_t address,
                             uint32_t data ) const
  throw (BusAdapterException) {
  pciBusAdapter.configWrite( *deviceIdentifierPtr, address, data );
}

void HAL::PCIExprDevice::configWrite64( uint64_t address,
                             uint64_t data ) const
  throw (BusAdapterException) {
  pciBusAdapter.configWrite64( *deviceIdentifierPtr, address, data );
}
// Hannes Sakulin: To accelerate the JAL (not to be used by users !!!)
void HAL::PCIExprDevice::configRead( uint32_t address,
                            uint32_t* result ) const 
  throw (BusAdapterException) {
  pciBusAdapter.configRead( *deviceIdentifierPtr, address, result );
}
void HAL::PCIExprDevice::configRead64( uint64_t address,
                            uint64_t* result ) const 
  throw (BusAdapterException) {
  pciBusAdapter.configRead64( *deviceIdentifierPtr, address, result );
}

