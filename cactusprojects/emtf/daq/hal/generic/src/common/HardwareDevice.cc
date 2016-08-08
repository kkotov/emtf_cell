#include "hal/HardwareDevice.hh"
#include <unistd.h>
#include <sstream>
#include <iomanip>
#include <string.h>
#include <fstream>

HAL::HardwareDevice::HardwareDevice( HAL::AddressTableInterface & addressTable )
  :  addressTable(addressTable) {}

HAL::HardwareDevice::~HardwareDevice() {}

void HAL::HardwareDevice::unmaskedWrite( std::string item, 
					 uint32_t data,
					 HalVerifyOption verifyFlag,
					 uint32_t offset ) const
  throw (HAL::NoSuchItemException, 
         HAL::IllegalOperationException,
         HAL::BusAdapterException,
         HAL::VerifyException,
	 HAL::AddressOutOfLimitsException) {
  const HAL::GeneralHardwareAddress& address = 
    addressTable.getWritableHardwareAddress(item);
  if ( offset != 0 ) addressTable.checkAddressLimits( address, offset );
  hardwareWrite( address, data, offset );
  if ( verifyFlag ) {
    uint32_t result;
    unmaskedRead( item, &result, offset);
    verify( result, data, "unmaskedWrite" );
  }
}

void HAL::HardwareDevice::unmaskedWrite64( std::string item, 
					 uint64_t data,
					 HalVerifyOption verifyFlag,
					 uint64_t offset ) const
  throw (HAL::NoSuchItemException, 
         HAL::IllegalOperationException,
         HAL::BusAdapterException,
         HAL::VerifyException,
	 HAL::AddressOutOfLimitsException) {
  const HAL::GeneralHardwareAddress& address = 
    addressTable.getWritableHardwareAddress(item);
  if ( offset != 0 ) addressTable.checkAddressLimits( address, offset );
  hardwareWrite64( address, data, offset );
  if ( verifyFlag ) {
    uint64_t result;
    unmaskedRead64( item, &result, offset);
    verify( result, data, "unmaskedWrite64" );
  }
}

void HAL::HardwareDevice::write( std::string item, 
				 uint32_t data, 
				 HalVerifyOption verifyFlag,
				 uint32_t offset ) const
  throw (HAL::NoSuchItemException, 
         HAL::BusAdapterException,
         HAL::IllegalOperationException, 
         HAL::VerifyException,
         HAL::MaskBoundaryException,
	 HAL::AddressOutOfLimitsException ) {

  const HAL::AddressTableItem& addressItem = addressTable.checkItem( item );
  addressItem.checkWritable();
  writeAddressItem( addressItem, data, offset, verifyFlag );
}

void HAL::HardwareDevice::write64( std::string item, 
				 uint64_t data, 
				 HalVerifyOption verifyFlag,
				 uint64_t offset ) const
  throw (HAL::NoSuchItemException, 
         HAL::BusAdapterException,
         HAL::IllegalOperationException, 
         HAL::VerifyException,
         HAL::MaskBoundaryException,
	 HAL::AddressOutOfLimitsException ) {

  const HAL::AddressTableItem& addressItem = addressTable.checkItem( item );
  addressItem.checkWritable();
  writeAddressItem64( addressItem, data, offset, verifyFlag );
}



void HAL::HardwareDevice::writePulse( std::string item, 
				      uint32_t offset ) const
  throw (HAL::NoSuchItemException,
         HAL::BusAdapterException,
         HAL::IllegalOperationException,
	 HAL::AddressOutOfLimitsException) {
  const HAL::GeneralHardwareAddress& address = 
    addressTable.getWritableHardwareAddress(item);
  if ( offset != 0 ) addressTable.checkAddressLimits( address, offset );
  hardwareWrite( address, 0x0, offset );
}

void HAL::HardwareDevice::writePulse64( std::string item, 
				      uint64_t offset ) const
  throw (HAL::NoSuchItemException,
         HAL::BusAdapterException,
         HAL::IllegalOperationException,
	 HAL::AddressOutOfLimitsException) {
  const HAL::GeneralHardwareAddress& address = 
    addressTable.getWritableHardwareAddress(item);
  if ( offset != 0 ) addressTable.checkAddressLimits( address, offset );
  hardwareWrite64( address, 0x0, offset );
}


