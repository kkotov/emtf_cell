#include "hal/VME64xDevice.hh"
#include <sstream>

HAL::VME64xDevice::VME64xDevice( HAL::VMEAddressTable & vmeAddressTable,
				 HAL::VMEBusAdapterInterface & vmeBusAdapter,
				 std::vector<uint32_t>& baseaddresses,
				 bool doSwapping,
				 const HAL::VMESlot &vmeSlot )
  
  throw(  HAL::NoSuchItemException, 
	  HAL::IllegalOperationException,
	  HAL::BusAdapterException,
	  HAL::IllegalValueException,
	  HAL::UnsupportedException )

  : VMEDevice( vmeAddressTable,
	       vmeBusAdapter,
	       baseaddresses, 
	       doSwapping ),
    vmeSlot_( vmeSlot )
{
  configSpaceOffset_ = vmeSlot_.getSlotId() * VME64X_CONFIGURATIONSPACE_SIZE;
  CRAMAccessWidth_ = getAccessWidth( "CRAMWidth" );
  CROMAccessWidth_ = getAccessWidth( "CRWidth" );
  CSRAccessWidth_  = getAccessWidth( "CSRWidth" );
  
  vmeSlot_.stdConfigRead( "userCRStart", &userCRStart_ );
  vmeSlot_.stdConfigRead( "userCREnd", &userCREnd_ );
  vmeSlot_.stdConfigRead( "userCSRStart", &userCSRStart_ );
  vmeSlot_.stdConfigRead( "userCSREnd", &userCSREnd_ );
  vmeSlot_.stdConfigRead( "CRAMStart", &CRAMStart_ );
  vmeSlot_.stdConfigRead( "CRAMEnd", &CRAMEnd_ );

  // do some checks on the values read in:
  if ( (userCRStart_ != 0 && userCREnd_ <= userCRStart_) ||
       (userCRStart_ != 0 && userCRStart_ < 0x1000) ||
       (userCREnd_ >= 0x07fc00) ) 
    {
      std::stringstream text;
      text << "Illegal range for userCR defined in configuration space:\n"
	   << "   found " << std::hex << userCRStart_ << " ... " << userCREnd_ << "\n"
	   << "   must be in range " << 0x1000 << " ... " << 0x07fc00 
	   << std::ends;
      throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
   }
 
  if ( (userCSRStart_ != 0 && userCSREnd_ <= userCSRStart_) ||
       (userCSRStart_ != 0 && userCSRStart_ < 0x1000) ||
       (userCSREnd_ >= 0x07fc00) ) 
    {
      std::stringstream text;
      text << "Illegal range for userCSR defined in configuration space:\n"
	   << "   found " << std::hex << userCSRStart_ << " ... " << userCSREnd_ << "\n"
	   << "   must be in range " << 0x1000 << " ... " << 0x07fc00 
	   << std::ends;
      throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
   }
 
  if ( (CRAMStart_ != 0 && CRAMEnd_ <= CRAMStart_) ||
       (CRAMStart_ != 0 && CRAMStart_ < 0x1000) ||
       (CRAMEnd_ >= 0x07fc00) ) 
    {
      std::stringstream text;
      text << "Illegal range for CRAM defined in configuration space:\n"
	   << "   found " << std::hex << CRAMStart_ << " ... " << CRAMEnd_ << "\n"
	   << "   must be in range " << 0x1000 << " ... " << 0x07fc00 
	   << std::ends;
      throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
   }
 
 }

void HAL::VME64xDevice::hardwareRead( const HAL::GeneralHardwareAddress& vmeAddress, 
				      uint32_t* result,
				      uint32_t offset ) const
  throw( HAL::BusAdapterException )
{
  if ( vmeAddress.isMemorySpace() )
    {
      VMEDevice::hardwareRead( vmeAddress, result, offset );
    } 
  else 
    {
      userConfigRead( vmeAddress, result, offset );
    }
}

