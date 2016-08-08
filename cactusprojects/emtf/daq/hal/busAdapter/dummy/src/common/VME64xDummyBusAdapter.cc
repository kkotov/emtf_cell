#include "hal/VME64xDummyBusAdapter.hh"
#include <iomanip>
#include <fstream>
#include <string.h>

// dirty but ok...:
typedef unsigned short U16;
typedef uint32_t U32;

HAL::VME64xDummyBusAdapter::VME64xDummyBusAdapter( std::string configSpaceDefinitionFile,
						   enum VerboseMode verbose, 
						   enum MemoryMode  memoryMode,
						   std::ostream& os )
    throw ( IllegalValueException,
            NoSuchFileException )
    : os_(os) {
    deviceNumberCounter = 0;
    this->verbose = verbose;
    this->memoryMode = memoryMode;
    os_  << "VME64xDummyBusAdapter : constructor has been called :\n" 
         << "                     The \"verbose\" flag is set to " << (int)verbose << "\n"
         << "                     The \"memoryMode\" flag is set to " << (int)memoryMode  
         << std::endl;

    for( int i = 0; i <= ( VME64x_AMNESIA_SLOT > MAX_NUMBER_OF_SLOTS ? VME64x_AMNESIA_SLOT : MAX_NUMBER_OF_SLOTS); i++ ) {
        configSpaceVector_.push_back( (std::tr1::unordered_map< uint32_t, uint32_t >*) 0 );
    }


    const int MaxLength=1024;
    char buffer[MaxLength];
    std::ifstream FileStream( configSpaceDefinitionFile.c_str() );
    if ( FileStream.fail() ) {
        std::string text = "Cannot open file : " + configSpaceDefinitionFile + "    (HAL::VME64xDummyBusAdapter::VME64xDummyBusAdapter)";
        throw (NoSuchFileException( text, __FILE__, __LINE__, __FUNCTION__ ));
    }

    // prepare the loop over all lines of the ascii file:
    uint32_t slotId;
    std::string fileName;
    while ( ! FileStream.eof() ) {
        FileStream.getline(buffer, MaxLength);
        if ( FileStream.eof() ) break;
        //skip all white space at beginning of buffer:
        int ic = 0;
        while ( buffer[ic] == ' ' ||
                buffer[ic] == '\t' ) {
            ic++;
        }
        if (buffer[ic] == '#') continue; // a comment
        if (buffer[ic] == '\n') continue; // should never happen since \n is not extracted with getline
        if (buffer[ic] == 0x00) continue;

        std::istringstream Line(buffer);    
        Line >> std::dec >> slotId;
        Line >> fileName;

        if ( slotId >= MAX_NUMBER_OF_SLOTS && slotId != VME64x_AMNESIA_SLOT ) {
            std::stringstream text;
            text << "Illegal slot number : " << slotId
                 << "\n    (HAL::VME64xDummyBusAdapter::VME64xDummyBusAdapter)" << std::ends;
            throw( IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
        }
    
        HAL::ConfigurationSpaceFileReader reader( fileName );
        std::tr1::unordered_map<uint32_t, uint32_t>* mapPtr = reader.getDataMapPtr();
        configSpaceVector_[ slotId ] = mapPtr;
    }
    FileStream.close();
}

HAL::VME64xDummyBusAdapter::~VME64xDummyBusAdapter() {
    for( int i = 0; i < MAX_NUMBER_OF_SLOTS; i++ ) {
        if( configSpaceVector_[i] ) {
            delete configSpaceVector_[i];
        }
    }
    if ( configSpaceVector_[VME64x_AMNESIA_SLOT] ) {
        os_ << "deleting Amnesia " <<std::dec <<  VME64x_AMNESIA_SLOT<< std::endl;
        os_ << std::hex << configSpaceVector_[VME64x_AMNESIA_SLOT] << std::endl;
        delete configSpaceVector_[VME64x_AMNESIA_SLOT];
    }
}

void
HAL::VME64xDummyBusAdapter::openDevice( const VMEAddressTable& vmeAddressTable,
					uint32_t vmeBaseaddress,
					DeviceIdentifier** deviceIdentifierPtr,
					uint32_t* baseAddressPtr,
					bool doSwapping ) 
    throw() {
    std::vector<char *> memoryRegions(NUMBER_OF_VME64XFUNCTIONS);
    for ( int i=0; i<NUMBER_OF_VME64XFUNCTIONS; i++ ) memoryRegions.push_back( (char*) 0 );

    uint32_t ic;
    os_  << "VME64xDummyBusAdapter : opening Device number "<< deviceNumberCounter 
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

    // If memory mapping is on we have to do some mapping into the memory of the PC.
    // The size of pointers in the PC is in general not equal to the size of vme addresses.
    // Therefore we need to maintain this mapping in the deviceIdentifier and hidden from 
    // the rest of the HAL operations. 
    if ( memoryMode == MEMORY_MAP_ON ) {

        vmeAddressTable.getAddressBoundaries( minAddresses, maxAddresses );

        *baseAddressPtr = vmeBaseaddress;

 	uint32_t nbytes = maxAddresses[0] + 1;

        memoryRegions[0] = new char[ nbytes ];
	baseAddresses[0] = vmeBaseaddress;
	mapSizes[0]      = nbytes;

        for (ic=0; ic<=nbytes; ic++) memoryRegions[0][ic] = (char)0x00;

	
        os_  << "                     mapped the address space to memory address " 
	     << std::hex << (memoryRegions[0]) 
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
							      doSwapping);
    deviceNumberCounter++;
}

// the call for VME64x modules
void
HAL::VME64xDummyBusAdapter::openDevice( const VMEAddressTable& vmeAddressTable,
					std::vector<uint32_t> vmeBaseaddresses,
					DeviceIdentifier** deviceIdentifierPtr,
					std::vector<uint32_t>* baseAddressPtr,
					bool doSwapping ) 
    throw() {
    std::vector<char *> memoryRegions(NUMBER_OF_VME64XFUNCTIONS);
    for ( int i=0; i<NUMBER_OF_VME64XFUNCTIONS; i++ ) memoryRegions.push_back( (char*) 0 );

    uint32_t ic;
    os_  << "VME64xDummyBusAdapter : opening VME64x Device number "<< deviceNumberCounter 
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
	        
                uint32_t nbytes = maxAddresses[ifunc] + 1;
	        
	        memoryRegions[ifunc] = new char[ nbytes ];
	        baseAddresses[ifunc] = vmeBaseaddresses[ifunc];
	        mapSizes[ifunc]      = nbytes;

                for (ic=0; ic<=maxAddresses[ifunc]; ic++) memoryRegions[ifunc][ic] = (char)0x00;

                os_  << "                     mapped the address space of function" << ifunc 
                     << " to memory address " << std::hex << memoryRegions[ifunc] 
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
HAL::VME64xDummyBusAdapter::closeDevice( DeviceIdentifier* vmeDevice ) 
    throw() {
    os_  << "VME64xDummyBusAdapter : closing Device => " << vmeDevice->printString() << std::endl;
    delete( vmeDevice );
}

void HAL::VME64xDummyBusAdapter::checkConfigAccess(uint32_t address, 
						   uint32_t dataWidth, 
						   uint32_t slotId, 
						   uint32_t byte )
    throw (BusAdapterException) {
    if ( ! configSpaceVector_[slotId] ) {
        std::stringstream text;
        text << "No module in slot number " << std::dec << slotId
             << "\n    (HAL::VME64xDummyBusAdapter::read)" << std::ends;
        throw( BusAdapterException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
    }
    if ( ((( 1 << (dataWidth - 1)) - 1) & byte) != 0 ) {
        std::stringstream text;
        text << "ERROR: try to do an unaligned access:\n"
             << "    address : " << std::hex << address 
             << "  datawidth : " << std::dec << dataWidth
             << "       slot : " << std::dec << slotId
             << "\n    (HAL::VME64xDummyBusAdapter::read)" << std::ends;
        throw( BusAdapterException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
    }
  
}

void
HAL::VME64xDummyBusAdapter::read( DeviceIdentifier* vmeDevice, 
				  uint32_t address,
				  uint32_t addressModifier,
				  uint32_t dataWidth,
				  uint32_t *resultPtr ) 
    throw(BusAdapterException) {

    if ( addressModifier == 0x2f ) {
        uint32_t slotId = (address/VME64X_CONFIGURATIONSPACE_SIZE);
        uint32_t mapAddress = (~0x3)&(address % VME64X_CONFIGURATIONSPACE_SIZE);
        int byte = 0x3 & address;
        checkConfigAccess( mapAddress, dataWidth, slotId, byte); // simulate a busadapterexception if no config space in slot
        uint32_t readData;
        if ( (*configSpaceVector_[slotId]).find(mapAddress) != (*configSpaceVector_[slotId]).end() ) {
            readData = (*configSpaceVector_[slotId])[mapAddress];
        } else { 
            readData = 0;
        }
        uint32_t result = readData >> (8*byte);
        if ( dataWidth == 1 ) {
            result = result & 0xff;
        } else if( dataWidth == 2 ) {
            result = result & 0xffff;
        } else if( dataWidth == 3 ) {
            result = result & 0xffffff;
        }

        *resultPtr = result;

        // normal access

    } else {
        *resultPtr = 0;
        if ( memoryMode == MEMORY_MAP_ON ) {

            char *memoryAddress = dynamic_cast<HAL::VMEDummyDeviceIdentifier*>(vmeDevice)->remap( address );

            memcpy( resultPtr, memoryAddress, dataWidth);

            bool doSwapping = dynamic_cast<HAL::VMEDummyDeviceIdentifier*>(vmeDevice)->doSwapping();
            
            if ( dataWidth > 1 && doSwapping ) {
                *resultPtr = doSwap( *resultPtr, dataWidth );
            }

        }
    }

    if ( verbose == VERBOSE_ON ) {
        os_  << std::endl;
        os_  << "VME64xDummyBusAdapter : read from Device number " << vmeDevice->printString() << std::endl;
        os_  << "                     address : " << std::hex << std::setw(8) << std::setfill('0') << address << std::endl;
        os_  << "                          AM :       " << std::hex << std::setw(2) << std::setfill('0') << addressModifier << std::endl;
        os_  << "                   dataWidth : " << std::dec << std::setw(8) << std::setfill(' ') << dataWidth << std::endl;
        os_  << "                   returning : 0x" << std::hex << std::setw(8) << std::setfill('0') << *resultPtr 
             << " (dec) " << std::dec << *resultPtr << std::endl;
        os_  << std::endl;
    }
}

void
HAL::VME64xDummyBusAdapter::write( DeviceIdentifier* vmeDevice, 
                              uint32_t address, 
                              uint32_t addressModifier,
                              uint32_t dataWidth,
                              uint32_t data) 
    throw(BusAdapterException) {

    bool doSwapping = dynamic_cast<HAL::VMEDummyDeviceIdentifier*>(vmeDevice)->doSwapping();
    
    if ( dataWidth > 1 && doSwapping && addressModifier != 0x2f ) {
        data = doSwap( data, dataWidth );
    }
    
    if ( verbose == VERBOSE_ON ) {
        os_  << std::endl;
        os_  << "VME64xDummyBusAdapter : write to Device number " << vmeDevice->printString() << std::endl;
        os_  << "                     address : " << std::hex << std::setw(8) << std::setfill('0') << address << std::endl;
        os_  << "                          AM :       " << std::hex << std::setw(2) << std::setfill(' ') << addressModifier << std::endl;
        os_  << "                   dataWidth : " << std::dec << std::setw(8) << std::setfill(' ') << dataWidth << std::endl;
        os_  << "                   dataValue : 0x" << std::hex << std::setw(8) << std::setfill('0') << data 
             << " (dec) " << std::dec << data << std::endl;
        os_  << std::endl;
    }

    if ( addressModifier == 0x2f ) {
        uint32_t slotId = (address/VME64X_CONFIGURATIONSPACE_SIZE);
        int byte = 0x3 & address;
        uint32_t mapAddress = (~0x3)&(address % VME64X_CONFIGURATIONSPACE_SIZE);
        checkConfigAccess( mapAddress, dataWidth, slotId, byte); 
        uint32_t writeData = data << (8*byte);
    
        (*(configSpaceVector_[slotId]))[mapAddress] = writeData;
    } else if ( memoryMode == MEMORY_MAP_ON ) {

       char *memoryAddress = dynamic_cast<HAL::VMEDummyDeviceIdentifier*>(vmeDevice)->remap( address );
       memcpy( memoryAddress, &data, dataWidth);

    }
  
}

void
HAL::VME64xDummyBusAdapter::resetBus( ) 
    throw() {
    if ( verbose == VERBOSE_ON ) {
        os_  << std::endl;
        os_  << "VME64xDummyBusAdapter : resetting bus" << std::endl;
        os_  << std::endl;
    }
}

void
HAL::VME64xDummyBusAdapter::readBlock( DeviceIdentifier *vmeDevice,
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
            os_  << "HAL::VME64xDummyBusAdapter::readBlock : No Fifo functionality in VME64xDummyBusAdapter. Doing nothing!" << std::endl;
        }
        // check if data needs to be swapped and swap it
        bool doSwapping = dynamic_cast<HAL::VMEDummyDeviceIdentifier*>(vmeDevice)->doSwapping();
        
        if ( dataWidth > 1 && doSwapping ) {
            if ( dataWidth == 2 ) {
                for ( U16* ptr = (U16*)buffer;  (uint8_t*)ptr <  (uint8_t*)buffer + length; ptr++ ) {
                    *ptr = doSwap( *ptr, dataWidth );
                }
            } else if ( dataWidth == 4 ) {
                for ( U32* ptr = (U32*)buffer;  (uint8_t*)ptr <  (uint8_t*)buffer + length; ptr++ ) {
                    *ptr = doSwap( *ptr, dataWidth );
                }
            } else {
                os_ << "HAL Software bug: dataWidth " << std::dec << dataWidth << "is illegal!" << std::endl;
            }
        }

    }

    if( verbose == VERBOSE_ON ) {
        os_  << std::endl;
        os_  << "VME64xDummyBusAdapter : readBlock from Device \n                     " << vmeDevice->printString() << std::endl;
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
            os_  << "XXXXX this should never happen XXXXXXX" << std::endl;
        }
        os_  << "       pointer to readbuffer (hex) : " << std::hex << std::setw(8) << std::setfill('0') << buffer << std::endl;
        os_  << std::endl;
    }
}
  
void
HAL::VME64xDummyBusAdapter::writeBlock( DeviceIdentifier* vmeDevice,
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
            os_  << "HAL::VME64xDummyBusAdapter::writeBlock : No Fifo functionality in VME64xDummyBusAdapter. Doing nothing!" << std::endl;
        }

        delete buffercpy;

    }

    if ( verbose == VERBOSE_ON ) {
        os_  << std::endl;
        os_  << "VME64xDummyBusAdapter : writeBlock to Device \n                     " << vmeDevice->printString() << std::endl;
        os_  << "                startaddress (hex) : " << std::hex << std::setw(8) << std::setfill('0') << startAddress << std::endl;
        os_  << "                      length (hex) : " << std::hex << std::setw(8) << std::setfill('0') << length << std::endl;
        os_  << "                          AM (hex) :       " << std::hex << std::setw(2) <<std:: setfill('0') << addressModifier << std::endl;
        os_  << "                         dataWidth : " << std::dec << std::setw(8) << std::setfill(' ') << dataWidth << std::endl;
        os_  << "                  addressBehaviour : ";
        if ( addressBehaviour == HAL_DO_INCREMENT ) {
            os_  << "HAL_DO_INCREMENT" << std::endl;
        } else if ( addressBehaviour == HAL_NO_INCREMENT ) {
            os_  << "HAL_NO_INCREMENT" << std::endl;
        } else { // should never happen
            os_  << "XXXXXX this should never happen XXXXXX" << std::endl;
        }
        os_  << "     pointer to sourcebuffer (hex) : " << std::hex << std::setw(8) << std::setfill('0') << buffer << std::endl;
        os_  << std::endl;
    }
}

uint32_t HAL::VME64xDummyBusAdapter::doSwap( uint32_t data, uint32_t dataWidth ) const {
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
