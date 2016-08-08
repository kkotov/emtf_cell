#ifndef __NoSuchDeviceException
#define __NoSuchDeviceException

#include <string>
#include "hal/HardwareAccessException.hh"

namespace HAL {

/**
*
*
*     @short Indicates a wrong value used in the program.
*            
*            This Exception  is thrown in  the constructor of  the PCI 
*            device if the hardware device cannot be found.
*
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class NoSuchDeviceException : public HardwareAccessException {
public:
  NoSuchDeviceException( const std::string message, 
			 const std::string module,
			 const int line,
			 const std::string function ) :
    HardwareAccessException( "NoSuchDeviceException", 
			     message, 
			     module,
			     line, 
			     function ) {};

  NoSuchDeviceException( const std::string message,
			 const std::string module,
			 const int line,
			 const std::string function,
			     xcept::Exception& previous ) :
    HardwareAccessException( "NoSuchDeviceException",
			     message, 
			     module,
			     line, 
			     function, 
			     previous ) {};
};

} /* namespace HAL */

#endif /* __NoSuchDeviceException */


