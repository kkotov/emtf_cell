#include "hal/PCIExprLinuxBusAdapter.hh"

#include <sstream>
#include <iomanip>

HAL::PCIExprLinuxBusAdapter::PCIExprLinuxBusAdapter() throw (HAL::BusAdapterException)
  try
    : PCIBusAdapterInterface(),
      pciBus_(){}
  catch ( xpci::exception::IOError &e ) {
    std::string err = std::string( e.what() );
    throw BusAdapterException(  e.what(),  __FILE__, __LINE__, __FUNCTION__ );
  }

HAL::PCIExprLinuxBusAdapter::~PCIExprLinuxBusAdapter() {
}

void HAL::PCIExprLinuxBusAdapter::findDeviceByIndex( uint32_t index ) 

  throw (BusAdapterException,
	 NoSuchDeviceException) {
  
  std::cout << "Initializing the MTF7 board #" << index;

  char driver_string[100];
  sprintf(driver_string, "%s%d", PCIEXPR_DEFAULT_FILE, index);
  std::cout << ": driver is " << driver_string << std::endl;
  device_d = open(driver_string,O_RDWR);
  if (device_d < 0) {
    std::cout << "ERROR: Can not open device file for SP12" << std::endl;
  }
} 

/*
  Currently the driver for the MTF6(7) reads  Vendor and Device ID to make sure it talks to the appropriate 
  boards. It also reads BAR registers to learn the starting addresses and sizes of address space windows 
  allocated for each board. However, the user-mode software is isolated from these details. As soon as one 
  opens a /dev/utca_sp12X device, we know that this is one of the boards.

  As a consequence the methods findDeviceByBus, findDeviceByVendor and findDevice are temporarily left
  empty as they are not useful.
*/
void HAL::PCIExprLinuxBusAdapter::findDeviceByBus( uint32_t busID, 
					       uint32_t slotID,
					       uint32_t functionID,
					       const PCIAddressTable& pciAddressTable,
					       PCIDeviceIdentifier** deviceIdentifierPtr,
					       std::vector<uint32_t>& baseAddresses,
					       bool swapFlag )
  throw (BusAdapterException,
	 NoSuchDeviceException) {
}
    
void HAL::PCIExprLinuxBusAdapter::findDeviceByVendor( uint32_t vendorID, 
						  uint32_t deviceID,
						  uint32_t index,
						  const PCIAddressTable& pciAddressTable,
						  PCIDeviceIdentifier** deviceIdentifierPtr,
						  std::vector<uint32_t>& baseAddresses,
						  bool swapFlag )
  throw (BusAdapterException,
	 NoSuchDeviceException) {
}
    
void HAL::PCIExprLinuxBusAdapter::findDevice( xpci::Address& deviceConfigAddress,
					  const PCIAddressTable& pciAddressTable,
					  PCIDeviceIdentifier** deviceIdentifierPtr,
					  std::vector<uint32_t>& baseAddresses,
					  bool swapFlag )
throw (BusAdapterException,
       NoSuchDeviceException) {
}

void HAL::PCIExprLinuxBusAdapter::configWrite( PCIDeviceIdentifier& pciDevice,
                                     uint32_t address,
                                     uint32_t data) 
  throw( BusAdapterException ){
  /*if ( (address%4 != 0) ) {
    std::stringstream text;
    text << "address or length not aligned to 4 byte boundary " 
         << "\n     address (hex) : " << std::hex << address 
         << std::ends;
    throw( BusAdapterException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  xpci::Address pciConfigAddress 
    = (dynamic_cast<HAL::PCIExprLinuxDeviceIdentifier&>(pciDevice)).getConfigAddress();
  try {
    pciBus_.write(pciConfigAddress, address, data);
  } catch( xpci::exception::IOError &e ) {
    std::stringstream text;
    text << "error during write to address  " 
         << std::hex << address 
	 << " (data : " << data << ")"
         << std::ends;
    throw( BusAdapterException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
    }*/
}

void HAL::PCIExprLinuxBusAdapter::configRead( PCIDeviceIdentifier& pciDevice,
					  uint32_t address,
					  uint32_t* data) 
  throw( BusAdapterException ){
  /*if ( (address%4 != 0) ) {
    std::stringstream text;
    text << "address or length not aligned to 4 byte boundary " 
         << "\n     address (hex) : " << std::hex << address 
         << std::ends;
    throw( BusAdapterException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  xpci::Address pciConfigAddress 
    = (dynamic_cast<HAL::PCIExprLinuxDeviceIdentifier&>(pciDevice)).getConfigAddress();
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
    }*/
}

