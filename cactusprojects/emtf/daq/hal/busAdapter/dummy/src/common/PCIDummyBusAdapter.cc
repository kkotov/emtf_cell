#include "hal/PCIDummyBusAdapter.hh"
#include <iomanip>
#include <string.h>

HAL::PCIDummyBusAdapter::PCIDummyBusAdapter( enum VerboseMode verbose, 
					     enum MemoryMode  memoryMode,
					     std::ostream& os ) 
  : os_(os) {
  deviceNumberCounter = 0;
  this->verbose = verbose;
  this->memoryMode = memoryMode;
  os_ << "PCIDummyBusAdapter : constructor has been called :\n" 
      << "                     Thec \"verbose\" flag is set to " << (int)verbose << "\n"
      << "                     The \"memoryMode\" flag is set to " << (int)memoryMode  
      << std::endl;
}

HAL::PCIDummyBusAdapter::~PCIDummyBusAdapter() {
  os_ << "PCIDummyBusAdapter : destructor called" << std::endl;
}

void HAL::PCIDummyBusAdapter::findDeviceByVendor( uint32_t vendorID, 
						  uint32_t deviceID,
						  uint32_t index,
						  const PCIAddressTable& pciAddressTable,
						  PCIDeviceIdentifier** deviceIdentifierPtr,
						  std::vector<uint32_t>& barRegisters,
						  bool swapFlag ) 
  throw() {
  
  os_ << "PCIDummyBusAdapter : find device by Vendor called :"
      << "\n   memory-mapping-mode : " << (int)this->memoryMode 
      << "\n   vendorID            : " << vendorID 
      << "\n   deviceID            : " << deviceID 
      << "\n   index               : " << index
      << "\n" << std::endl;
  
  findDevice( pciAddressTable ,
	      deviceIdentifierPtr,
	      barRegisters,
	      swapFlag);
}


void 
HAL::PCIDummyBusAdapter::findDeviceByBus( uint32_t busID, 
					  uint32_t slotID,
					  uint32_t functionID,
					  const PCIAddressTable& pciAddressTable,
					  PCIDeviceIdentifier** deviceIdentifierPtr,
					  std::vector<uint32_t>& barRegisters,
					  bool swapFlag ) 
  throw() {
  

  os_ << "PCIDummyBusAdapter : find device by Bus called :"
      << "\n   memory-mapping-mode : " << (int)this->memoryMode 
      << "\n   busID               : " << busID 
      << "\n   slotID              : " << slotID 
      << "\n   functionID          : " << functionID
      << "\n" << std::endl;
  
  findDevice( pciAddressTable ,
	      deviceIdentifierPtr,
	      barRegisters,
	      swapFlag);
}


void
HAL::PCIDummyBusAdapter::findDevice( const PCIAddressTable& pciAddressTable,
				     PCIDeviceIdentifier** deviceIdentifierPtr,
				     std::vector<uint32_t>& barRegisters,
				     bool swapFlag ) 
  throw() {
  
  int ic;
  uint32_t uic;

  std::vector<uint32_t> maxAddresses, minAddresses, mapsizes;
  uint32_t maxConfigAddress, minConfigAddress;

  char *configSpacePtr = (char*)0;
  std::vector<char *> memorySpacePtr;
  std::vector<uint32_t>::const_iterator it;

  pciAddressTable.getAddressBoundaries( minConfigAddress, maxConfigAddress,
                                        minAddresses, maxAddresses);
  uint32_t barIntervall = getBARIntervall( maxAddresses );
  char* pointer;

  if ( memoryMode == MEMORY_MAP_ON ) {
    configSpacePtr = maxConfigAddress ? new char[ maxConfigAddress+1 ] : (char*) 0;
    if (configSpacePtr) {
      for (uic=0; uic<=maxConfigAddress; uic++) configSpacePtr[uic] = (char)0x00;
    }

    uint32_t bar = 0xffffffff - (6*barIntervall);
    bar = bar + 1;
    for ( it = maxAddresses.begin(); it != maxAddresses.end(); it++ ) {
      uint32_t nbytes = *it ? *it + 1 : 0;
      pointer = *it ? new char[ nbytes ] : (char*) 0;
      if (pointer) {
        for (uic=0; uic<=*it; uic++) pointer[uic] = (char)0x00;
      }
      memorySpacePtr.push_back( pointer ); 
      barRegisters.push_back( bar );
      mapsizes.push_back( nbytes );
      bar += barIntervall;

    }
  } else {
    // we must also invent some BAR addresses which we put to the top of 
    // the address space. We are generous with the AddressSpace and we 
    // do not care if a BAR is really used or not.
    configSpacePtr = (char*) 0;
    uint32_t bar = 0xffffffff - (6*barIntervall);
    bar = bar + 1;
    for ( ic = 0; ic < 6; ic++ ) {
      barRegisters.push_back( bar );
      memorySpacePtr.push_back( (char*)0 ); 
      bar += barIntervall;
    }
  }

  *deviceIdentifierPtr = new HAL::PCIDummyDeviceIdentifier( deviceNumberCounter,
							    memorySpacePtr,
							    configSpacePtr,	     
							    barRegisters,
							    mapsizes
							    );
  if ( verbose == VERBOSE_ON ) {
    os_ << "\n   opening Device number "<< deviceNumberCounter ;
    os_ << "\n   fake BaseAddresses : "
         << "\n      0 : " << std::hex << std::setw(8) << std::setfill('0') << barRegisters[0]  
         << "\n      1 : " << std::hex << std::setw(8) << std::setfill('0') << barRegisters[1]  
         << "\n      2 : " << std::hex << std::setw(8) << std::setfill('0') << barRegisters[2]  
         << "\n      3 : " << std::hex << std::setw(8) << std::setfill('0') << barRegisters[3]  
         << "\n      4 : " << std::hex << std::setw(8) << std::setfill('0') << barRegisters[4]  
         << "\n      5 : " << std::hex << std::setw(8) << std::setfill('0') << barRegisters[5]  
         << "\n" << std::endl;
    if ( memoryMode == MEMORY_MAP_ON ) {
      os_ << "   mapped memory regions for config space and BARs which are used" << std::endl;
    }
  }

  deviceNumberCounter++;
}

