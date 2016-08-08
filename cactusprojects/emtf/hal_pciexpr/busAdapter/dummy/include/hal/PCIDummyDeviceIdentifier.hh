#ifndef __PCIDummyDeviceIdentifier
#define __PCIDummyDeviceIdentifier

#include <stdint.h>
#include <vector>
#include <string>
#include "hal/PCIDeviceIdentifier.hh"

namespace HAL {

class PCIDummyDeviceIdentifier : public PCIDeviceIdentifier {
public:
  PCIDummyDeviceIdentifier( uint32_t deviceNumber,
                            std::vector<char *>&memorySpacePtr,
                            char *configSpacePtr,
			    std::vector<uint32_t> baseaddresses,
			    std::vector<uint32_t> mapsizes,
			    bool swapFlag = false);
 
  ~PCIDummyDeviceIdentifier();

  std::string printString() const;

  char* getConfigSpacePtr() const;

  char* remap( uint32_t address ) const throw( BusAdapterException );

  bool doSwap() const;

  uint32_t getPCIBusAddress( uint32_t bar ) const
    throw();

private :
  uint32_t deviceNumber;
  std::vector<char *>memorySpacePtr_;
  char *configSpacePtr_;
  std::vector<uint32_t> baseaddresses_;
  std::vector<uint32_t> mapsizes_;
  bool swapFlag_;
};

} /* namespace HAL */

#endif /* __PCIDummyDeviceIdentifier */