void HAL::HardwareDevice::readPulse( std::string item, 
				     uint32_t offset ) const
  throw (HAL::NoSuchItemException,
         HAL::BusAdapterException,
         HAL::IllegalOperationException,
	 HAL::AddressOutOfLimitsException) {
  uint32_t dummy;
  const HAL::GeneralHardwareAddress& address = 
    addressTable.getReadableHardwareAddress(item);
  if ( offset != 0 ) addressTable.checkAddressLimits( address, offset );
  hardwareRead( address, &dummy, offset );
}

void HAL::HardwareDevice::readPulse64( std::string item, 
				     uint64_t offset ) const
  throw (HAL::NoSuchItemException,
         HAL::BusAdapterException,
         HAL::IllegalOperationException,
	 HAL::AddressOutOfLimitsException) {
  uint64_t dummy;
  const HAL::GeneralHardwareAddress& address = 
    addressTable.getReadableHardwareAddress(item);
  if ( offset != 0 ) addressTable.checkAddressLimits( address, offset );
  hardwareRead64( address, &dummy, offset );
}


void HAL::HardwareDevice::unmaskedRead( std::string item,
					uint32_t* result, 
					uint32_t offset ) const
  throw (HAL::NoSuchItemException,
         HAL::BusAdapterException,
         HAL::IllegalOperationException,
	 HAL::AddressOutOfLimitsException) {
  const HAL::GeneralHardwareAddress& address = addressTable.getReadableHardwareAddress( item );
  if ( offset != 0 ) addressTable.checkAddressLimits( address, offset );
  hardwareRead( address, result, offset );
}

void HAL::HardwareDevice::unmaskedRead64( std::string item,
					uint64_t* result, 
					uint64_t offset ) const
  throw (HAL::NoSuchItemException,
         HAL::BusAdapterException,
         HAL::IllegalOperationException,
	 HAL::AddressOutOfLimitsException) {
  const HAL::GeneralHardwareAddress& address = addressTable.getReadableHardwareAddress( item );
  if ( offset != 0 ) addressTable.checkAddressLimits( address, offset );
  hardwareRead64( address, result, offset );
}


void HAL::HardwareDevice::read( std::string item, 
				uint32_t* result,
				uint32_t offset ) const
  throw (HAL::NoSuchItemException,
         HAL::BusAdapterException,
         HAL::IllegalOperationException,
         HAL::MaskBoundaryException,
         HAL::AddressOutOfLimitsException) {
  uint32_t data;

  const HAL::AddressTableItem& addressItem = addressTable.checkItem( item );
  addressItem.checkReadable();
  const HAL::GeneralHardwareAddress& address = addressItem.getGeneralHardwareAddress();
  if ( offset != 0 ) addressTable.checkAddressLimits( address, offset );
  hardwareRead( address, &data, offset );
  data &= addressItem.getMask();
  *result = addressItem.applyFromMask( data );
}

void HAL::HardwareDevice::read64( std::string item, 
				uint64_t* result,
				uint64_t offset ) const
  throw (HAL::NoSuchItemException,
         HAL::BusAdapterException,
         HAL::IllegalOperationException,
         HAL::MaskBoundaryException,
         HAL::AddressOutOfLimitsException) {
  uint64_t data;

  const HAL::AddressTableItem& addressItem = addressTable.checkItem( item );
  addressItem.checkReadable();
  const HAL::GeneralHardwareAddress& address = addressItem.getGeneralHardwareAddress();
  if ( offset != 0 ) addressTable.checkAddressLimits( address, offset );
  hardwareRead64( address, &data, offset );
  data &= addressItem.getMask();
  *result = addressItem.applyFromMask( data );
}


