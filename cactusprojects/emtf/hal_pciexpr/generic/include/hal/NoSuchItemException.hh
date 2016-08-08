#ifndef __NoSuchItemException
#define __NoSuchItemException

#include <string>

#include "hal/HardwareAccessException.hh"

namespace HAL {

/**
*
*
*     @short Indicates a wrong value used in the program.
*
*            This Exception is thrown if it is tried to access an item
*            which does not exist in the AddressTable.
*
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class NoSuchItemException : public HardwareAccessException {
public:
  NoSuchItemException( const std::string message, 
		       const std::string module,
		       const int line,
		       const std::string function ) :
    HardwareAccessException( "NoSuchItemException", 
			     message, 
			     module,
			     line, 
			     function ) {};

  NoSuchItemException( const std::string message,
		       const std::string module,
		       const int line,
		       const std::string function,
			     xcept::Exception& previous ) :
    HardwareAccessException( "NoSuchItemException",
			     message, 
			     module,
			     line, 
			     function, 
			     previous ) {};
};

} /* namespace HAL */

#endif /* __NoSuchItemException */