void
HAL::PCIDummyBusAdapter::closeDevice( PCIDeviceIdentifier* PCIDevice ) 
  throw() {
    HAL::PCIDummyDeviceIdentifier* theDevice = dynamic_cast<HAL::PCIDummyDeviceIdentifier*>(PCIDevice);
    os_ << "PCIDummyBusAdapter : closing Device  \n" 
         << "                     " << theDevice->printString() << std::endl;
    delete( PCIDevice );
}

void
HAL::PCIDummyBusAdapter::read( PCIDeviceIdentifier& PCIDevice, 
                          uint32_t address,
                          uint32_t *resultPtr ) 
  throw() {
  HAL::PCIDummyDeviceIdentifier& theDevice = dynamic_cast<HAL::PCIDummyDeviceIdentifier&>(PCIDevice);

  *resultPtr = 0;
  if ( memoryMode == MEMORY_MAP_ON ) {
    char *memoryAddress = theDevice.remap( address );
    memcpy( resultPtr, memoryAddress, 4);
  }
  if ( verbose == VERBOSE_ON ) {
    os_ << std::endl;
    os_ << "PCIDummyBusAdapter : read from Device number " << theDevice.printString() << std::endl;
    os_ << "                     address   : " << std::hex << std::setw(8) << std::setfill('0') << address << std::endl;
    os_ << "                     returning : 0x" << std::hex << std::setw(8) << std::setfill('0') << *resultPtr 
         << " (dec) " << std::dec << *resultPtr << std::endl;
    os_ << std::endl;
  }
}

void
HAL::PCIDummyBusAdapter::write( PCIDeviceIdentifier& PCIDevice, 
                           uint32_t address, 
                           uint32_t data) 
  throw() {
  HAL::PCIDummyDeviceIdentifier& theDevice = dynamic_cast<HAL::PCIDummyDeviceIdentifier&>(PCIDevice);

  if ( memoryMode == MEMORY_MAP_ON ) {
    char *memoryAddress = theDevice.remap( address );
    memcpy( memoryAddress, &data, 4);
  }

  if (verbose == VERBOSE_ON) {
    os_ << std::endl;
    os_ << "PCIDummyBusAdapter : write to Device number " << theDevice.printString() << std::endl;
    os_ << "                     address : " << std::hex << std::setw(8) << std::setfill('0') << address << std::endl;
    os_ << "                   dataValue : 0x" << std::hex << std::setw(8) << std::setfill('0') << data 
         << " (dec) " << std::dec << data << std::endl;
    os_ << std::endl;
  }
}


void HAL::PCIDummyBusAdapter::configWrite( PCIDeviceIdentifier& device, 
					   uint32_t address, 
					   uint32_t data ) 
  throw( ) {

  if ( memoryMode == MEMORY_MAP_ON ) {
    HAL::PCIDummyDeviceIdentifier& theDevice = dynamic_cast<HAL::PCIDummyDeviceIdentifier&>(device);
    char* memoryAddress = address +  theDevice.getConfigSpacePtr();
    memcpy( memoryAddress, &data, 4);
  }

  if ( verbose == VERBOSE_ON ) {
    os_ << std::endl;
    os_ << "PCIDummyBusAdapter : configuration write to Device number " << device.printString() << std::endl;
    os_ << "                     address   : " << std::hex << std::setw(8) << std::setfill('0') << address << std::endl;
    os_ << "                     dataValue : 0x" << std::hex << std::setw(8) << std::setfill('0') << data 
         << " (dec) " << std::dec << data << std::endl;
    os_ << std::endl;
  }
}