void HAL::HardwareDevice::pollItem( std::string item,
				    uint32_t referenceValue,
				    uint32_t timeout,
				    uint32_t *result,
				    HalPollMethod pollMethod,
				    uint32_t offset ) const
    throw ( HAL::NoSuchItemException,
            HAL::IllegalOperationException,
            HAL::BusAdapterException,
            HAL::MaskBoundaryException,
            HAL::TimeoutException,
	    HAL::AddressOutOfLimitsException) {
  HAL::StopWatch watch(0);
  const HAL::AddressTableItem& addressItem = addressTable.checkItem( item );
  addressItem.checkReadable();
  const HAL::GeneralHardwareAddress& address = addressItem.getGeneralHardwareAddress();
  if ( offset != 0 ) addressTable.checkAddressLimits( address, offset );
  uint32_t mask = addressItem.getMask();
  uint32_t compareValue = addressItem.applyToMask( referenceValue );
  uint32_t data;
  bool pollCondition = false;
  uint32_t loopCounter = 0;
  watch.start();
  uint32_t time;

  // here the poll loop starts
  while ( ! pollCondition ) {
    hardwareRead( address, &data, offset);
    if ( (pollMethod == HAL_POLL_UNTIL_EQUAL) ) 
      pollCondition = (compareValue == (data & mask));
    else 
      pollCondition = (compareValue != (data & mask));
    loopCounter++;
    if (loopCounter%1000 == 0) {
      watch.stop();
      time = watch.read();
      if ( time/1000 >= timeout ) {
        std::stringstream text;
        // the poll value is also interesting in case of a timeout
        *result = addressItem.applyFromMask( mask & data );
        text << "Timeout during polling the item \"" 
             << item << "\"\n"
             << "      " << std::dec << time/1000 
             << "ms have passed wheras the timeout has been set to "
             << timeout << "ms\n"
             << "      " << std::dec << loopCounter << " polls have been carried out\n" 
             << std::ends; 
        throw( HAL::TimeoutException( text.str(), __FILE__, __LINE__, __FUNCTION__  ) );
      }      
    }
    //::usleep( 100000 ); // limit the number of accesses
  }
  *result = addressItem.applyFromMask( mask & data );
}

void HAL::HardwareDevice::pollItem64( std::string item,
				    uint64_t referenceValue,
				    uint64_t timeout,
				    uint64_t *result,
				    HalPollMethod pollMethod,
				    uint64_t offset ) const
    throw ( HAL::NoSuchItemException,
            HAL::IllegalOperationException,
            HAL::BusAdapterException,
            HAL::MaskBoundaryException,
            HAL::TimeoutException,
	    HAL::AddressOutOfLimitsException) {
  HAL::StopWatch watch(0);
  const HAL::AddressTableItem& addressItem = addressTable.checkItem( item );
  addressItem.checkReadable();
  const HAL::GeneralHardwareAddress& address = addressItem.getGeneralHardwareAddress();
  if ( offset != 0 ) addressTable.checkAddressLimits( address, offset );
  uint64_t mask = addressItem.getMask();
  uint64_t compareValue = addressItem.applyToMask( referenceValue );
  uint64_t data;
  bool pollCondition = false;
  uint64_t loopCounter = 0;
  watch.start();
  uint64_t time;

  // here the poll loop starts
  while ( ! pollCondition ) {
    hardwareRead64( address, &data, offset);
    if ( (pollMethod == HAL_POLL_UNTIL_EQUAL) ) 
      pollCondition = (compareValue == (data & mask));
    else 
      pollCondition = (compareValue != (data & mask));
    loopCounter++;
    if (loopCounter%1000 == 0) {
      watch.stop();
      time = watch.read();
      if ( time/1000 >= timeout ) {
        std::stringstream text;
        // the poll value is also interesting in case of a timeout
        *result = addressItem.applyFromMask( mask & data );
        text << "Timeout during polling the item \"" 
             << item << "\"\n"
             << "      " << std::dec << time/1000 
             << "ms have passed wheras the timeout has been set to "
             << timeout << "ms\n"
             << "      " << std::dec << loopCounter << " polls have been carried out\n" 
             << std::ends; 
        throw( HAL::TimeoutException( text.str(), __FILE__, __LINE__, __FUNCTION__  ) );
      }      
    }
    //::usleep( 100000 ); // limit the number of accesses
  }
  *result = addressItem.applyFromMask( mask & data );
}


