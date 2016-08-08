#ifndef __PCIDeviceIdentifier
#define __PCIDeviceIdentifier

#include <stdint.h>
#include "hal/DeviceIdentifier.hh"
#include "hal/BusAdapterException.hh"

namespace HAL {

/**
*
*
*     @short Some extensions to the DeviceIdentifier for PCI.
*            
*            In order to provide support for DMA functionality the user
*            must   be   able   to   retrieve   the   PCI   BusAddress 
*            corresponding to a BAR. 
*
*       @see DeviceIdentifier
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class PCIDeviceIdentifier : public DeviceIdentifier {
public:
  /**
   * Retrieve the Bus address of a BAR. 
   */
  virtual uint64_t getPCIBusAddress( uint64_t iBar ) const 
    throw( BusAdapterException )= 0;
  virtual bool doSwap() const = 0;
};

} /* namespace HAL */

#endif /* __PCIDeviceIdentifier */
