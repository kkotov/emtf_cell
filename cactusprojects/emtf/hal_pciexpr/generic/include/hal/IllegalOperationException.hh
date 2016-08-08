#ifndef __IllegalOperationException
#define __IllegalOperationException

#include <string>

#include "hal/HardwareAccessException.hh"

namespace HAL {

/**
*
*
*     @short Indicates  the  use of  a  operation  in an  inapropriate 
*            context. 
* 
*            Examples are  trying to write to  a non-writable address, 
*            trying  to use  the setBit()  function on  an  item which 
*            does not correspond to a single bit. 
*            
*            The what() methods reveals further explanations.
*
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/

class IllegalOperationException : public HardwareAccessException {
public:
  IllegalOperationException( const std::string message, 
			     const std::string module,
			     const int line,
			     const std::string function ) :
    HardwareAccessException( "IllegalOperationException", 
			     message, 
			     module,
			     line, 
			     function ) {};

  IllegalOperationException( const std::string message,
			     const std::string module,
			     const int line,
			     const std::string function,
			     xcept::Exception& previous ) :
    HardwareAccessException( "IllegalOperationException",
			     message, 
			     module,
			     line, 
			     function, 
			     previous ) {};
};

} /* namespace HAL */

#endif /* __IllegalOperationException */
