#ifndef __NoSuchFileException
#define __NoSuchFileException

#include <string>

#include "hal/HardwareAccessException.hh"

namespace HAL {

/**
*
*
*     @short Indicates a wrong value used in the program.
*            
*            As   an  example   this  Exception   is  thrown   in  the
*            constructors  of  the  PCIAddressTableASCIIReader and  the
*            VMEAddressTableASCIIReader  if the  input  file cannot  be
*            found.
*
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class NoSuchFileException : public HardwareAccessException {
public:
  NoSuchFileException( const std::string message, 
		       const std::string module,
		       const int line,
		       const std::string function ) :
    HardwareAccessException( "NoSuchFileException", 
			     message, 
			     module,
			     line, 
			     function ) {};

  NoSuchFileException( const std::string message,
		       const std::string module,
		       const int line,
		       const std::string function,
			     xcept::Exception& previous ) :
    HardwareAccessException( "NoSuchFileException",
			     message, 
			     module,
			     line, 
			     function, 
			     previous ) {};
};

} /* namespace HAL */

#endif /* __NoSuchFileException */
