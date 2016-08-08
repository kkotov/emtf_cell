#ifndef __ModuleMapperInterface
#define __ModuleMapperInterface

#include <stdint.h>
#include <string>

#include "hal/IllegalValueException.hh"
#include <sstream>
#include "hal/ModuleMapperInterface.hh"

namespace HAL {

/**
*
*
*     @short An interface  to retrieve information on a  module with a 
*            given serial number.
*
*       @see 
*    @author Christoph Schwick
* $Revision: 1.3 $
*     $Date: 2009/04/29 10:23:07 $
*
*
**/

class ModuleMapperInterface {

public:

  virtual ~ModuleMapperInterface() {};

  /**
   * This function maps the serialNumber to the TypeID. If the mapping
   * is unsuccessfull an exception is thrown.
   */
  virtual std::string getTypeId( std::string serialNumber ) 
    throw( IllegalValueException ) = 0;

  /**
   * Retrieve the baseAddress for a standard VME module.
   */
  virtual uint32_t getBaseaddress( std::string serialNumber ) 
    throw( IllegalValueException ) = 0;

};

} /* namespace HAL */

#endif /* __ModuleMapperInterface */
