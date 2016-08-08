#include "hal/VMEConfigurationSpaceHandler.hh"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdlib.h>

HAL::VMEConfigurationSpaceHandler::VMEConfigurationSpaceHandler( HAL::VMEBusAdapterInterface& busAdapter ) 
  : busAdapter_(busAdapter) {
  
  HAL::VMEConfigurationSpaceAddressReader configurationReader;
  configurationAddressTablePtr_ = new HAL::VMEAddressTable("VME64x Configuration Space", 
							   configurationReader);
  vmeDevicePtr_ = new HAL::VMEConfigurationSpaceDevice( *configurationAddressTablePtr_,
							busAdapter );
}

HAL::VMEConfigurationSpaceHandler::~VMEConfigurationSpaceHandler() {
  delete vmeDevicePtr_;
  delete configurationAddressTablePtr_;
}

uint32_t HAL::VMEConfigurationSpaceHandler::calculateOffset( uint32_t slot ) const {
  return ( slot * VME64X_CONFIGURATIONSPACE_SIZE );
}

void HAL::VMEConfigurationSpaceHandler::configWrite( std::string item,
						     uint32_t slot,
						     uint32_t data,
						     HalVerifyOption verifyFlag,
						     uint32_t offset ) const
  throw( HAL::NoSuchItemException, 
         HAL::IllegalOperationException,
         HAL::BusAdapterException ) {
  offset += calculateOffset( slot );
  vmeDevicePtr_->write( item, data, verifyFlag, offset );
}

void HAL::VMEConfigurationSpaceHandler::saveWrite( std::string item,
						   uint32_t slot,
						   uint32_t data,
						   HalVerifyOption verifyFlag,
						   uint32_t offset ) const {
  try {
    configWrite( item, slot, data, verifyFlag, offset );
  } catch ( HAL::BusAdapterException& e ) {
    std::cout << "Internal error while accessing the configuration space via the busAdapter\n    (HAL::VMEConfigurationSpaceHandler::saveWrite)" << std::endl;
    std::cout << e.what() << std::endl;
    exit (-1);
  } catch( HAL::HardwareAccessException& e ) {
    std::cout << "Internal program error in HAL::VMEConfigurationSpaceHandler::saveWrite: "
         << "\ncaught exception:\n"
         << e.what() << std::endl;
    exit(-1);
  }
}

void HAL::VMEConfigurationSpaceHandler::configRead( std::string item,
						    uint32_t slot,
						    uint32_t* resultPtr,
						    uint32_t offset ) const
  throw( HAL::NoSuchItemException, 
         HAL::IllegalOperationException,
         HAL::BusAdapterException ) {
  offset += calculateOffset( slot );
  vmeDevicePtr_->read( item, resultPtr, offset );
}

void HAL::VMEConfigurationSpaceHandler::saveRead(std::string item,
						 uint32_t slot,
						 uint32_t* resultPtr,
						 uint32_t offset ) const {
  try {
    configRead( item, slot, resultPtr, offset );
  } catch ( HAL::BusAdapterException& e ) {
    std::cout << "Internal Error while accessing the configuration space via the busAdapter\n    (HAL::VMEConfigurationSpaceHandler::saveRead)" << std::endl;
    std::cout << e.what() << std::endl;
    exit ( -1 );
  } catch( HAL::HardwareAccessException& e ) {
    std::cout << "Internal program error in HAL::VMEConfigurationSpaceHandler::saveRead: "
         << "\ncaught exception:\n"
         << e.what() << std::endl;
    exit(-1);
  }
}

bool 
HAL::VMEConfigurationSpaceHandler::containsVME64xModule( uint32_t slot ) const 
        throw (HAL::IllegalValueException) {
  uint32_t CRIdentifier, CRspecificationId;
  bool result;
  try{
    configRead( "CR", slot, &CRIdentifier ); 
    configRead( "specificationId", slot, &CRspecificationId ); 
  } catch ( HAL::BusAdapterException& e ) {
    CRIdentifier = 0;
  } catch( HAL::HardwareAccessException& e ) {
    std::cout << "Internal program error in HAL::VMEConfigurationSpaceHandler::containsVME64xModule "
         << "\ncaught exception:\n"
         << e.what() << std::endl;
    exit(-1);
  }
  if ( (CRIdentifier == 0x4352) & (CRspecificationId==0x02) ) {
    result = true;
  } else {
    result = false;
  }
  return result;
}

bool 
HAL::VMEConfigurationSpaceHandler::functionIsImplemented( uint32_t slotId, 
							  uint32_t functionId ) const {

  std::ostringstream item;
  uint32_t ADEM;

  // retrieve raw function data from config space: The ADER is also read
  // in case one day we want to support fixed address functions (FAFs).
  //  item << "ADEM-F" << setw(1)<< setfill('0') << functionId << ends;
  //  item << "ADEM-F" << dec << setw(1) << functionId << ends;
  item << "ADEM-F" << std::setw(1) << functionId;
  try {
    configRead(item.str(), slotId, &ADEM);
  } catch ( HAL::BusAdapterException& e ) {
    return false;
  } catch ( HAL::HardwareAccessException& e ) {
    std::cout << "Internal program error (HAL::VMEConfigurationSpaceHandler::functionIsImplemented)" << std::endl;
    std::cout << e.what() << std::endl;
    exit(-1);
  }
  // check if this function is implemented at all:
  if ( ADEM == 0x00 ) {
    return false;
  }
  return true;
}

