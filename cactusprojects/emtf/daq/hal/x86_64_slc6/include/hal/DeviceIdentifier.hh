#ifndef __DeviceIdentifier
#define __DeviceIdentifier

#include <string>

namespace HAL {

/**
*
*
*     @short Contains  information  to  identifiy  a  hardware  device 
*            unambiguously on the bus.
*            
*            Always  when  the BusAdapter  has  to perform  operations 
*            which are  not simply  memory mapped it needs information 
*            to  identify the  device. This  information  in principle 
*            varies  for  different  BusAdapters  and  of  course  for 
*            different technologies  (PCI or VME). In  order to handle 
*            requests  in  a  general  way  this  baseclass  has  been 
*            introduced.  It   allows  formulating  requests   to  the 
*            BusAdapter   in  a   implementation   independent  way.    
*
*       @see 
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/

class DeviceIdentifier {
public:
  virtual std::string printString() const = 0;
  /**
   * The Destructor of the DeviceIdentifier.
   *
   * If the BusAdapter uses memory mapping (like for example the
   * DummyBusAdapters) this is the place where the memory maps
   * can be deleted. The DeviceIdentifier is deleted in the 
   * closeDevice method of the HardwareDevice. So if in the 
   * constructor to the DeviceIdentifier the pointers to the 
   * created maps are passed they can be deleted here. See the
   * DummyBusAdapters for an example.
   *
   * @see VMEDummyBusAdapter PCIDummyBusAdapter
   */
  virtual ~DeviceIdentifier() {};

};

} /* namespace HAL */

#endif /* __DeviceIdentifier */
