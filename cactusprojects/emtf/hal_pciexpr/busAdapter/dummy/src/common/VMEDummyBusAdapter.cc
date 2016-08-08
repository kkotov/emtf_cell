#include "hal/VMEDummyBusAdapter.hh"
#include <iomanip>
#include <iostream>
#include <string.h>

// dirty but ok...:
typedef unsigned short U16;
typedef uint32_t U32;

HAL::VMEDummyBusAdapter::VMEDummyBusAdapter( enum VerboseMode verbose, 
                                        enum MemoryMode  memoryMode,
                                        std::ostream& os )
    : os_( os ) {
    deviceNumberCounter = 0;
    this->verbose = verbose;
    this->memoryMode = memoryMode;
    os_  << "VMEDummyBusAdapter : constructor has been called :\n" 
         << "                     The \"verbose\" flag is set to " << (int)verbose << "\n"
         << "                     The \"memoryMode\" flag is set to " << (int)memoryMode  
         << std::endl;
}

HAL::VMEDummyBusAdapter::~VMEDummyBusAdapter() {
    os_  << "VMEDummyBusAdapter : destructor called" << std::endl;
}

void
HAL::VMEDummyBusAdapter::openDevice(const VMEAddressTable& vmeAddressTable,
                               uint32_t vmeBaseaddress,
                               DeviceIdentifier** deviceIdentifierPtr,
                               uint32_t* baseAddressPtr,
                               bool doSwapping ) 
    throw() {
    std::vector<char *> memoryRegions(NUMBER_OF_VME64XFUNCTIONS);
    for ( int i=0; i<NUMBER_OF_VME64XFUNCTIONS; i++ ) memoryRegions.push_back( (char*) 0 );

    uint32_t ic;
    os_  << "VMEDummyBusAdapter : opening Device number "<< deviceNumberCounter 
         << " with baseAddress : " 
         << std::hex << std::setw(8) << std::setfill('0') << vmeBaseaddress
         << "\n                     memory-mapping-mode : "
         << (int)this->memoryMode 
         << std::endl;

    std::vector<uint32_t> minAddresses(NUMBER_OF_VME64XFUNCTIONS); 
    std::vector<uint32_t> maxAddresses(NUMBER_OF_VME64XFUNCTIONS);
    std::vector<uint32_t> baseAddresses(NUMBER_OF_VME64XFUNCTIONS);
    std::vector<uint32_t> mapSizes(NUMBER_OF_VME64XFUNCTIONS);

    for ( int i=0; i<NUMBER_OF_VME64XFUNCTIONS; i++ ) {
      minAddresses[i] = 0;
      maxAddresses[i] = 0;
      baseAddresses[i] = 0;
      mapSizes[i] = 0;
    }

    if ( memoryMode == MEMORY_MAP_ON ) {

        vmeAddressTable.getAddressBoundaries( minAddresses, maxAddresses );

        *baseAddressPtr = vmeBaseaddress;

	// it is assumed that the mapped region starts at an item which corresponds
	// to an AddressTable entry at '0'.
 	uint32_t nbytes = maxAddresses[0] + 1;

        memoryRegions[0] = new char[ nbytes ];
	baseAddresses[0] = vmeBaseaddress;
	mapSizes[0]      = nbytes;

        for (ic=0; ic<=maxAddresses[0]; ic++) memoryRegions[0][ic] = (char)0x00;

        os_  << "                     mapped the address space to memory address " 
             << std::hex << memoryRegions[0] 
             << "\n                     reserved 0x" << std::hex << nbytes
             << " (dec: " << std::dec << nbytes << " )" 
             << " bytes for memory mapped operation" << std::endl; 
    } else {
        *baseAddressPtr = vmeBaseaddress;
    }

    *deviceIdentifierPtr = new HAL::VMEDummyDeviceIdentifier( deviceNumberCounter,
							      memoryRegions,
							      baseAddresses,
							      mapSizes,
							      doSwapping );
    deviceNumberCounter++;
}

