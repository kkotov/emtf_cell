#include "hal/PCILinuxBusAdapter.hh"

#include <sstream>
#include <iomanip>

HAL::PCILinuxBusAdapter::PCILinuxBusAdapter() throw (HAL::BusAdapterException)
  try
  : PCIBusAdapterInterface(),
    pciBus_() {}
  catch ( xpci::exception::IOError &e ) {
    std::string err = std::string( e.what() );
    throw BusAdapterException(  e.what(),  __FILE__, __LINE__, __FUNCTION__ );
  }

HAL::PCILinuxBusAdapter::~PCILinuxBusAdapter() {
}

void HAL::PCILinuxBusAdapter::findDeviceByIndex( uint32_t index )
  throw (BusAdapterException,
	 NoSuchDeviceException) {
}

void HAL::PCILinuxBusAdapter::findDeviceByBus( uint32_t busID, 
					       uint32_t slotID,
					       uint32_t functionID,
					       const PCIAddressTable& pciAddressTable,
					       PCIDeviceIdentifier** deviceIdentifierPtr,
					       std::vector<uint32_t>& baseAddresses,
					       bool swapFlag )
  throw (BusAdapterException,
	 NoSuchDeviceException) {
  
  xpci::Address deviceConfigAddress = 
    xpci::Address::getConfigSpaceAddressByBus(busID, slotID, functionID);

  findDevice( deviceConfigAddress,
	      pciAddressTable,
	      deviceIdentifierPtr,
	      baseAddresses,
	      swapFlag );
}
    
void HAL::PCILinuxBusAdapter::findDeviceByVendor( uint32_t vendorID, 
						  uint32_t deviceID,
						  uint32_t index,
						  const PCIAddressTable& pciAddressTable,
						  PCIDeviceIdentifier** deviceIdentifierPtr,
						  std::vector<uint32_t>& baseAddresses,
						  bool swapFlag )
  throw (BusAdapterException,
	 NoSuchDeviceException) {

  xpci::Address deviceConfigAddress = 
    xpci::Address::getConfigSpaceAddressByVendor(vendorID, deviceID, index);

  findDevice( deviceConfigAddress,
	      pciAddressTable,
	      deviceIdentifierPtr,
	      baseAddresses,
	      swapFlag );
}
    
void HAL::PCILinuxBusAdapter::findDevice( xpci::Address& deviceConfigAddress,
					  const PCIAddressTable& pciAddressTable,
					  PCIDeviceIdentifier** deviceIdentifierPtr,
					  std::vector<uint32_t>& baseAddresses,
					  bool swapFlag )
throw (BusAdapterException,
       NoSuchDeviceException) {

  *deviceIdentifierPtr = (PCIDeviceIdentifier*) 0; // default: unvalid

  // check if the device has been found, otherwise throw exception
  try {
    uint32_t val;
    pciBus_.read (deviceConfigAddress, 0x00000010, val);
  } catch ( xpci::exception::IOError e) {
    std::stringstream text;
    text << "Could not find PCI device \n"
         << std::ends;
    std::string textStr = text.str();
    throw ( NoSuchDeviceException( textStr, __FILE__, __LINE__, __FUNCTION__ ));
  }
  HAL::PCILinuxDeviceIdentifier* newIdentifierPtr = 
    new HAL::PCILinuxDeviceIdentifier(deviceConfigAddress, swapFlag);
  uint32_t minConfigAddress, maxConfigAddress;
  std::vector<uint32_t> minAddresses, maxAddresses;
  pciAddressTable.getAddressBoundaries( minConfigAddress, maxConfigAddress,
                                        minAddresses, maxAddresses );
  baseAddresses.clear();
  
  try {
    for ( int ix = 0; ix<6; ix++ ) {
      xpci::Address ba = pciBus_.BAR(ix, deviceConfigAddress);
      std::cout << "BAR" << ix << "=" << std::hex << "0x" << ba.getAddress() << std::endl;
      baseAddresses.push_back(ba.getAddress());
    }
  } catch ( xpci::exception::IOError &e ) {
    throw BusAdapterException( e.what(), __FILE__, __LINE__, __FUNCTION__ );
  } 


  *deviceIdentifierPtr = newIdentifierPtr;
}

