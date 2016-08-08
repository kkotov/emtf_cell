#include "hal/PCIAddressTable.hh"
#include <sstream>
#include <iomanip>
#include <vector>

HAL::PCIAddressTable::PCIAddressTable( std::string name, 
				       HAL::AddressTableReader& tableReader ) 
  : HAL::AddressTable( name ) {
  HAL::AddressTableItem* newTableItem;
  // fill the itemMap 
  while ( tableReader.next( &newTableItem ) ) {
    itemMap[newTableItem->getKey()] = newTableItem;
  }
  determineAddressBoundaries();
}

HAL::PCIAddressTable::~PCIAddressTable() {
}

void HAL::PCIAddressTable::print( std::ostream& os ) const {
  std::tr1::unordered_map<std::string, HAL::AddressTableItem*, HAL::HalHash<std::string> >::const_iterator it;
  os << "\n******************************************************************************************************" << std::endl;
  os << "*                   PCI Address Table : " << name << std::endl;
  os << "*                         item       AdrSpace BAR  address      mask  r  w  description"<< std::endl;
  os << "******************************************************************************************************" << std::endl;
  for( it=itemMap.begin(); it!=itemMap.end(); it++ ) {
    (*it).second->print();
  }
  os << "******************************************************************************************************\n" << std::endl;
 }

void HAL::PCIAddressTable::getAddressBoundaries( uint32_t& minConfigAddress,
						 uint32_t& maxConfigAddress,
						 std::vector<uint32_t>& minAddresses,
						 std::vector<uint32_t>& maxAddresses ) const {
  minConfigAddress = minConfigAddress_;
  maxConfigAddress = maxConfigAddress_;
  minAddresses.clear();
  maxAddresses.clear();
  minAddresses.insert(minAddresses.begin(),minAddresses_.begin(),minAddresses_.end());
  maxAddresses.insert(maxAddresses.begin(),maxAddresses_.begin(),maxAddresses_.end());
}

void HAL::PCIAddressTable::getAddressBoundaries( uint64_t& minConfigAddress,
						 uint64_t& maxConfigAddress,
						 std::vector<uint64_t>& minAddresses,
						 std::vector<uint64_t>& maxAddresses ) const {
  minConfigAddress = minConfigAddress_;
  maxConfigAddress = maxConfigAddress_;
  minAddresses = minAddresses_;
  maxAddresses = maxAddresses_;
}


void HAL::PCIAddressTable::checkAddressLimits( std::string item,
					  uint64_t offset ) const
  throw( HAL::NoSuchItemException,
	 HAL::AddressOutOfLimitsException ) {
  const HAL::GeneralHardwareAddress& pciAddress = getGeneralHardwareAddress( item );
  uint64_t address = pciAddress.getAddress() + offset;
  uint64_t width = pciAddress.getDataWidth();
  if ( ( pciAddress.isConfigSpace() && 
	 ( (address + width - 1) > maxConfigAddress_ || address < minConfigAddress_) ) || 
       ( pciAddress.isMemorySpace() && 
	 ( (address + width - 1) > maxAddresses_[pciAddress.getMapId()] || address < minAddresses_[pciAddress.getMapId()]) )) {
    std::stringstream text;
    text << "The address is outside of the HAL::AddressTable range: \n"
		 << "               HAL::AddressTable : " << name << "\n"
		 << "                       item : " << item << "\n"
		 << "               offset (hex) : " << std::hex << offset << "\n"
		 << std::ends;
    throw ( HAL::AddressOutOfLimitsException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
}

void HAL::PCIAddressTable::checkAddressLimits( const HAL::GeneralHardwareAddress& pciAddress,
					  uint64_t offset ) const
  throw( HAL::AddressOutOfLimitsException ) {
  uint64_t address = pciAddress.getAddress() + offset;
  uint64_t width = pciAddress.getDataWidth();
  if ( ( pciAddress.isConfigSpace() && 
	 ( (address + width - 1) > maxConfigAddress_ || address < minConfigAddress_) ) || 
       ( pciAddress.isMemorySpace() && 
	 ( (address + width - 1) > maxAddresses_[pciAddress.getMapId()] || address < minAddresses_[pciAddress.getMapId()]) )) {
    std::stringstream text;
    text << "The address is outside of the HAL::AddressTable range: \n"
	 << "              HAL::AddressTable : " << name << "\n"
	 << "              address (hex): " << std::hex << pciAddress.getAddress() << "\n"
	 << "               offset (hex): " << std::hex << offset << "\n"
	 << std::ends;
    throw ( HAL::AddressOutOfLimitsException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
}

void HAL::PCIAddressTable::determineAddressBoundaries() {
  std::tr1::unordered_map<std::string, HAL::AddressTableItem*, HAL::HalHash<std::string> >::const_iterator it;
  uint64_t address, barId;
  std::string key;

  // initialize the address boundary variables for the 
  // sorting algorithm below. The AddressesBoundaries for
  // each BAR are of course determined separately.
  maxConfigAddress_ = 0;
  minConfigAddress_ = ~0x00;
  for ( int ic = 0; ic < 6; ic++ ) {
	maxAddresses_.push_back(0);
	minAddresses_.push_back(~0x00);
  }

  // sort algorithm: scan all items in the AddressMap and 
  // memorize the largest and smallest addresses in configuration
  // space and memory space. 
  for ( it = itemMap.begin(); it != itemMap.end(); it++ ) {
    key = (*it).second->getKey();
    const HAL::GeneralHardwareAddress& pciAddress = getGeneralHardwareAddress( key );
      address = pciAddress.getAddress();
    if ( pciAddress.isConfigSpace() ) {
      if ( address >= maxConfigAddress_ ) maxConfigAddress_ = 
					   address + pciAddress.getDataWidth() - 1;
	  if ( address < minConfigAddress_ ) minConfigAddress_ = address;
    } else if ( pciAddress.isMemorySpace() ) {
      barId = pciAddress.getMapId();
      if ( address >= maxAddresses_[barId] ) maxAddresses_[barId] = 
					      address + pciAddress.getDataWidth() - 1;
      if ( address < minAddresses_[barId] ) minAddresses_[barId] = address;
    }
  }  
}