// the call for VME64x modules
void
HAL::VMEDummyBusAdapter::openDevice( const VMEAddressTable& vmeAddressTable,
				     std::vector<uint32_t>& vmeBaseaddresses,
				     DeviceIdentifier** deviceIdentifierPtr,
				     std::vector<uint32_t>* baseAddressPtr,
				     bool doSwapping ) 
    throw() {
    std::vector<char *> memoryRegions(NUMBER_OF_VME64XFUNCTIONS);
    for ( int i=0; i<NUMBER_OF_VME64XFUNCTIONS; i++ ) memoryRegions.push_back( (char*) 0 );

    uint32_t ic;
    os_  << "VMEDummyBusAdapter : opening VME64x Device number "<< deviceNumberCounter 
         << "\n                     memory-mapping-mode : "
         << (int)this->memoryMode 
         << std::endl;

    std::vector<uint32_t> minAddresses(NUMBER_OF_VME64XFUNCTIONS); 
    std::vector<uint32_t> maxAddresses(NUMBER_OF_VME64XFUNCTIONS);
    std::vector<uint32_t> baseAddresses(NUMBER_OF_VME64XFUNCTIONS);
    std::vector<uint32_t> mapSizes(NUMBER_OF_VME64XFUNCTIONS);

    for ( int i=0; i<NUMBER_OF_VME64XFUNCTIONS; i++ ) {
      minAddresses[i] = 0;
      maxAddresses[i] = 0;
      baseAddresses[i] = 0;
      mapSizes[i] = 0;
    }

    if ( memoryMode == MEMORY_MAP_ON ) {

        vmeAddressTable.getAddressBoundaries( minAddresses, maxAddresses );

        for ( int ifunc=0; ifunc<NUMBER_OF_VME64XFUNCTIONS; ifunc++ ) {

            if ( maxAddresses[ifunc] > 0 ) {

                (*baseAddressPtr)[ifunc] = vmeBaseaddresses[ifunc];
	        
		// it is assumed that the mapped region starts at an item which corresponds
		// to an AddressTable entry at '0'.
                uint32_t nbytes = maxAddresses[ifunc] + 1;
	        
	        memoryRegions[ifunc] = new char[ nbytes ];
	        baseAddresses[ifunc] = vmeBaseaddresses[ifunc];
	        mapSizes[ifunc]      = nbytes;

                for (ic=0; ic<maxAddresses[ifunc]; ic++) memoryRegions[ifunc][ic] = (char)0x00;
	
                os_  << "                     mapped the address space of function" << ifunc 
                     << " to memory address " << std::hex << (memoryRegions[ifunc]) 
                     << "\n                     reserved 0x" << std::hex << nbytes
                     << " (dec: " << std::dec << nbytes << " )" 
                     << " bytes for memory mapped operation" << std::endl;
            } 
        }
    } else {
        for ( int ifunc=0; ifunc<NUMBER_OF_VME64XFUNCTIONS; ifunc++ ) {
            (*baseAddressPtr)[ifunc] = vmeBaseaddresses[ifunc];
        }
    }

    *deviceIdentifierPtr = new HAL::VMEDummyDeviceIdentifier( deviceNumberCounter,
							      memoryRegions,
 							      baseAddresses,
							      mapSizes,
							      doSwapping );
    deviceNumberCounter++;
}

void
HAL::VMEDummyBusAdapter::closeDevice( DeviceIdentifier* vmeDevice ) 
    throw() {
    os_  << "VMEDummyBusAdapter : closing Device  \n" 
         << "                     " << vmeDevice->printString() << std::endl;
    delete( vmeDevice );
}

void
HAL::VMEDummyBusAdapter::read( DeviceIdentifier* vmeDevice, 
                          uint32_t address,
                          uint32_t addressModifier,
                          uint32_t dataWidth,
                          uint32_t *resultPtr ) 
    throw() {

    *resultPtr = 0;
    if ( memoryMode == MEMORY_MAP_ON ) {

        char *memoryAddress = dynamic_cast<HAL::VMEDummyDeviceIdentifier*>(vmeDevice)->remap( address );
        
	memcpy( resultPtr, memoryAddress, dataWidth);
        
        bool doSwapping = dynamic_cast<HAL::VMEDummyDeviceIdentifier*>(vmeDevice)->doSwapping();
        
        if ( dataWidth > 1 && doSwapping ) {
            *resultPtr = doSwap( *resultPtr, dataWidth );
        }
        
    }

    if ( verbose == VERBOSE_ON ) {
        os_  << std::endl;
        os_  << "VMEDummyBusAdapter : read from Device number " << vmeDevice->printString() << std::endl;
        os_  << "                     address : " << std::hex << std::setw(8) << std::setfill('0') << address << std::endl;
        os_  << "                          AM :       " << std::hex << std::setw(2) << std::setfill('0') << addressModifier << std::endl;
        os_  << "                   dataWidth : " << std::dec << std::setw(8) << std::setfill(' ') << dataWidth << std::endl;
        os_  << "                   returning : 0x" << std::hex << std::setw(8) << std::setfill('0') << *resultPtr 
             << " (dec) " << std::dec << *resultPtr << std::endl;
        os_  << std::endl;
    }
}