void HAL::PCIExprLinuxBusAdapter::write( PCIDeviceIdentifier& device,
					 uint32_t address, 
					 uint32_t data ) 
  throw( BusAdapterException ) {
  if ( (address%4 != 0) ) {
    std::stringstream text;
    text << "address or length not aligned to 4 bytes boundary " 
         << "\n     address (hex) : " << std::hex << address 
         << std::ends;
    throw( BusAdapterException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }

  // mwrite is the write function defined in the mtf7 driver
  try {
    mwrite(device_d, &data, 4, address);
  }
  catch ( xpci::exception::IOError &e ) { 	
    std::stringstream text;
    text << "error during write to address  " 
         << std::hex << address 
	 << " (data : " << data << ")"
         << std::ends;
    throw( BusAdapterException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
}

void HAL::PCIExprLinuxBusAdapter::read( PCIDeviceIdentifier& device,
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

  try {
  // mread is the standard read function defined in the mtf7 driver 
    mread(device_d, result, 4, address);
  } catch( xpci::exception::IOError &e ) {
    std::stringstream text;
    text << "error during read from address  " 
         << std::hex << address 
         << std::ends;
    throw( BusAdapterException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
}

void HAL::PCIExprLinuxBusAdapter::writeBlock( PCIDeviceIdentifier& device,
					      uint32_t startAddress,
					      uint32_t length,
					      char *buffer,
					      HalAddressIncrement addressBehaviour) 
  throw( BusAdapterException ) {
 
  //std::cout << "Do nothing for the time being..." << std::endl;

  //std::cout << "startAddress="<< startAddress << std::endl;
  //std::cout << "length="      << length       << std::endl;
  //std::cout << "buffer="      << buffer       << std::endl; //this does not print anything

  const int nWords = length/4;
  uint32_t write_buf[nWords];
    
  uint32_t* sourcePtr = (uint32_t*) buffer;
  uint32_t ic;
  for ( ic = 0; ic<nWords; ic++, sourcePtr++ ) {
    //std::cout << "sourcePtr in writeBlock= " << *sourcePtr << std::endl;
    write_buf[ic] = *sourcePtr;
    //std::cout << std::dec << "HAL::PCIExprLinuxBusAdapter::writeBlock -> write_buf[" << ic << "]=" << write_buf[ic] << std::endl;
  }

  try {
    mwrite(device_d, write_buf, length, startAddress-0x10);
  }
  catch( xpci::exception::IOError &e ) {
    std::stringstream text;
    text << "error during writeBlock to address  " 
         << std::hex << startAddress 
         << std::ends;
    throw( BusAdapterException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }

  // should I add an extra method? for like uint64_t and no HalAddressIncrement?...
  // let's require the address to be aligned to 64 bit and then read
  // uint32_ts
  //if ( (startAddress%8 != 0) || 
  //     (length%8 != 0) ) {
  //  std::stringstream text;
  //  text << "Start address or length not aligned to 8 byte boundary " 
  //       << "\n     StartAddress (hex) : " << std::hex << startAddress 
  //       << "\n     BlockLength  (hex) : " << std::hex << length
  //       << std::ends;
  //  std::string textStr = text.str();
  //  throw( BusAdapterException( textStr, __FILE__, __LINE__, __FUNCTION__ ) );
  //}
  //

 /* // let's require the address to be aligned to 32 bit and then read
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
    }*/
}
  
void HAL::PCIExprLinuxBusAdapter::readBlock(  PCIDeviceIdentifier& device,
					      uint32_t startAddress,
					      uint32_t length,
					      char *buffer,
					      HalAddressIncrement addressBehaviour) 
  throw( BusAdapterException ) {

  // std::cout << "startAddress=" << startAddress-0x10 << std::endl;
  //std::cout << "length="       << length << std::endl;
  try {
    mread(device_d, buffer, length, startAddress-0x10);
  }
  catch( xpci::exception::IOError &e ) {
    std::stringstream text;
    text << "error during readBlock from startAddress  " 
         << std::hex << startAddress 
         << std::ends;
    throw( BusAdapterException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }

 //std::cout << "mread command issued\n";

  // const int nWords = length/4;
  // uint32_t read_buf[nWords];

  // uint32_t* sourcePtr = (uint32_t*) buffer;
  // uint32_t ic;
  // for ( ic = 0; ic<nWords; ic++, sourcePtr++ ) {
  //   read_buf[ic] = *sourcePtr;
  //   std::cout << "read_buf[" << ic << "]=" << read_buf[ic] << std::endl;
  // }
  
  /* // let's require the address to be aligned to 32 bit and then read
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
    }*/
}
  
void HAL::PCIExprLinuxBusAdapter::closeDevice( PCIDeviceIdentifier* deviceIdentifierPtr )
  throw() {
  /*delete(deviceIdentifierPtr); // this destroys the maps !!! */
}