void HAL::HardwareDevice::setBit( std::string item, 
				  HalVerifyOption verifyFlag,
				  uint32_t offset ) const
  throw (HAL::NoSuchItemException, 
         HAL::IllegalOperationException, 
         HAL::BusAdapterException,
         HAL::VerifyException,
	 HAL::AddressOutOfLimitsException ) {
  const HAL::AddressTableItem& addressItem = addressTable.checkItem( item );
  addressItem.checkWritable();
  addressItem.checkBit();
  writeAddressItem( addressItem, 0x01, offset, verifyFlag );
}

void HAL::HardwareDevice::setBit64( std::string item, 
				  HalVerifyOption verifyFlag,
				  uint64_t offset ) const
  throw (HAL::NoSuchItemException, 
         HAL::IllegalOperationException, 
         HAL::BusAdapterException,
         HAL::VerifyException,
	 HAL::AddressOutOfLimitsException ) {
  const HAL::AddressTableItem& addressItem = addressTable.checkItem( item );
  addressItem.checkWritable();
  addressItem.checkBit();
  writeAddressItem64( addressItem, 0x01, offset, verifyFlag );
}


void HAL::HardwareDevice::resetBit( std::string item, 
				    HalVerifyOption verifyFlag,
				    uint32_t offset ) const
  throw (HAL::NoSuchItemException, 
         HAL::IllegalOperationException, 
         HAL::BusAdapterException,
         HAL::VerifyException,
	 HAL::AddressOutOfLimitsException ) {
  const HAL::AddressTableItem& addressItem = addressTable.checkItem( item );
  addressItem.checkWritable();
  addressItem.checkBit();
  writeAddressItem( addressItem, 0x00, offset, verifyFlag );
}

void HAL::HardwareDevice::resetBit64( std::string item, 
				    HalVerifyOption verifyFlag,
				    uint64_t offset ) const
  throw (HAL::NoSuchItemException, 
         HAL::IllegalOperationException, 
         HAL::BusAdapterException,
         HAL::VerifyException,
	 HAL::AddressOutOfLimitsException ) {
  const HAL::AddressTableItem& addressItem = addressTable.checkItem( item );
  addressItem.checkWritable();
  addressItem.checkBit();
  writeAddressItem64( addressItem, 0x00, offset, verifyFlag );
}


bool HAL::HardwareDevice::isSet( std::string item, 
				 uint32_t offset ) const  
  throw (HAL::NoSuchItemException, 
         HAL::BusAdapterException,
         HAL::IllegalOperationException,
	 HAL::AddressOutOfLimitsException) {

  uint32_t data;

  const HAL::AddressTableItem& addressItem = addressTable.checkItem( item );
  addressItem.checkReadable();
  addressItem.checkBit();
  const HAL::GeneralHardwareAddress& address = addressItem.getGeneralHardwareAddress();
  if ( offset != 0 ) addressTable.checkAddressLimits( address, offset );
  hardwareRead( address, &data, offset );
  data &= addressItem.getMask();
  return( (bool)data );
}

bool HAL::HardwareDevice::isSet64( std::string item, 
				 uint64_t offset ) const  
  throw (HAL::NoSuchItemException, 
         HAL::BusAdapterException,
         HAL::IllegalOperationException,
	 HAL::AddressOutOfLimitsException) {

  uint64_t data;

  const HAL::AddressTableItem& addressItem = addressTable.checkItem( item );
  addressItem.checkReadable();
  addressItem.checkBit();
  const HAL::GeneralHardwareAddress& address = addressItem.getGeneralHardwareAddress();
  if ( offset != 0 ) addressTable.checkAddressLimits( address, offset );
  hardwareRead64( address, &data, offset );
  data &= addressItem.getMask();
  return( (bool)data );
}
 