void HAL::PCIDummyBusAdapter::configRead( PCIDeviceIdentifier& device,
					  uint32_t address, 
					  uint32_t* resultPtr )
  throw( ) {

  *resultPtr = 0;
  if ( memoryMode == MEMORY_MAP_ON ) {
    HAL::PCIDummyDeviceIdentifier& theDevice = dynamic_cast<HAL::PCIDummyDeviceIdentifier&>(device);
    char* memoryAddress = address + theDevice.getConfigSpacePtr();
    memcpy( resultPtr, memoryAddress, 4);
  }
  if ( verbose == VERBOSE_ON ) {
    os_ << std::endl;
    os_ << "PCIDummyBusAdapter : configuration read from Device number " << device.printString() << std::endl;
    os_ << "                     address   : " << std::hex << std::setw(8) << std::setfill('0') << address << std::endl;
    os_ << "                     returning : 0x" << std::hex << std::setw(8) << std::setfill('0') << *resultPtr 
         << " (dec) " << std::dec << *resultPtr << std::endl;
    os_ << std::endl;
  }
}

void
HAL::PCIDummyBusAdapter::readBlock( PCIDeviceIdentifier &device,
				    uint32_t startAddress,
				    uint32_t length,      // in bytes
				    char *buffer,
				    HalAddressIncrement addressBehaviour ) 
  throw() {
  HAL::PCIDummyDeviceIdentifier& theDevice = dynamic_cast<HAL::PCIDummyDeviceIdentifier&>(device);

  if ( memoryMode == MEMORY_MAP_ON ) {
    char *memoryAddress = theDevice.remap( startAddress );
    if ( addressBehaviour == HAL_DO_INCREMENT ) {
      memcpy( buffer, memoryAddress, length);
    } else {
      os_ << "HAL::PCIDummyBusAdapter::readBlock : No Fifo functionality in PCIDummyBusAdapter. Doing nothing!" << std::endl;
    }
  }

  if( verbose == VERBOSE_ON ) {
    os_ << std::endl;
    os_ << "PCIDummyBusAdapter : readBlock from Device \n                     " << theDevice.printString() << std::endl;
    os_ << "                     startaddress (hex) : " << std::hex << std::setw(8) << std::setfill('0') << startAddress << std::endl;
    os_ << "                     length (hex)       : " << std::hex << std::setw(8) << std::setfill('0') << length << std::endl;
    os_ << "                     addressBehaviour   : ";
    if ( addressBehaviour == HAL_DO_INCREMENT ) {
      os_ << "HAL_DO_INCREMENT" << std::endl;
    } else if ( addressBehaviour == HAL_NO_INCREMENT ) {
      os_ << "HAL_NO_INCREMENT" << std::endl;
    } else { // should never happen
      os_ << "XXXXXXXXXXXXXXXX" << std::endl;
    }
    os_ << "   pointer to readbuffer (hex) : " << std::hex << std::setw(8) << std::setfill('0') << buffer << std::endl;
    os_ << std::endl;
  }
}
  
void
HAL::PCIDummyBusAdapter::writeBlock( PCIDeviceIdentifier& device,
				     uint32_t startAddress,
				     uint32_t length,      // in bytes
				     char *buffer,
				     HalAddressIncrement addressBehaviour ) 
  throw() {
  
  HAL::PCIDummyDeviceIdentifier& theDevice = dynamic_cast<HAL::PCIDummyDeviceIdentifier&>(device);

  if ( memoryMode == MEMORY_MAP_ON ) {
    char *memoryAddress = theDevice.remap( startAddress );
    if ( addressBehaviour == HAL_DO_INCREMENT ) {
      memcpy( memoryAddress, buffer, length);
     } else {
       os_ << "HAL::PCIDummyBusAdapter::writeBlock : No Fifo functionality in PCIDummyBusAdapter. Doing nothing!" << std::endl;
     }
  }

  if ( verbose == VERBOSE_ON ) {
    os_ << std::endl;
    os_ << "PCIDummyBusAdapter : writeBlock to Device \n                     " << theDevice.printString() << std::endl;
    os_ << "                     startaddress (hex) : " << std::hex << std::setw(8) << std::setfill('0') << startAddress << std::endl;
    os_ << "                     length (hex)       : " << std::hex << std::setw(8) << std::setfill('0') << length << std::endl;
    os_ << "                     addressBehaviour   : ";
    if ( addressBehaviour == HAL_DO_INCREMENT ) {
      os_ << "HAL_DO_INCREMENT" << std::endl;
    } else if ( addressBehaviour == HAL_NO_INCREMENT ) {
      os_ << "HAL_NO_INCREMENT" << std::endl;
    } else { // should never happen
      os_ << "XXXXXXXXXXXXXXXX" << std::endl;
    }
    os_ << "   pointer to sourcebuffer (hex) : " << std::hex << std::setw(8) << std::setfill('0') << buffer << std::endl;
    os_ << std::endl;
  }
}
 
uint32_t HAL::PCIDummyBusAdapter::getBARIntervall( const std::vector<uint32_t>& maxAddresses ) {
  uint32_t intervall = 1;
  std::vector<uint32_t>::const_iterator it;
  for ( it = maxAddresses.begin(); it != maxAddresses.end(); it++ ) {
    while (*it > intervall) {
      intervall <<= 1;
    }
  }
  os_ << "found intervall " << std::hex << intervall << std::endl;
  return intervall;
}

