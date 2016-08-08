#ifndef __HardwareAccessException
#define __HardwareAccessException

#include <stdexcept>
#include "xcept/Exception.h"

namespace HAL {

/**
*
*
*     @short The baseclass of all exceptions thrown in this library.
*
*
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/

class HardwareAccessException : public xcept::Exception {
public:
  HardwareAccessException ( ) {};

  HardwareAccessException( const std::string name,
			   const std::string message, 
			   const std::string module,
			   const int line,
			   const std::string function ) :
    Exception( name, message, module, line, function ) {};
  
  HardwareAccessException( const std::string name,
			   const std::string message, 
			   const std::string module,
			   const int line,
			   const std::string function,
			   xcept::Exception& previous ) :
    Exception( name, message, module, line, function, previous ) {};
  
};

} /* namespace HAL */

#endif /* __HardwareAccessException */
