#ifndef __VMEDummyDeviceIdentifier
#define __VMEDummyDeviceIdentifier

#include <stdint.h>
#include <string>
#include <vector>
#include "hal/BusAdapterException.hh"
#include "hal/DeviceIdentifier.hh"
#include "hal/VMEConstants.h"

namespace HAL {

class VMEDummyDeviceIdentifier : public DeviceIdentifier {
public:
    VMEDummyDeviceIdentifier( uint32_t deviceNumber,
                              std::vector<char *> memoryRegionPtr,
                              std::vector<uint32_t> baseaddresses,
                              std::vector<uint32_t> mapsizes,
                              bool doSwapping );

    ~VMEDummyDeviceIdentifier();

    std::string printString() const ;

    char* remap( uint32_t address )const 
      throw( BusAdapterException );

    bool doSwapping() const;

private :
    uint32_t deviceNumber_;
    std::vector<char *> memoryRegionPtr_;
    std::vector<uint32_t> baseaddresses_;
    std::vector<uint32_t> mapsizes_;
    bool doSwapping_;
};

} /* namespace HAL */

#endif /* __VMEDummyDeviceIdentifier */