void HAL::PCILinuxBusAdapter::configWrite( PCIDeviceIdentifier& pciDevice,
                                     uint32_t address,
                                     uint32_t data) 
  throw( BusAdapterException ){
  if ( (address%4 != 0) ) {
    std::stringstream text;
    text << "address or length not aligned to 4 byte boundary " 
         << "\n     address (hex) : " << std::hex << address 
         << std::ends;
    throw( BusAdapterException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  xpci::Address pciConfigAddress 
    = (dynamic_cast<HAL::PCILinuxDeviceIdentifier&>(pciDevice)).getConfigAddress();
  try {
    pciBus_.write(pciConfigAddress, address, data);
  } catch( xpci::exception::IOError &e ) {
    std::stringstream text;
    text << "error during write to address  " 
         << std::hex << address 
	 << " (data : " << data << ")"
         << std::ends;
    throw( BusAdapterException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
}

void HAL::PCILinuxBusAdapter::configRead( PCIDeviceIdentifier& pciDevice,
					  uint32_t address,
					  uint32_t* data) 
  throw( BusAdapterException ){
  if ( (address%4 != 0) ) {
    std::stringstream text;
    text << "address or length not aligned to 4 byte boundary " 
         << "\n     address (hex) : " << std::hex << address 
         << std::ends;
    throw( BusAdapterException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  xpci::Address pciConfigAddress 
    = (dynamic_cast<HAL::PCILinuxDeviceIdentifier&>(pciDevice)).getConfigAddress();
  try {
	uint32_t val;
     pciBus_.read(pciConfigAddress, address, val);
    *data = val;
  } catch( xpci::exception::IOError &e ) {
    std::stringstream text;
    text << "error during read from address  " 
         << std::hex << address 
         << std::ends;
    throw( BusAdapterException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
}

void HAL::PCILinuxBusAdapter::write( PCIDeviceIdentifier& device,
				     uint32_t address, 
				     uint32_t data ) 
  throw( BusAdapterException ) {
  if ( (address%4 != 0) ) {
    std::stringstream text;
    text << "address or length not aligned to 4 byte boundary " 
         << "\n     address (hex) : " << std::hex << address 
         << std::ends;
    throw( BusAdapterException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }

  xpci::Address pciAddress = xpci::Address::getMemorySpaceAddress( address );
  try {
    if ( device.doSwap() ){
      pciBus_.write( pciAddress, 0, bswap_32(data) );
    } else {
      pciBus_.write( pciAddress, 0, data );
    }
  } catch( xpci::exception::IOError &e ) {
    std::stringstream text;
    text << "error during write to address  " 
         << std::hex << address 
	 << " (data : " << data << ")"
         << std::ends;
    throw( BusAdapterException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
}

void HAL::PCILinuxBusAdapter::read( PCIDeviceIdentifier& device,
				   uint32_t address, 
				   uint32_t* result ) 
  throw( BusAdapterException ) {
  if ( (address%4 != 0) ) {
    std::stringstream text;
    text << "address or length not aligned to 4 byte boundary " 
         << "\n     address (hex) : " << std::hex << address 
         << std::ends;
    throw( BusAdapterException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }

  xpci::Address pciAddress = xpci::Address::getMemorySpaceAddress( address );
  try {
    if ( device.doSwap() ){
	uint32_t val;
	pciBus_.read( pciAddress, 0 , val );
	*result = bswap_32( val );
    } else {
	 uint32_t val;
	pciBus_.read( pciAddress, 0, val );
        *result = val;
    }
  } catch( xpci::exception::IOError &e ) {
    std::stringstream text;
    text << "error during read from address  " 
         << std::hex << address 
         << std::ends;
    throw( BusAdapterException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }

}

void HAL::PCILinuxBusAdapter::writeBlock( PCIDeviceIdentifier& device,
                                    uint32_t startAddress,
                                    uint32_t length,
                                    char *buffer,
                                    HalAddressIncrement addressBehaviour) 
  throw( BusAdapterException ) {
  // let's require the address to be aligned to 32 bit and then read
  // uint32_ts
  if ( (startAddress%4 != 0) || 
       (length%4 != 0) ) {
    std::stringstream text;
    text << "Start address or length not aligned to 4 byte boundary " 
         << "\n     StartAddress (hex) : " << std::hex << startAddress 
         << "\n     BlockLength  (hex) : " << std::hex << length
         << std::ends;
    std::string textStr = text.str();
    throw( BusAdapterException( textStr, __FILE__, __LINE__, __FUNCTION__ ) );
  }

  uint32_t* sourcePtr = (uint32_t*) buffer;
  uint32_t nWords = length/4;
  uint32_t ioff=0;
  xpci::Address pciAddress = xpci::Address::getMemorySpaceAddress( startAddress );

  try {
    if ( device.doSwap() ) {
      
      if ( addressBehaviour == HAL_DO_INCREMENT ) {
	for ( uint32_t ic = 0; ic<nWords; ic++, sourcePtr++, ioff+=4 ) {
	  pciBus_.write( pciAddress, ioff, bswap_32(*sourcePtr) );
	}
      } else { // do not increment destination address 
	for (uint32_t ic = 0; ic<nWords; ic++, sourcePtr++ ) {
	  pciBus_.write( pciAddress, 0, bswap_32(*sourcePtr) );
	}
      }

    } else {
      if ( addressBehaviour == HAL_DO_INCREMENT ) {
	for ( uint32_t ic = 0; ic<nWords; ic++, ioff+=4, sourcePtr++ ) {
	  pciBus_.write( pciAddress, ioff, *sourcePtr );
	}
      } else { // do not increment destination address 
	for (uint32_t ic = 0; ic<nWords; ic++, sourcePtr++ ) {
	  pciBus_.write( pciAddress, 0, *sourcePtr );
	}
      }
    }
  } catch( xpci::exception::IOError &e ) {
    std::stringstream text;
    text << "error during writeBlock to address  " 
         << std::hex << startAddress 
         << std::ends;
    throw( BusAdapterException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
}
  
void HAL::PCILinuxBusAdapter::readBlock(  PCIDeviceIdentifier& device,
                                    uint32_t startAddress,
                                    uint32_t length,
                                    char *buffer,
                                    HalAddressIncrement addressBehaviour) 
  throw( BusAdapterException ) {
  // let's require the address to be aligned to 32 bit and then read
  // uint32_ts
  if ( (startAddress%4 != 0) || 
       (length%4 != 0) ) {
    std::stringstream text;
    text << "Start address or length not aligned to 4 byte boundary " 
         << "\n     StartAddress (hex) : " << std::hex << startAddress 
         << "\n     BlockLength  (hex) : " << std::hex << length
         << std::ends;
    std::string textStr = text.str();
    throw( BusAdapterException( textStr, __FILE__, __LINE__, __FUNCTION__ ) );
  }

  uint32_t* destPtr   = (uint32_t*) buffer;
  uint32_t nWords = length/4;
  uint32_t ioff = 0;
  xpci::Address pciAddress = xpci::Address::getMemorySpaceAddress( startAddress );
  try {
    if ( device.doSwap() ){
      // byte swapping
      if ( addressBehaviour == HAL_DO_INCREMENT ) {
	for ( uint32_t ic = 0; ic<nWords; ic++, destPtr++, ioff+=4 ) {
           uint32_t val;
	  pciBus_.read( pciAddress, ioff, val );
	  *destPtr = bswap_32( val );
	}
      } else {                // do not increment source address 
	for (uint32_t ic = 0; ic<nWords; ic++, destPtr++ ) {
           uint32_t val;
	pciBus_.read( pciAddress, 0, val ); 
	  *destPtr = bswap_32( val );
	}
      }
      // no byte swapping
    } else {
      if ( addressBehaviour == HAL_DO_INCREMENT ) {
	for ( uint32_t ic = 0; ic<nWords; ic++, destPtr++, ioff+=4 ) {
	uint32_t val;
	pciBus_.read( pciAddress, ioff , val );
	  *destPtr = val;
	}
      } else {                // do not increment source address 
	for (uint32_t ic = 0; ic<nWords; ic++, destPtr++ ) {
	uint32_t val;
	   pciBus_.read( pciAddress, 0, val );
	  *destPtr = val;
	}
      }
    }
  } catch( xpci::exception::IOError &e ) {
    std::stringstream text;
    text << "error during readBlock from startAddress  " 
         << std::hex << startAddress 
         << std::ends;
    throw( BusAdapterException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
}
  
void HAL::PCILinuxBusAdapter::closeDevice( PCIDeviceIdentifier* deviceIdentifierPtr )
  throw() {
  delete(deviceIdentifierPtr); // this destroys the maps !!!
}

