#ifndef __MaskBoundaryException
#define __MaskBoundaryException

#include <string>
#include "hal/HardwareAccessException.hh"

namespace HAL {

/**
*
*
*     @short Indicates that a value has bits set which are outside
*            of a specified mask.
*
*            This Exception is thrown if a value is checked against
*            a mask and if set bits are detected outside of the given
*            mask.
*
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class MaskBoundaryException : public HardwareAccessException {
public:
  MaskBoundaryException( const std::string message, 
			 const std::string module,
			 const int line,
			 const std::string function ) :
    HardwareAccessException( "MaskBoundaryException", 
			     message, 
			     module,
			     line, 
			     function ) {};

  MaskBoundaryException( const std::string message,
			 const std::string module,
			 const int line,
			 const std::string function,
			     xcept::Exception& previous ) :
    HardwareAccessException( "MaskBoundaryException",
			     message, 
			     module,
			     line, 
			     function, 
			     previous ) {};
};

} /* namespace HAL */

#endif /* __MaskBoundaryException */