bool HAL::HardwareDevice::check( std::string item,
				 uint32_t expectation,
				 std::string faultMessage,
				 uint32_t offset,
				 std::ostream& os ) const
  throw( HAL::NoSuchItemException,
	 HAL::IllegalOperationException,
	 HAL::BusAdapterException,
	 HAL::AddressOutOfLimitsException) {
  uint32_t result;
  bool returnVal;
  returnVal = true;
  read( item, &result, offset );
  if ( expectation != result ) {
    // we found a mismatch: prepare the output string:
    returnVal = false;
    std::stringstream text;
    text << "Check was unsuccessfull for item \"" << item << "\"";
    if ( offset != 0 ) text << " with offset 0x" << std::hex << std::setw(8) << std::setfill('0') << offset;
    text << "\n   read 0x" << std::hex << result << " (dec: " << std::dec << result 
         << " )   expected 0x" << std::hex << expectation << " (dec: " << std::dec << expectation 
         << " )" ;
    if ( faultMessage != "" ) {
      text << "\n   " << faultMessage << std::ends;
    } else { 
      text << std::ends;
    }
    os << text.str() << std::endl;
  }
  return returnVal;
}

bool HAL::HardwareDevice::check64( std::string item,
				 uint64_t expectation,
				 std::string faultMessage,
				 uint64_t offset,
				 std::ostream& os ) const
  throw( HAL::NoSuchItemException,
	 HAL::IllegalOperationException,
	 HAL::BusAdapterException,
	 HAL::AddressOutOfLimitsException) {
  uint64_t result;
  bool returnVal;
  returnVal = true;
  read64( item, &result, offset );
  if ( expectation != result ) {
    // we found a mismatch: prepare the output string:
    returnVal = false;
    std::stringstream text;
    text << "Check was unsuccessfull for item \"" << item << "\"";
    if ( offset != 0 ) text << " with offset 0x" << std::hex << std::setw(16) << std::setfill('0') << offset;
    text << "\n   read 0x" << std::hex << result << " (dec: " << std::dec << result 
         << " )   expected 0x" << std::hex << expectation << " (dec: " << std::dec << expectation 
         << " )" ;
    if ( faultMessage != "" ) {
      text << "\n   " << faultMessage << std::ends;
    } else { 
      text << std::ends;
    }
    os << text.str() << std::endl;
  }
  return returnVal;
}


