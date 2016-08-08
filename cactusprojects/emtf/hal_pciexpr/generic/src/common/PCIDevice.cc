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

  baseRegisters = baseAddresses;
  pciBusAdapter.findDeviceByVendor( vendorID, deviceID, index, pciAddressTable,
                            &deviceIdentifierPtr, baseRegisters );
  pciBusAdapter.configWrite(*deviceIdentifierPtr,
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

/* MTF6/7 specific */
HAL::PCIDevice::PCIDevice( HAL::PCIAddressTable & addressTable,
			   HAL::PCIBusAdapterInterface & pciBusAdapter,
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

HAL::PCIDevice::~PCIDevice() {
  pciBusAdapter.closeDevice( deviceIdentifierPtr );
}

uint32_t HAL::PCIDevice::getItemAddress( std::string item ) const 
  throw ( HAL::NoSuchItemException,
	  HAL::IllegalOperationException ) {
  const HAL::GeneralHardwareAddress& pciAddress = 
    pciAddressTable.getGeneralHardwareAddress( item );
  if ( ! pciAddress.isMemorySpace() ) {
    std::string text = "Operation only allowed for Memory Space Items\n     (HAL::PCIDevice::getItemAddress)";
    throw (HAL::IllegalOperationException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }
  uint32_t address = pciAddress.getAddress();
  uint32_t barId = pciAddress.getMapId();
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

void HAL::PCIDevice::hardwareRead( const HAL::GeneralHardwareAddress& pciAddress, 
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

void HAL::PCIDevice::memoryRead( uint32_t address,
				 uint32_t barId,
				 uint32_t* result ) const 
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
         << "    This is because no item with this barId is in the HAL::AddressTable.\n"
         << "    (HAL::PCIDevice::memoryRead)" << std::ends;
    throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  pciBusAdapter.read( *deviceIdentifierPtr, address + baseRegisters[barId], result );
}

uint32_t HAL::PCIDevice::getPCIBusAddress( uint32_t iBar )
  throw (HAL::BusAdapterException) {
  return deviceIdentifierPtr->getPCIBusAddress( iBar );
}


// Hannes Sakulin: To accelerate the JAL (not to be used by users !!!)
void HAL::PCIDevice::configWrite( uint32_t address,
                             uint32_t data ) const
  throw (BusAdapterException) {
  pciBusAdapter.configWrite( *deviceIdentifierPtr, address, data );
}

// Hannes Sakulin: To accelerate the JAL (not to be used by users !!!)
void HAL::PCIDevice::configRead( uint32_t address,
                            uint32_t* result ) const 
  throw (BusAdapterException) {
  pciBusAdapter.configRead( *deviceIdentifierPtr, address, result );
}