uint32_t HAL::VME64xDevice::getAccessInterval( uint32_t address, uint32_t width ) const
  throw( HAL::IllegalOperationException )
{
  uint32_t interval = 0;
  if ( userCRStart_ > 0 &&
       address >= userCRStart_ && address <= (userCREnd_ - ((width-1)*CROMAccessWidth_)) )
    {
      interval = CROMAccessWidth_;
    }
  else if ( userCSRStart_ > 0 &&
	    address >= userCSRStart_ && address <= (userCSREnd_ - ((width-1)*CSRAccessWidth_)) )
    {
      interval = CSRAccessWidth_;
    }
  else if ( CRAMStart_ > 0 &&
	    address >= CRAMStart_ && address <= (CRAMEnd_ - ((width-1)*CRAMAccessWidth_)) )
    {
      interval = CRAMAccessWidth_;
    }
  // also check for standard pre-defined configuration space items
  else if ( address < 0x1000 ||
	    (address > 0x7fc00 && address < 80000) )
    {
      interval = 4;
    }
  else 
    {
      std::stringstream text;
      text << "The config-space address 0x" << std::hex << address 
	   << " does not fall in one of the regions\n"
	   << "UserCR, UserCSR, or CRAM. Therfore it cannot be accessed!"
	   << std::ends;  
      throw( HAL::IllegalOperationException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
    }
  return interval;
}

void HAL::VME64xDevice::userConfigRead(const HAL::GeneralHardwareAddress& vmeAddress, 
				       uint32_t* resultPtr,
				       uint32_t offset) const
  throw( HAL::BusAdapterException,
	 HAL::IllegalOperationException)
{

  uint32_t width = vmeAddress.getDataWidth();
  uint32_t address = vmeAddress.getAddress();
  uint32_t interval = getAccessInterval( address, width );

  uint32_t myresult = 0;
  uint32_t newByte;
  for ( uint32_t i = 0; i < width; i++ ) {
    vmeBusAdapter.read( deviceIdentifierPtr,
			vmeAddress.getAddress() + configSpaceOffset_ + offset,
			0x2fUL, 1UL, &newByte );
    newByte &= 0xff;
    offset += interval;
    myresult = ((myresult << 8) & 0xffffff00) + newByte;
  }
  *resultPtr = myresult;
}



void HAL::VME64xDevice::hardwareWrite( const HAL::GeneralHardwareAddress& vmeAddress, 
				       uint32_t data,
				       uint32_t offset ) const
  throw( HAL::BusAdapterException )
{
  if ( vmeAddress.isMemorySpace() )
    {
      VMEDevice::hardwareWrite( vmeAddress, data, offset );
    } 
  else 
    {
      userConfigWrite( vmeAddress, data, offset );     
     }
}

void HAL::VME64xDevice::userConfigWrite(const HAL::GeneralHardwareAddress& vmeAddress, 
					uint32_t data,
					uint32_t offset) const
  throw( HAL::IllegalOperationException,
	 HAL::BusAdapterException )
{
  uint32_t width = vmeAddress.getDataWidth();
  uint32_t address = vmeAddress.getAddress();
  uint32_t interval = getAccessInterval( address, width );

  uint32_t newByte;
  for ( int i = (width - 1); i >= 0; i-- ) {
    newByte = (data >> (i*8)) & 0xff;
    vmeBusAdapter.write( deviceIdentifierPtr,
                         address + configSpaceOffset_ + offset,
                         0x2fUL, 1UL, newByte );
    offset += interval;
  }
}

void HAL::VME64xDevice::stdConfigRead( std::string item,
				       uint32_t* resultPtr, 
				       uint32_t offset ) const
  throw( HAL::NoSuchItemException, 
	 HAL::IllegalOperationException,
	 HAL::BusAdapterException )
{
  vmeSlot_.stdConfigRead( item, resultPtr, offset ); 
}

void HAL::VME64xDevice::stdConfigWrite( std::string item,
					uint32_t data,
					HalVerifyOption verifyFlag,	
					uint32_t offset ) const
  throw( HAL::NoSuchItemException, 
	 HAL::IllegalOperationException,
	 HAL::BusAdapterException )
{
  vmeSlot_.stdConfigWrite( item, data, verifyFlag, offset ); 
}

std::string HAL::VME64xDevice::getSerialNumber() const
{
  return vmeSlot_.getSerialNumber();
}


uint32_t HAL::VME64xDevice::getAccessWidth( std::string item ) const
  throw( HAL::NoSuchItemException, 
	 HAL::IllegalOperationException,
	 HAL::BusAdapterException )
{
  uint32_t byteInterval, width;
  vmeSlot_.stdConfigRead( item, &width );
  if ( width == 0x81 ) {
	byteInterval = 4;
  } else if( width == 0x82 ) {
	byteInterval = 2;
  } else {
	byteInterval = 1;
  }
  return byteInterval;
};
