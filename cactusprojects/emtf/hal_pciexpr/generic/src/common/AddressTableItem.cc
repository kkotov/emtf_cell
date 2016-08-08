#include "hal/AddressTableItem.hh"
#include <sstream>
#include <iomanip>

HAL::AddressTableItem::AddressTableItem( std::string key,
                                    std::string description,
                                    HAL::GeneralHardwareAddress& hardwareAddress,
                                    uint32_t mask,
                                    bool writableFlag,
                                    bool readableFlag) 
  : hardwareAddress(hardwareAddress),
    key(key),
    description(description),
    mask(mask),
    writableFlag(writableFlag),
    readableFlag(readableFlag) {

  if ( countBits(mask) == 1 ) {
    bitFlag = true; 
  } else {
    bitFlag = false;
  }     
}

HAL::AddressTableItem::AddressTableItem( const AddressTableItem& original )
  :hardwareAddress( *(original.hardwareAddress.clone()) ) {
  key = original.key;
  description = original.description;
  mask = original.mask;
  writableFlag = original.writableFlag;
  readableFlag = original.readableFlag;
  bitFlag = original.bitFlag;
}

HAL::AddressTableItem::~AddressTableItem() {
    delete &hardwareAddress;   // it is on the heap !
}

HAL::GeneralHardwareAddress&
HAL::AddressTableItem::getGeneralHardwareAddress() const {
  return hardwareAddress;
}

uint32_t HAL::AddressTableItem::getMask() const {
  return mask;
}

std::string HAL::AddressTableItem::getKey() const {
  return key;
}

std::string HAL::AddressTableItem::getDescription() const {
  return description;
}

bool HAL::AddressTableItem::isReadable() const {
  return readableFlag;
}

bool HAL::AddressTableItem::isWritable() const {
  return writableFlag;
}

void HAL::AddressTableItem::checkReadable() const
  throw (HAL::IllegalOperationException) {
  if ( ! readableFlag ) {
    std::stringstream text;
    text << "Try to read from non readable item!"
         << "\n    Item : " << getKey()
         << "\n    (HAL::AddressTableItem::checkReadable)" << std::ends;
    std::string textStr = text.str();
    throw (HAL::IllegalOperationException( textStr, __FILE__, __LINE__, __FUNCTION__ ));
  }
}

void HAL::AddressTableItem::checkWritable() const
  throw (HAL::IllegalOperationException) {
  if ( ! writableFlag ) {
    std::stringstream text;
    text << "Try to write to non writable item!"
         << "\n    Item : " << getKey()
         << "\n    (HAL::AddressTableItem::checkWritable)" << std::ends;
    std::string textStr = text.str();
    throw (HAL::IllegalOperationException( textStr, __FILE__, __LINE__, __FUNCTION__ ));
  }
}

void HAL::AddressTableItem::checkBit() const 
  throw (HAL::IllegalOperationException) {
  if ( ! bitFlag ) {
    std::stringstream text;
    text << "Try to apply bit-operation to non-bit item!"
         << "\n    Item : " << getKey()
         << "\n    (HAL::AddressTableItem::checkBit)" << std::ends;
    std::string textStr = text.str();
    throw (HAL::IllegalOperationException( textStr, __FILE__, __LINE__, __FUNCTION__ ));
  }
}

bool HAL::AddressTableItem::isBit() const {
  return bitFlag;
}

void HAL::AddressTableItem::print( std::ostream& os ) const {
  os << std::setfill(' ') << std::setw(30) << key.c_str() << "  ";
  hardwareAddress.print();
  os << std::setw(8) << std::hex << std::setfill('0') << std::setw(8) << mask << "  " << std::setfill(' ')
       << std::setw(1) << readableFlag << "  "
       << std::setw(1) << writableFlag << "  "
       << description
       << std::endl;
}

uint32_t HAL::AddressTableItem::countBits( uint32_t word ) const {
  uint32_t result = 0;
  for( int ic=0; ic<32; ic++ ) {
    if ( (word & 0x00000001) == 1 ) result++;
    word >>= 1;
  }
  return result;
}


uint32_t HAL::AddressTableItem::applyToMask( uint32_t data,
						  bool MaskBoundaryCheck ) const 
  throw( HAL::MaskBoundaryException ) {
  if ( mask == 0 ) {
    data = 0;
  } else {
    bool maskOk;
    if ( MaskBoundaryCheck ) {
      uint32_t help = mask;
      while ( (help & 0x00000001) == 0 ) help >>= 1;
      maskOk = ( (~ help & data) == 0 );
      if ( ! maskOk ) {
        std::stringstream text;
        text << "Data bits set outside of mask.\n"
			 << "  Item : " << key << "\n" 
			 << "  Data : " << std::hex << data << "\n"  
			 << "  Mask : " << mask
			 << "      (HAL::AddressTableItem::applyToMask)" 
			 << std::ends;
        std::string textStr = text.str();
        throw( HAL::MaskBoundaryException( textStr, __FILE__, __LINE__, __FUNCTION__ ) );
      }
    }
    uint32_t help = mask;
    while ( (help & 0x00000001) == 0 ) {
      help >>= 1;
      data <<= 1;
    }
  }
  return data;
}

uint32_t HAL::AddressTableItem::applyFromMask( uint32_t data,
						    bool MaskBoundaryCheck ) const 
  throw( HAL::MaskBoundaryException ) {
  if ( mask == 0 ) {
    data = 0;
  } else {
	bool maskOk;
	if ( MaskBoundaryCheck ) {
	  maskOk = ( (data & (~ mask)) == 0 ); 
	  if ( ! maskOk ) {
            std::stringstream text;
			text << "Data bits set outside of mask.\n"
				 << "  Item : " << key << "\n" 
				 << "  Data : " << std::hex << data << "\n"  
				 << "  Mask : " << mask
				 << "      (HAL::AddressTableItem::applyFromMask)" 
				 << std::ends;
            std::string textStr = text.str();
            throw( HAL::MaskBoundaryException( textStr, __FILE__, __LINE__, __FUNCTION__ ) );
	  }
	}
        uint32_t help = mask;
	while ( (help & 0x00000001) == 0 ) {
	  help >>= 1;
	  data >>= 1;
	}
  }
  return data;
}
