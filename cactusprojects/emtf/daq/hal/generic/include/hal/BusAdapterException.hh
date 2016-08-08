#ifndef __BusAdapterException
#define __BusAdapterException

#include "hal/HardwareAccessException.hh"
#include "string"

namespace HAL {

/**
*
*
*     @short Thrown if there is a problem in the BusAdapter.
*
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/

class BusAdapterException : public HardwareAccessException {
public:
  BusAdapterException( const std::string message, 
		       const std::string module,
		       const int line,
		       const std::string function ) :
    HardwareAccessException( "BusAdapterException", 
			     message, 
			     module,
			     line, 
			     function ) {};

  BusAdapterException( const std::string message,
		       const std::string module,
		       const int line,
		       const std::string function,
			     xcept::Exception& previous ) :
    HardwareAccessException( "BusAdapterException",
			     message, 
			     module,
			     line, 
			     function, 
			     previous ) {};
};

} /* namespace HAL */

#endif /* __BusAdapterException */
