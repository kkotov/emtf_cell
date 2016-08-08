#include "hal/VMEAddressTableDynamicReader.hh"
#include "hal/VMEHardwareAddress.hh"

void HAL::VMEAddressTableDynamicReader::createItem( std::string key, 
						    uint32_t width,
						    uint32_t address, 
						    uint32_t addressModifier,
						    uint32_t mask,
						    uint32_t isReadable, 
						    uint32_t isWritable, 
						    std::string description) {
  
  HAL::VMEHardwareAddress* addressPointer = new HAL::VMEHardwareAddress(address,addressModifier,width);
  HAL::AddressTableItem* itemPointer = new HAL::AddressTableItem(key, description, *addressPointer,
								 mask, isWritable, isReadable);  
  itemPointerList.push_back( itemPointer );
}
