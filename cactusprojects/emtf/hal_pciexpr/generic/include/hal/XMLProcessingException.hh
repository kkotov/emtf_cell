#ifndef __XMLProcessingException
#define __XMLProcessingException

#include <string>
#include "hal/HardwareAccessException.hh"

namespace HAL {

/**
*
*
*     @short Indicates a problem during processing of a XML Document.
*
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:11 $
*
*
**/

class XMLProcessingException : public HardwareAccessException {
public:
  XMLProcessingException( const std::string message, 
			  const std::string module,
			  const int line,
			  const std::string function ) :
    HardwareAccessException( "XMLProcessingExceptionException", 
			     message, 
			     module,
			     line, 
			     function ) {};

  XMLProcessingException( const std::string message,
			  const std::string module,
			  const int line,
			  const std::string function,
			  xcept::Exception& previous ) :
    HardwareAccessException( "XMLProcessingExceptionException",
			     message, 
			     module,
			     line, 
			     function, 
			     previous ) {};
};

} /* namespace HAL */

#endif /* __XMLProcessingException */
