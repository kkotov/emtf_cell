#ifndef __ConfigCSRDumper_
#define __ConfigCSRDumper_

#include <vector>
#include <string>
#include "hal/VMEConfigurationSpaceHandler.hh"

namespace HAL {

class ConfigCSRDumper {
public:
  ConfigCSRDumper();
  void dumpCSR (const VMEConfigurationSpaceHandler&, 
		const uint32_t slotId );
private:
  void displayItem( const std::vector<unsigned char>csr, 
		    const uint32_t length,
		    const uint32_t address,
		    const std::string key,
		    const std::string description );

};

} /* namespace HAL */

#endif