void HAL::VMEConfigurationSpaceHandler::checkVME64xConfigSpace( uint32_t slot ) const
  throw( HAL::IllegalOperationException ) {
  if ( ! containsVME64xModule( slot ) ) {
	std::stringstream text;
	text << "There is no VME64x Module in slot" << std::dec << slot
             << "\n    (HAL::VMEConfigurationSpaceHandler::checkVME64xConfigSpace)"
             << std::ends;
	throw( HAL::IllegalOperationException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
}

uint32_t HAL::VMEConfigurationSpaceHandler::getUserROMInterval( uint32_t slot ) const {
  uint32_t byteInterval, romWidth;
  saveRead( "CRWidth", slot, &romWidth );
  if ( romWidth == 0x81 ) {
	byteInterval = 4;
  } else if( romWidth == 0x82 ) {
	byteInterval = 2;
  } else {
	byteInterval = 1;
  }
  return byteInterval;
};

void HAL::VMEConfigurationSpaceHandler::readROMBlock( uint32_t slot, 
						      uint32_t startAdr, 
						      uint32_t endAdr, 
						      std::vector<unsigned char>* data ) const {
  // The configuration region which is valid for readout is given by the
  // field "Configuration ROM data access width". The value of this field
  // determines which bytes are considered during the checksum building.
  // The region above 0x1000 is the User region. Which bytes are used is 
  // specified in the field "Configuration ROM data access width".
  //
  // In the specified part of the ROM only every 4th byte is valid and has 
  // to be taken into account when building the checksum. But modules must
  // deliver 00 on bytes which are not defined. Therefore the checksum does
  // not change if these bytes are also taken in consideration while building
  // the checksum.
  uint32_t ic, offset, readData;
  uint32_t userROMInterval = getUserROMInterval( slot );

  // it must be considered that the area up to 0xfff is only every 4th byte.
  for ( ic=0, offset = startAdr; offset <= endAdr; ic++, offset += userROMInterval ) {
    saveRead( "romStart", slot, &readData, offset );
	data->push_back((unsigned char)readData);
  }
}


bool HAL::VMEConfigurationSpaceHandler::checksumOk( uint32_t slot ) const 
  throw( HAL::IllegalOperationException ) {
  checkVME64xConfigSpace( slot );
  uint32_t romLength;
  saveRead( "romLength", slot, &romLength );
  uint32_t userROMInterval = getUserROMInterval( slot );
  std::vector<unsigned char> CRRom;
  uint32_t endAdr = userROMInterval * (romLength - 1) + 7;
  readROMBlock( slot, 0x07, endAdr, &CRRom );
  // build checksum
  uint32_t checksum = 0;
  for ( uint32_t ic=0; ic<romLength; ic++ ) {
    checksum += (uint32_t)CRRom[ic];
  }

  // now the spec is not clear...what the hell should be an
  // eight bit 2s complement binary checksum ???? Probably they
  // want a 2s complement of the checksum in order to be able 
  // to add it to the checksum and test for 0.
  checksum &= 0x00ff;
  uint32_t romChecksum;
  saveRead( "checksum", slot, &romChecksum ); 
  return ( (0xff & (checksum + romChecksum)) == 0);
}
 
std::string HAL::VMEConfigurationSpaceHandler::readSerialNumber( uint32_t slot ) const
  throw( HAL::IllegalOperationException ) {

  checkVME64xConfigSpace( slot );
  uint32_t size, startAdr, endAdr, userROMInterval;
  saveRead( "serialNumberStart", slot, &startAdr );
  saveRead( "serialNumberEnd", slot, &endAdr );
  userROMInterval = getUserROMInterval( slot );
  size = (endAdr - startAdr + userROMInterval) / userROMInterval;
  std::vector<unsigned char> serialChar;
  readROMBlock( slot, startAdr, endAdr, &serialChar );
  size = serialChar.size();
  char chararr[ size + 1 ];
  for ( uint32_t ic = 0; ic < size; ic++ ) {
    chararr[ic] = serialChar[ic];
  }
  chararr [ size ] = (char)0x00;
  std::string result( (char*)chararr );
  return result;
}

void HAL::VMEConfigurationSpaceHandler::enableVME64xModule( uint32_t slotId ) const 
  throw( HAL::IllegalOperationException ) {
  checkVME64xConfigSpace( slotId );
  saveWrite( "bitSet", slotId, 0x10 );
}

void HAL::VMEConfigurationSpaceHandler::disableVME64xModule( uint32_t slotId ) const 
  throw( HAL::IllegalOperationException ) {
  checkVME64xConfigSpace( slotId );
  saveWrite( "bitClear", slotId, 0x10 );
}

bool HAL::VMEConfigurationSpaceHandler::enabled( uint32_t slotId ) const 
  throw( HAL::IllegalOperationException ) {
  checkVME64xConfigSpace( slotId );
  uint32_t result;
  saveRead( "bitSet", slotId, &result);
  return ( (result & 0x10) == 0x10 ); 
}

HAL::VMEBusAdapterInterface& HAL::VMEConfigurationSpaceHandler::getBusAdapter() const {
  return busAdapter_;
}
