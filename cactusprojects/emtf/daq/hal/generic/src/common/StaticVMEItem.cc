#include "hal/StaticVMEItem.hh"
#include <sstream>
#include <iomanip>

HAL::StaticVMEItem::StaticVMEItem(  std::string serialNumber,
				    uint32_t slotId )
  throw( HAL::IllegalValueException ) 
  : serialNumber_(serialNumber),
    slotId_(slotId) {
  
  if ( slotId_ > MAX_NUMBER_OF_SLOTS ) {
    std::stringstream text;
    text <<  "Too large number of slots : " 
         << std::dec << slotId_ 
         << "  (max is " 
         << MAX_NUMBER_OF_SLOTS
         << ")\n    (HAL::StaticVMEItem::StaticVMEItem)" << std::ends;
    throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
}

std::string HAL::StaticVMEItem::getSerialNumber() const {
  return serialNumber_;
}

uint32_t HAL::StaticVMEItem::getSlotId() const {
  return slotId_;
}
  