void
HAL::VMEDummyBusAdapter::write( DeviceIdentifier* vmeDevice, 
                           uint32_t address, 
                           uint32_t addressModifier,
                           uint32_t dataWidth,
                           uint32_t data) 
    throw() {

    bool doSwapping = dynamic_cast<HAL::VMEDummyDeviceIdentifier*>(vmeDevice)->doSwapping();

    if ( dataWidth > 1 && doSwapping ) {
        data = doSwap( data, dataWidth );
    }

    if ( memoryMode == MEMORY_MAP_ON ) {

        char *memoryAddress = dynamic_cast<HAL::VMEDummyDeviceIdentifier*>(vmeDevice)->remap( address );
        memcpy( memoryAddress, &data, dataWidth);

    }

    if ( verbose == VERBOSE_ON ) {
        os_  << std::endl;
        os_  << "VMEDummyBusAdapter : write to Device number " << vmeDevice->printString() << std::endl;
        os_  << "                     address : " << std::hex << std::setw(8) << std::setfill('0') << address << std::endl;
        os_  << "                          AM :       " << std::hex << std::setw(2) << std::setfill(' ') << addressModifier << std::endl;
        os_  << "                   dataWidth : " << std::dec << std::setw(8) << std::setfill(' ') << dataWidth << std::endl;
        os_  << "                   dataValue : 0x" << std::hex << std::setw(8) << std::setfill('0') << data 
             << " (dec) " << std::dec << data << std::endl;
        os_  << std::endl;
    }
}

uint32_t HAL::VMEDummyBusAdapter::doSwap( uint32_t data, uint32_t dataWidth ) const {
    uint32_t result = 0;
    if ( dataWidth == 2 ) {
        result = ((data & 0xff) << 8) +
            ((data & 0xff00) >> 8);

    } else if ( dataWidth == 4 ) {
        result = ((data & 0xff) << 24) +
            ((data & 0xff00) << 8 ) +
            ((data & 0xff0000) >> 8) +
            ((data & 0xff000000) >> 24);
    } else {
        os_ << "HAL Software bug: dataWidth " << std::dec << dataWidth << "is illegal!" << std::endl;
    }  
    return result;
}

void
HAL::VMEDummyBusAdapter::resetBus( ) 
    throw() {
    if ( verbose == VERBOSE_ON ) {
        os_  << std::endl;
        os_  << "VMEDummyBusAdapter : resetting bus" << std::endl;
        os_  << std::endl;
    }
}

void
HAL::VMEDummyBusAdapter::readBlock( DeviceIdentifier *vmeDevice,
                               uint32_t startAddress,
                               uint32_t length,      // in bytes
                               uint32_t addressModifier,
                               uint32_t dataWidth,
                               char *buffer,
                               HalAddressIncrement addressBehaviour ) 
    throw() {

    if ( memoryMode == MEMORY_MAP_ON ) {
        char *memoryAddress = dynamic_cast<HAL::VMEDummyDeviceIdentifier*>(vmeDevice)->remap( startAddress );
        if ( addressBehaviour == HAL_DO_INCREMENT ) {
            memcpy( buffer, memoryAddress, length);
        } else {
            os_  << "HAL::VMEDummyBusAdapter::readBlock : No Fifo functionality in VMEDummyBusAdapter. Doing nothing!" << std::endl;
        }

        // check if data needs to be swapped and swap it
        bool doSwapping = dynamic_cast<HAL::VMEDummyDeviceIdentifier*>(vmeDevice)->doSwapping();
        
        if ( dataWidth > 1 && doSwapping ) {
            if ( dataWidth == 2 ) {
                for ( U16* ptr = (U16*)buffer; (uint8_t*) ptr < (uint8_t*) buffer + length; ptr++ ) {
                    *ptr = doSwap( *ptr, dataWidth );
                }
            } else if ( dataWidth == 4 ) {
                for ( U32* ptr = (U32*)buffer; (uint8_t*) ptr < (uint8_t*) buffer + length; ptr++ ) {
                    *ptr = doSwap( *ptr, dataWidth );
                }
            } else {
                os_ << "HAL Software bug: dataWidth " << std::dec << dataWidth << "is illegal!" << std::endl;
            }
        }
        
    }

 

    if( verbose == VERBOSE_ON ) {
        os_  << std::endl;
        os_  << "VMEDummyBusAdapter : readBlock from Device \n                     " << vmeDevice->printString() << std::endl;
        os_  << "                startaddress (hex) : " << std::hex << std::setw(8) << std::setfill('0') << startAddress << std::endl;
        os_  << "                      length (hex) : " << std::hex << std::setw(8) << std::setfill('0') << length << std::endl;
        os_  << "                          AM (hex) :       " << std::hex << std::setw(2) << std::setfill('0') << addressModifier << std::endl;
        os_  << "                         dataWidth : " << std::dec << std::setw(8) << std::setfill(' ') << dataWidth << std::endl;
        os_  << "                  addressBehaviour : ";
        if ( addressBehaviour == HAL_DO_INCREMENT ) {
            os_  << "HAL_DO_INCREMENT" << std::endl;
        } else if ( addressBehaviour == HAL_NO_INCREMENT ) {
            os_  << "HAL_NO_INCREMENT" << std::endl;
        } else { // should never happen
            os_  << "XXXXXXXXXXXXXXXX" << std::endl;
        }
        os_  << "       pointer to readbuffer (hex) : " << std::hex << std::setw(8) << std::setfill('0') << buffer << std::endl;
        os_  << std::endl;
    }
}
  
