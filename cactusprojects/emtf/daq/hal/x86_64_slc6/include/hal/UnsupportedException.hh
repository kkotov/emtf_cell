#ifndef __UnsupportedException
#define __UnsupportedException

#include <string>
#include "hal/HardwareAccessException.hh"

namespace HAL {

/**
*
*
*     @short Indicates that the call or option is not supported by the 
*            current implementation.
*            
*            This Exception should be  thrown only in rare exceptions. 
*            It  is an  indication for  an uncomplete  implementation.  
*            There is may be an exception to this rule: If BusAdapters
*            cannot  implement a  feature for  technical  reasons they 
*            may    throw   this    exception.   (Example:    if   the 
*            addressBehaviour   in   the   readBlock   or   writeBlock 
*            functions is  set to HAL_NO_INCREMENT  and this behaviour 
*            cannot be implemented in  a specific hardware driver, the
*            corresponding BusAdapter might throw this exception.
*            
*
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class UnsupportedException : public HardwareAccessException {
public:
  UnsupportedException( const std::string message, 
			const std::string module,
			const int line,
			const std::string function ) :
    HardwareAccessException( "UnsupportedException", 
			     message, 
			     module,
			     line, 
			     function ) {};

  UnsupportedException( const std::string message,
			const std::string module,
			const int line,
			const std::string function,
			     xcept::Exception& previous ) :
    HardwareAccessException( "UnsupportedException",
			     message, 
			     module,
			     line, 
			     function, 
			     previous ) {};
};

} /* namespace HAL */

#endif /* __UnsupportedException */
