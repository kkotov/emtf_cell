#include "hal/VME64xHardwareAddress.hh"
#include <sstream>
#include <iomanip>

HAL::VME64xHardwareAddress::VME64xHardwareAddress (uint32_t address, 
						   enum AddressSpace addressSpace,
						   uint32_t mappedWindowId,
						   uint32_t width ) 
  throw (HAL::IllegalValueException)
  : HAL::GeneralHardwareAddress( address, 0 ) {
  
  addressSpace_ = addressSpace;

  if ( mappedWindowId > 7 && addressSpace == (AddressSpace)MEMORY) {
    std::stringstream text;
    text << "mapped window ID of " << mappedWindowId 
	 << "is illegal for VME64x modules\n    (HAL::VME64xHardwareAddress::VME64xHardwareAddress)" 
	 << std::ends;
    throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }

  if ( addressSpace_ == (AddressSpace)CONFIGURATION ) {
    addressModifier_ = 0x2f;
    dataWidth_ = width; 
    mapId_ = 0;  // not used
  } else {
    addressModifier_ = 0;
    mapId_ = mappedWindowId;
    dataWidth_ = 4; //not used
  }
}

void HAL::VME64xHardwareAddress::setWindowConfiguration( uint32_t AM,
							 uint32_t dataWidth )
  throw (HAL::IllegalValueException) {
  
  if ( AM > 255 ) {
    std::stringstream text;
    text << "AddressModifier of "
	 << std::hex << AM
	 << " is illegal in this implementation.\n    (HAL::VME64xHardwareAddress::setWindowConfiguration)"
	 << std::ends;
    throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  if ( addressSpace_ == (AddressSpace)CONFIGURATION && AM != 0x2f ) {
    std::stringstream text;
    text << "AddressModifier of "
	 << std::hex << AM
	 << " is illegal for Configuration space items.\n    (HAL::VME64xHardwareAddress::setWindowConfiguration)"
	 << std::ends;
    throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  addressModifier_ = AM;
  dataWidth_ = dataWidth;
}

uint32_t HAL::VME64xHardwareAddress::getAddressModifier() const 
  throw () {
  return addressModifier_;
}

bool HAL::VME64xHardwareAddress::isIOSpace() const
  throw( HAL::IllegalOperationException ) {
  std::string text = "There is no IO space in VME64x.\n    (HAL::VME64xHardwareAddress::isIOSpace)";
  throw( HAL::IllegalOperationException( text, __FILE__, __LINE__, __FUNCTION__ ) );
}

std::string HAL::VME64xHardwareAddress::getAddressSpaceString() const {
  std::string resultString = "UNDEFINED";
  if (addressSpace_ == (AddressSpace)MEMORY) {
    resultString =  "memory";
  } else if (addressSpace_ == (AddressSpace)CONFIGURATION) {
    resultString = "configuration";
  }
  return resultString;
}

void HAL::VME64xHardwareAddress::print( std::ostream& os ) const {
  std::string vmeSpaceString = getAddressSpaceString();
  os  << std::setw(13) << vmeSpaceString.c_str() << "   ";
  if ( addressSpace_ == (AddressSpace)MEMORY) {
    os  << std::setw(1) << std::hex << mapId_ << "  ";
  } else {
    os  << "   ";
  }
  os  << std::setw(2) << std::hex << std::setfill('0') << addressModifier_ << "      "
      << std::setw(1) << std::dec << dataWidth_ << "  "
      << std::setfill('0') << std::setw(8) << std::hex << address_ << "  ";
}

HAL::GeneralHardwareAddress* HAL::VME64xHardwareAddress::clone() const {
  return new VME64xHardwareAddress( *this );
}
