#include "hal/PCIAddressTableDynamicReader.hh"

void HAL::PCIAddressTableDynamicReader::createItem( std::string key, 
						    enum AddressSpace addressSpace,
						    uint32_t bar,
						    uint32_t address, 
						    uint32_t mask,
						    uint32_t isReadable, 
						    uint32_t isWritable, 
						    std::string description) {
  
  HAL::PCIHardwareAddress* addressPointer = new HAL::PCIHardwareAddress(address, addressSpace, bar);
  HAL::AddressTableItem* itemPointer = new HAL::AddressTableItem(key, description, *addressPointer,
								 mask, isWritable, isReadable);  
  itemPointerList.push_back( itemPointer );
}
