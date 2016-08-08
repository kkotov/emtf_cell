#ifndef __PCIExprLinuxDeviceIdentifier
#define __PCIExprLinuxDeviceIdentifier

#include <string>
#include "hal/PCIDeviceIdentifier.hh"
#include "hal/BusAdapterException.hh"
#include "xpci/Bus.h"
#include "xpci/Address.h"

namespace HAL {

/**
*
*
*     @short The DeviceIdentifier for the PCIExprLinuxBusAdapter.
*            
*            The  class   contains  data   which  is  needed   by  the 
*            PCIExprLinuxBusAdapter to perform configuration space accesses.
*            In addition  it keeps pointers to the  address maps which 
*            the  PCIBusAdapter  created  for memory  mapped  hardware 
*            accesss  (used in  memory accesses).  They are  used when 
*            unregistering  the   device  in  order   to  destroy  the 
*            mapping. 
*
*       @see PCIExprLinuxBusAdapter
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 17:54:12 $
*
*
**/

class PCIExprLinuxDeviceIdentifier : public PCIDeviceIdentifier {
public :

  PCIExprLinuxDeviceIdentifier( xpci::Address deviceConfigAddress,
			   bool swapFlag = false);

  /**
   * Destruction of the Identifier and the maps.
   * The memory maps which are related to the hardware device identified
   * by this identifier are destroyed by this desructor.
   */
  virtual ~PCIExprLinuxDeviceIdentifier();

  xpci::Address getConfigAddress();

//  /**
//   * Sets the internal data array "maps[]". 
//   * @throws HardwareAccessException is thrown if the index ix is 
//   *         out of range. 
//   */
//  void setMap( int iMap, xdsh_Pmap* map ) 
//    throw (BusAdapterException);

//  /**
//   * Returns the bus-addresses for a given BAR.
//   * This function is useful if the busAddress must be given to a 
//   * DMA engine.
//   *
//   */

  uint64_t getPCIBusAddress( uint64_t iBar ) const 
    throw (BusAdapterException);

  /**
   * Returns an internal state variable, set in the constructor.
   */
  bool doSwap() const;

  std::string printString() const;


private:
  xpci::Address deviceConfigAddress;
  bool swapFlag;
};

} /* namespace HAL */

#endif /* __PCIExprLinuxDeviceIdentifier */