void HAL::HardwareDevice::writeBlock( std::string startItem, 
				      uint32_t length,
				      char *buffer,
				      HalVerifyOption verifyFlag,
				      HalAddressIncrement addressBehaviour,
				      uint32_t offset ) const
  throw ( HAL::NoSuchItemException,
          HAL::IllegalValueException,
          HAL::UnsupportedException, 
          HAL::VerifyException,
          HAL::BusAdapterException,
          HAL::IllegalOperationException,
	  HAL::AddressOutOfLimitsException ){
  const HAL::GeneralHardwareAddress& address = addressTable.getWritableHardwareAddress( startItem );
  // check the start address only if there is an offset given
  if ( offset != 0 ) addressTable.checkAddressLimits( address, offset );
  // always check the end of the block
  // for block-transfers the width of the item is irrelevant. Since checkAddressLimits 
  // implicitly adds an offset=item-data-width to the address (which it should do for 
  // single read/write accesses) this data width must be substracted here. (Block transfers
  // are special in the sense that the start item only gives the startaddress and the 
  // width of this item is irrelevant for the transfer)
  if ( addressBehaviour == HAL_DO_INCREMENT ) {
    addressTable.checkAddressLimits( address, offset+length-address.getDataWidth() );
  }
  hardwareWriteBlock( address,
                      length,
                      buffer,
                      addressBehaviour,
                      offset);
  if ( verifyFlag ) {
    char* verifyBuffer = new char[length];
    readBlock( startItem, length, verifyBuffer, addressBehaviour, offset ); 
    if ( memcmp(buffer, verifyBuffer, length ) != 0) {
        //dumpVerification( buffer, verifyBuffer, length );
        delete (verifyBuffer);
        std::stringstream text;
        text << "Verify error in writeBlock "
             << "\n    (HAL::HardwareDevice::writeBlock)" << std::ends;
        throw( HAL::VerifyException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
    }
    delete( verifyBuffer );
  }
}

void HAL::HardwareDevice::writeBlock64( std::string startItem, 
				      uint64_t length,
				      char *buffer,
				      HalVerifyOption verifyFlag,
				      HalAddressIncrement addressBehaviour,
				      uint64_t offset ) const
  throw ( HAL::NoSuchItemException,
          HAL::IllegalValueException,
          HAL::UnsupportedException, 
          HAL::VerifyException,
          HAL::BusAdapterException,
          HAL::IllegalOperationException,
	  HAL::AddressOutOfLimitsException ){
  const HAL::GeneralHardwareAddress& address = addressTable.getWritableHardwareAddress( startItem );
  // check the start address only if there is an offset given
  if ( offset != 0 ) addressTable.checkAddressLimits( address, offset );
  // always check the end of the block
  // for block-transfers the width of the item is irrelevant. Since checkAddressLimits 
  // implicitly adds an offset=item-data-width to the address (which it should do for 
  // single read/write accesses) this data width must be substracted here. (Block transfers
  // are special in the sense that the start item only gives the startaddress and the 
  // width of this item is irrelevant for the transfer)
  if ( addressBehaviour == HAL_DO_INCREMENT ) {
    addressTable.checkAddressLimits( address, offset+length-address.getDataWidth() );
  }
  hardwareWriteBlock64( address,
                      length,
                      buffer,
                      addressBehaviour,
                      offset);
  if ( verifyFlag ) {
    char* verifyBuffer = new char[length];
    readBlock64( startItem, length, verifyBuffer, addressBehaviour, offset ); 
    if ( memcmp(buffer, verifyBuffer, length ) != 0) {
        //dumpVerification( buffer, verifyBuffer, length );
        delete (verifyBuffer);
        std::stringstream text;
        text << "Verify error in writeBlock64 "
             << "\n    (HAL::HardwareDevice::writeBlock64)" << std::ends;
        throw( HAL::VerifyException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
    }
    delete( verifyBuffer );
  }
}


void HAL::HardwareDevice::dumpVerification( char* buffer, char* verifyBuffer, uint64_t length ) const {
    std::ofstream outfile;
    outfile.open( "/tmp/blockWriteError.txt", std::ios::trunc);
    for( uint32_t i = 0; i<length; i++ ) {
        std::string errstr = "     ";
        if ( buffer[i] != verifyBuffer[i] ) errstr = " !!! ";
        outfile << errstr << std::setfill('0') << std::setw(6) << std::hex << i << "  " 
                << std::setw(2) << std::setfill('0') << std::hex << (uint64_t)((unsigned char)(buffer[i])) << " <=> " 
                << std::setw(2) << std::setfill('0') << std::hex << (uint64_t)((unsigned char)(verifyBuffer[i])) 
                << "\n";
    }
    outfile.close();
}


void HAL::HardwareDevice::readBlock( std::string startItem, 
				     uint32_t length,
				     char *buffer,
				     HalAddressIncrement addressBehaviour,
				     uint32_t offset) const
  throw ( HAL::NoSuchItemException,
          HAL::IllegalValueException,
          HAL::UnsupportedException, 
          HAL::BusAdapterException,
          HAL::IllegalOperationException,
	  HAL::AddressOutOfLimitsException ){
  const HAL::GeneralHardwareAddress& address = addressTable.getReadableHardwareAddress( startItem );
  // check the start address only if there is an offset given
  if ( offset != 0 ) addressTable.checkAddressLimits( address, offset );
  // always check the end of the block
  // for block-transfers the width of the item is irrelevant. Since checkAddressLimits 
  // implicitly adds an offset=item-data-width to the address (which it should do for 
  // single read/write accesses) this data width must be substracted here. (Block transfers
  // are special in the sense that the start item only gives the startaddress and the 
  // width of this item is irrelevant for the transfer)
  if ( addressBehaviour == HAL_DO_INCREMENT ) {
    addressTable.checkAddressLimits( address, offset+length-address.getDataWidth() );
  }
  hardwareReadBlock( address,
                     length,
                     buffer,
                     addressBehaviour,
                     offset);
}

void HAL::HardwareDevice::readBlock64( std::string startItem, 
				     uint64_t length,
				     char *buffer,
				     HalAddressIncrement addressBehaviour,
				     uint64_t offset) const
  throw ( HAL::NoSuchItemException,
          HAL::IllegalValueException,
          HAL::UnsupportedException, 
          HAL::BusAdapterException,
          HAL::IllegalOperationException,
	  HAL::AddressOutOfLimitsException ){
  const HAL::GeneralHardwareAddress& address = addressTable.getReadableHardwareAddress( startItem );
  // check the start address only if there is an offset given
  if ( offset != 0 ) addressTable.checkAddressLimits( address, offset );
  // always check the end of the block
  // for block-transfers the width of the item is irrelevant. Since checkAddressLimits 
  // implicitly adds an offset=item-data-width to the address (which it should do for 
  // single read/write accesses) this data width must be substracted here. (Block transfers
  // are special in the sense that the start item only gives the startaddress and the 
  // width of this item is irrelevant for the transfer)
  if ( addressBehaviour == HAL_DO_INCREMENT ) {
    addressTable.checkAddressLimits( address, offset+length-address.getDataWidth() );
  }
  hardwareReadBlock64( address,
                     length,
                     buffer,
                     addressBehaviour,
                     offset);
}


const HAL::AddressTableInterface& HAL::HardwareDevice::getAddressTableInterface() const {
  return addressTable;
}

void HAL::HardwareDevice::printAddressTable() const {
  addressTable.print();
}

void HAL::HardwareDevice::verify( uint64_t readBack,
				  uint64_t data,
				  std::string methodName ) const
  throw( HAL::VerifyException ) {
  if ( readBack != data ) {
      std::stringstream text;
      text << "Verify error: wrote " << std::hex << data 
           << "          read back " << std::hex << readBack
           << "\n    (HAL::HardwareDevice::" << methodName 
           << ")" << std::ends;
      throw( HAL::VerifyException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
    }
}

void HAL::HardwareDevice::writeAddressItem( const HAL::AddressTableItem& addressItem, 
					    uint32_t data,
					    uint32_t offset,
					    HalVerifyOption verifyFlag ) const
  throw( HAL::MaskBoundaryException,
         HAL::IllegalValueException,
         HAL::BusAdapterException,
         HAL::VerifyException,
	 HAL::AddressOutOfLimitsException ) {
  uint32_t mask;
  uint32_t newData = addressItem.applyToMask( data );

  const HAL::GeneralHardwareAddress& address = addressItem.getGeneralHardwareAddress();  

  if ( offset != 0 ) addressTable.checkAddressLimits( address, offset );

  mask = addressItem.getMask();
  if ( addressItem.isReadable() ){
    uint32_t oldData; 
    hardwareRead( address, &oldData, offset );
    newData |= ( oldData & (~ mask) );
  }
  hardwareWrite( address, newData, offset );

  // verify if wanted
  if ( verifyFlag ) {
    uint32_t result;
    addressItem.checkReadable();
    hardwareRead( addressItem.getGeneralHardwareAddress(), &result, offset );
    result &= mask;
    result = addressItem.applyFromMask( result );
    verify( result, data, "writeAddressItem" );
  }
}

void HAL::HardwareDevice::writeAddressItem64( const HAL::AddressTableItem& addressItem, 
					    uint64_t data,
					    uint64_t offset,
					    HalVerifyOption verifyFlag ) const
  throw( HAL::MaskBoundaryException,
         HAL::IllegalValueException,
         HAL::BusAdapterException,
         HAL::VerifyException,
	 HAL::AddressOutOfLimitsException ) {
  uint64_t mask;
  uint64_t newData = addressItem.applyToMask( data );

  const HAL::GeneralHardwareAddress& address = addressItem.getGeneralHardwareAddress();  

  if ( offset != 0 ) addressTable.checkAddressLimits( address, offset );

  mask = addressItem.getMask();
  if ( addressItem.isReadable() ){
    uint64_t oldData; 
    hardwareRead64( address, &oldData, offset );
    newData |= ( oldData & (~ mask) );
  }
  hardwareWrite64( address, newData, offset );

  // verify if wanted
  if ( verifyFlag ) {
    uint64_t result;
    addressItem.checkReadable();
    hardwareRead64( addressItem.getGeneralHardwareAddress(), &result, offset );
    result &= mask;
    result = addressItem.applyFromMask( result );
    verify( result, data, "writeAddressItem64" );
  }
}
