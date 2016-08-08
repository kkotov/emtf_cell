#ifndef __AddressOutOfLimitsException
#define __AddressOutOfLimitsException

#include <string>
#include "hal/HardwareAccessException.hh"

namespace HAL {

/**
*
*
*     @short Indicates that an address is not in the range of the 
*            address defined in an addressTable.
*
*            This Exception is thrown if it is tried to access an item
*            with an offset,  and the resulting address is not
*            in the range of the items of the addressTable.
*
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/
class AddressOutOfLimitsException : public HardwareAccessException {
public:
  AddressOutOfLimitsException( const std::string message, 
			       const std::string module,
			       const int line,
			       const std::string function ) :
    HardwareAccessException( "AddressOutOfLimitsException", 
			     message, 
			     module,
			     line, 
			     function ) {};

  AddressOutOfLimitsException( const std::string message,
			       const std::string module,
			       const int line,
			       const std::string function,
			       xcept::Exception& previous ) :
    HardwareAccessException( "AddressOutOfLimitsException",
			     message, 
			     module,
			     line, 
			     function, 
			     previous ) {};
};

} /* namespace HAL */

#endif /* __AddressOutOfLimitsException */