void
HAL::VMEDummyBusAdapter::writeBlock( DeviceIdentifier* vmeDevice,
                                uint32_t startAddress,
                                uint32_t length,      // in bytes
                                uint32_t addressModifier,
                                uint32_t dataWidth,
                                char *buffer,
                                HalAddressIncrement addressBehaviour ) 
    throw() {
  

    if ( memoryMode == MEMORY_MAP_ON ) {
        // check if data needs to be swapped and swap it
        bool doSwapping = dynamic_cast<HAL::VMEDummyDeviceIdentifier*>(vmeDevice)->doSwapping();
        char* buffercpy = new char[length];
        memcpy( buffercpy, buffer, length );

        if ( dataWidth > 1 && doSwapping ) {
            if ( dataWidth == 2 ) {
                for ( U16* ptr = (U16*)buffercpy; (uint8_t*) ptr < (uint8_t*) buffercpy + length; ptr++ ) {
                    *ptr = doSwap( *ptr, dataWidth );
                }
            } else if ( dataWidth == 4 ) {
                for ( U32* ptr = (U32*)buffercpy; (uint8_t*) ptr < (uint8_t*) buffercpy + length; ptr++ ) {
                    *ptr = doSwap( *ptr, dataWidth );
                }
            } else {
                os_ << "HAL Software bug: dataWidth " << std::dec << dataWidth << "is illegal!" << std::endl;
            }
        }
        
	char *memoryAddress = dynamic_cast<HAL::VMEDummyDeviceIdentifier*>(vmeDevice)->remap( startAddress );

        if ( addressBehaviour == HAL_DO_INCREMENT ) {
            memcpy( memoryAddress, buffercpy, length);
        } else {
            os_  << "HAL::VMEDummyBusAdapter::writeBlock : No Fifo functionality in VMEDummyBusAdapter. Doing nothing!" << std::endl;
        }
        
        delete buffercpy;

    }

    if ( verbose == VERBOSE_ON ) {
        os_  << std::endl;
        os_  << "VMEDummyBusAdapter : writeBlock to Device \n                     " << vmeDevice->printString() << std::endl;
        os_  << "                startaddress (hex) : " << std::hex << std::setw(8) << std::setfill('0') << startAddress << std::endl;
        os_  << "                      length (hex) : " << std::hex << std::setw(8) << std::setfill('0') << length << std::endl;
        os_  << "                          AM (hex) :       " << std::hex << std::setw(2) << std::setfill('0') << addressModifier << std::endl;
        os_  << "                         dataWidth : " << std::dec << std::setw(8) << std::setfill(' ') << dataWidth << std::endl;
        os_  << "                  addressBehaviour : ";
        if ( addressBehaviour == HAL_DO_INCREMENT ) {
            os_  << "HAL_DO_INCREMENT" << std::endl;
        } else if ( addressBehaviour == HAL_NO_INCREMENT ) {
            os_  << "HAL_NO_INCREMENT" << std::endl;
        } else { // should never happen
            os_  << "XXXXXXXXXXXXXXXX" << std::endl;
        }
        os_  << "     pointer to sourcebuffer (hex) : " << std::hex << std::setw(8) << std::setfill('0') << buffer << std::endl;
        os_  << std::endl;
    }
}
