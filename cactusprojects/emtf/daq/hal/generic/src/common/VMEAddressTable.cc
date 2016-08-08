#include "hal/VMEAddressTable.hh"
#include <sstream>
#include <iomanip>

HAL::VMEAddressTable::VMEAddressTable( std::string name, HAL::AddressTableReader& tableReader )
  : HAL::AddressTable( name ) {
  HAL::AddressTableItem* newTableItem;
  // fill the itemMap 
  while ( tableReader.next( &newTableItem ) ) {
    itemMap[newTableItem->getKey()] = newTableItem;
  }
  determineAddressBoundaries();
}

HAL::VMEAddressTable::~VMEAddressTable() {}

void HAL::VMEAddressTable::print( std::ostream& os ) const {

  std::tr1::unordered_map<std::string, HAL::AddressTableItem*, HAL::HalHash<std::string> >::const_iterator it;
  os   << "\n******************************************************************************************************" << std::endl;
  os   << "*                   VME Address Table : " << name << std::endl;
  os   << "*                         item       AdrSpace  map AM  width   address      mask  r  w  description"<< std::endl;
  os   << "******************************************************************************************************" << std::endl;
  for( it=itemMap.begin(); it!=itemMap.end(); it++ ) { 
    (*it).second->print();
  }
  os   << "******************************************************************************************************\n" << std::endl;
}

void HAL::VMEAddressTable::getAddressBoundaries( std::vector<uint32_t>& minAddresses,
						 std::vector<uint32_t>& maxAddresses ) const {
  minAddresses = minAddresses_;
  maxAddresses = maxAddresses_;
}

void HAL::VMEAddressTable::checkAddressLimits( std::string item, uint64_t offset ) const
  throw ( HAL::NoSuchItemException,
          HAL::AddressOutOfLimitsException ) {
  // we have to be precise here: in the block transfer method the length
  // of the transfer is given in bytes. 
  const HAL::GeneralHardwareAddress& hardwareAddress = getGeneralHardwareAddress( item );
  uint32_t address = hardwareAddress.getAddress() + offset;
  uint32_t width = hardwareAddress.getDataWidth();
  if ( hardwareAddress.isMemorySpace() ) {
    uint32_t functionId = hardwareAddress.getMapId();
    if ( (address + width - 1) > maxAddresses_[functionId] || address < minAddresses_[functionId] ) {
      std::stringstream text;
      text << "The address is outside of the HAL::AddressTable range: \n"
           << "              HAL::AddressTable : " << name << "\n"
           << "                      item : " << item << "\n"
           << "               offset (hex): " << std::hex << offset << "\n"
           << std::ends;
      throw ( HAL::AddressOutOfLimitsException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
    }
  } else {
    if ( address > 0xffffff ) {
      std::stringstream text;
      text << "The address for configuration space access is outside of the configuration space of the module: \n"
           << "              HAL::AddressTable : " << name << "\n"
           << "                      item : " << item << "\n"
           << "               offset (hex): " << std::hex << offset << "\n"
           << std::ends;
      throw ( HAL::AddressOutOfLimitsException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
    }
  }
}

void HAL::VMEAddressTable::checkAddressLimits( const HAL::GeneralHardwareAddress& hardwareAddress,
					       uint64_t offset ) const
  throw ( HAL::AddressOutOfLimitsException ) {
  // we have to be precise here: in the block transfer method the length
  // of the transfer is given in bytes. 
  uint32_t address = hardwareAddress.getAddress() + offset;
  uint32_t width = hardwareAddress.getDataWidth();
  if ( hardwareAddress.isMemorySpace() ) {
    uint32_t functionId = hardwareAddress.getMapId();
    if ( (address + width - 1) > maxAddresses_[functionId] || address < minAddresses_[functionId] ) {
      std::stringstream text;
      text << "The address is outside of the HAL::AddressTable range: \n"
           << "               HAL::AddressTable : " << name << "\n"
           << "              address (hex) : " << std::hex << hardwareAddress.getAddress() << "\n"
           << "               offset (hex) : " << std::hex << offset << "\n"
           << std::ends;
      throw ( HAL::AddressOutOfLimitsException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
    }
  } else {
    if ( address > 0xffffff ) {
      std::stringstream text;
      text << "The address for configuration space access is outside of the configuration space of the module: \n"
           << "              HAL::AddressTable  : " << name << "\n"
           << "              address (hex) : " << std::hex << hardwareAddress.getAddress() << "\n"
           << "               offset (hex) : " << std::hex << offset << "\n"
           << std::ends;
      throw ( HAL::AddressOutOfLimitsException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
    }
  }
}

void HAL::VMEAddressTable::determineAddressBoundaries( ) {
  std::tr1::unordered_map<std::string, HAL::AddressTableItem*, HAL::HalHash<std::string> >::const_iterator it;
  uint32_t address, maddress, functionId;
  std::string key;

  // initialize the address boundary variables for the 
  // sorting algorithm below. The AddressesBoundaries for
  // each function are, of course, determined separately.
  maxAddresses_.clear();
  minAddresses_.clear();
  for ( int ic = 0; ic < NUMBER_OF_VME64XFUNCTIONS; ic++ ) {
	maxAddresses_.push_back(0);
	minAddresses_.push_back(~0x00);
  }

  // sort algorithm: scan all items in the AddressMap and 
  // memorize the largest and smallest addresses for each
  // function. 
  for ( it = itemMap.begin(); it != itemMap.end(); it++ ) {
    key = (*it).second->getKey();
    const HAL::GeneralHardwareAddress& vmeAddress = getGeneralHardwareAddress( key );
    // do not handle the configuration space here
    if ( vmeAddress.isMemorySpace() ) {
      address = vmeAddress.getAddress();
      functionId = vmeAddress.getMapId();
      maddress = address + vmeAddress.getDataWidth() - 1;
      if ( maddress > maxAddresses_[functionId] ) {
        maxAddresses_[functionId] = maddress;
      }
      if ( address < minAddresses_[functionId] ) {
        minAddresses_[functionId] = address;
      }
    }
  }
}

