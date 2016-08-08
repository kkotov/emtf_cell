#ifndef __VMEConfigurationSpaceAddressReader
#define __VMEConfigurationSpaceAddressReader

#include <string>

#include "hal/AddressTableReader.hh"
#include "hal/VMEHardwareAddress.hh"

namespace HAL {

/**
*
*
*     @short A reader which generated the VME64x configuration space
*            
*            This class is a hardcoded implementation of the address-
*            items of the VME64x specification.
*
*       @see 
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class VMEConfigurationSpaceAddressReader : public AddressTableReader {

public:
  VMEConfigurationSpaceAddressReader();

private: 
  void createItem( std::string key, 
		   uint32_t width,
		   uint32_t address, 
		   uint32_t isReadable, 
		   uint32_t isWritable, 
		   std::string description);
};

} /* namespace HAL */

#endif /* __VMEConfigurationSpaceAddressReader */
