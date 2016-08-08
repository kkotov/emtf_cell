#include "hal/PCIHardwareAddress.hh"
#include <iomanip>
#include <sstream>

HAL::PCIHardwareAddress::PCIHardwareAddress( uint64_t address,
					     enum AddressSpace pciSpace,
					     uint64_t barId ) 
  throw (HAL::IllegalValueException)
  : HAL::GeneralHardwareAddress( address, 4 ) {   

  if ( pciSpace == (AddressSpace)MEMORY && barId > 5 ) {
    std::stringstream text;
    text << "barId of " << barId 
	 << " is illegal\n    (HAL::PCIHardwareAddress::PCIHardwareAddress)"
	 << std::ends;
    throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  mapId_ = barId;
  addressSpace_ = pciSpace;
  addressModifier_ = 0;  // not used in PCI
}

uint32_t HAL::PCIHardwareAddress::getAddressModifier() const
  throw( HAL::IllegalOperationException ) {
  std::string text = "There are no AddressModifiers in PCI\n    (HAL::PCIHardwareAddress::getAddressModifier)";
  throw( HAL::IllegalOperationException( text, __FILE__, __LINE__, __FUNCTION__ ) );
}


bool HAL::PCIHardwareAddress::isIOSpace() const throw() {
  return ( addressSpace_ == (AddressSpace)IO ) ;
}

std::string HAL::PCIHardwareAddress::getAddressSpaceString() const {
  std::string resultString = "UNDEFINED";
  if (addressSpace_ == (AddressSpace)MEMORY) {
    resultString =  "memory";
  } else if (addressSpace_ == (AddressSpace)CONFIGURATION) {
    resultString = "configuration";
  } else if (addressSpace_ == (AddressSpace)IO) {
    resultString = "I/O";
  } 
  return resultString;
}

void HAL::PCIHardwareAddress::print( std::ostream& os ) const {
  std::string pciSpaceString = getAddressSpaceString();
  os << std::setw(13) << pciSpaceString.c_str() << "  ";
  if ( addressSpace_ == (AddressSpace)MEMORY) {
    os << std::setw(1) << std::hex << mapId_ << "  ";
  } else {
    os << "   ";
  }
  os << std::setfill('0') << std::setw(8) << std::hex << address_ << "  ";
}

HAL::GeneralHardwareAddress* HAL::PCIHardwareAddress::clone() const {
  return new PCIHardwareAddress( *this );
}
