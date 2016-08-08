#include "hal/VMEHardwareAddress.hh"

#include <sstream>
#include <iomanip>

HAL::VMEHardwareAddress::VMEHardwareAddress (uint32_t address, 
					     uint32_t addressModifier,
					     uint32_t dataWidth ) 
  throw (HAL::IllegalValueException)
  : HAL::GeneralHardwareAddress( address, dataWidth ) {
  
  mapId_ = 0;
  if ( addressModifier == 0x2f ) {
    addressSpace_ = (AddressSpace)CONFIGURATION;
  } else {
    addressSpace_ = (AddressSpace)MEMORY;
  }
  if (addressModifier > 255 ) {
    std::stringstream text;
    text << "AddressModifier of "
	 << std::hex << addressModifier
	 << " is illegal in this implementation.\n    (HAL::VMEHardwareAddress::VMEHardwareAddress)"
	 << std::ends;
    throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  addressModifier_ = addressModifier;
}

uint32_t HAL::VMEHardwareAddress::getAddressModifier() const 
  throw () {
  return addressModifier_;
}

bool HAL::VMEHardwareAddress::isIOSpace() const
  throw( HAL::IllegalOperationException ) {
  std::string text = "There is no IO space in VME.\n    (HAL::VMEHardwareAddress::isIOSpace)";
  throw( HAL::IllegalOperationException( text, __FILE__, __LINE__, __FUNCTION__ ) );
}

std::string HAL::VMEHardwareAddress::getAddressSpaceString() const {
  std::string resultString = "UNDEFINED";
  if (addressSpace_ == (AddressSpace)MEMORY) {
    resultString =  "memory";
  } else if (addressSpace_ == (AddressSpace)CONFIGURATION) {
    resultString = "configuration";
  }
  return resultString;
}

void HAL::VMEHardwareAddress::print( std::ostream& os ) const {
  std::string vmeSpaceString = getAddressSpaceString();
  os << std::setw(13) <<"  " << "   ";
  os << "   ";
  os << std::setw(2) << std::hex << std::setfill('0') << addressModifier_ << "      "
     << std::setw(1) << std::dec << dataWidth_ << "  "
     << std::setfill('0') << std::setw(8) << std::hex << address_ << "  ";
}

HAL::GeneralHardwareAddress* HAL::VMEHardwareAddress::clone() const {
  return new VMEHardwareAddress( *this );
}
