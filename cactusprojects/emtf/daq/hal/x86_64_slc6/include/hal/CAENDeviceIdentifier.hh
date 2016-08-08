#ifndef __CAENDeviceIdentifier
#define __CAENDeviceIdentifier

#include <stdint.h>
#include <string>
#include "hal/BusAdapterException.hh"
#include "hal/DeviceIdentifier.hh"

namespace HAL {

/**
*
*
*     @short Stores specific device information for the CAENLinuxBusAdapter.
*            This  class  stores for  each  device a  deviceDescriptor
*            which is needed by the CAEN busAdapter in order to access
*            the  device.   The  Descriptor and  the  DeviceIdentifier
*            itself  are  created  in   the  openDevice  call  of  the
*            CAENLinuxBusAdapter.
*
*       @see CAENLinuxBusAdapter
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 17:52:18 $
*
*
**/

class CAENDeviceIdentifier : public DeviceIdentifier {

public:
  /**
   * The adapterHandle is created in the CAENVME_Init call of the CAEN
   * BusAdapter.
   */
  CAENDeviceIdentifier( uint32_t adapterHandle,
                        bool doSwapping = false )
    throw (BusAdapterException) ;

  ~CAENDeviceIdentifier();

  const long getAdapterHandle() const;

  const bool doSwapping() const;

  std::string printString() const;

private:
  long  adapterHandle_; 
  bool  doSwapping_;
};

} /* namespace HAL */

#endif /* __CAENDeviceIdentifier */
