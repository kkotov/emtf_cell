#ifndef __AddressTableContainerInterface
#define __AddressTableContainerInterface

#include <string>
#include "hal/VMEAddressTable.hh"
#include "hal/PCIAddressTable.hh"
#include "hal/IllegalValueException.hh"

namespace HAL {

/**
*
*
*     @short Creates AddressTables and keeps them.
*
*            This   class   remains   the   owner   of   the   created 
*            AddressTables.  It  deletes   them  in  its  destructor.   
*            Therfore an  object of this  class must have  a lifecycle 
*            at least as long as its AddressTables are in use. 
*
*       @see 
*    @author Christoph Schwick
* $Revision: 1.3 $
*     $Date: 2009/04/29 10:23:07 $
*
*
**/

class AddressTableContainerInterface {

public:

  virtual ~AddressTableContainerInterface() {};

  /**
   * Retrieve the AddressTable for a VME64x card if 
   * the serialnumber is given.
   */
  virtual VMEAddressTable& 
  getVMETableFromSerialNumber( std::string serialNumber ) 
    throw( IllegalValueException ) = 0;

};

} /* namespace HAL */

#endif /* __AddressTableContainerInterface */
