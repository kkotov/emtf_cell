#ifndef __StaticVMEItem
#define __StaticVMEItem

#include <stdint.h>
#include <string>

#include "VMEConstants.h"
#include "hal/IllegalValueException.hh"

namespace HAL {

class StaticVMEItem {
public:
  StaticVMEItem( std::string serialNumber,
                 uint32_t slotId_ )
    throw( IllegalValueException );
  std::string getSerialNumber() const;
  uint32_t getSlotId() const;
private:
  std::string serialNumber_;
  uint32_t slotId_;
};

} /* namespace HAL */

#endif /* __StaticVMEItem */
