#ifndef __IllegalValueException
#define __IllegalValueException

#include "hal/HardwareAccessException.hh"
#include <string>

namespace HAL {

/**
*
*
*     @short Indicates a wrong value used in the program.
*            
*            Examples are trying  to access a non existing  BAR in PCI 
*            devices,  using  non  defined  access modes  in  the  PCI 
*            address table.
*
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/

class IllegalValueException : public HardwareAccessException {
public:
  IllegalValueException( const std::string message, 
			 const std::string module,
			 const int line,
			 const std::string function ) :
    HardwareAccessException( "IllegalValueException", 
			     message, 
			     module,
			     line, 
			     function ) {};

  IllegalValueException( const std::string message,
			     const std::string module,
			     const int line,
			     const std::string function,
			     xcept::Exception& previous ) :
    HardwareAccessException( "IllegalValueException",
			     message, 
			     module,
			     line, 
			     function, 
			     previous ) {};
};

} /* namespace HAL */

#endif /* __IllegalValueException */
