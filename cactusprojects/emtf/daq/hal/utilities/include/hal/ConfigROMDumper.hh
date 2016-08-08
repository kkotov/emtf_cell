#ifndef __ConfigROMDumper_
#define __ConfigROMDumper_

#include <vector>
#include <string>

#include "hal/VMEConfigurationSpaceHandler.hh"

namespace HAL {

class ConfigROMDumper {
public:
  ConfigROMDumper();
  void dumpROM (const VMEConfigurationSpaceHandler&, 
		const uint32_t slotId );
private:
  void displayItem( const std::vector<unsigned char>rom, 
		    const uint32_t length,
		    const uint32_t address,
		    const std::string key,
		    const std::string description );

};

} /* namespace HAL */

#endif
