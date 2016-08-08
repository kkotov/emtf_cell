#ifndef __TimeoutException
#define __TimeoutException

#include "hal/HardwareAccessException.hh"

namespace HAL {

/**
*
*
*     @short Indicates that a timeout has expired.
*          
*            This exception is used in the pollItem method of the 
*            HardwareDeviceInterface.  
*
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class TimeoutException : public HardwareAccessException {
public:
  TimeoutException( const std::string message, 
		    const std::string module,
		    const int line,
		    const std::string function ) :
    HardwareAccessException( "TimeoutException", 
			     message, 
			     module,
			     line, 
			     function ) {};

  TimeoutException( const std::string message,
		    const std::string module,
		    const int line,
		    const std::string function,
			     xcept::Exception& previous ) :
    HardwareAccessException( "TimeoutException",
			     message, 
			     module,
			     line, 
			     function, 
			     previous ) {};
};

} /* namespace HAL */

#endif /* __TimeoutException */
