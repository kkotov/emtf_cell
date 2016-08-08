#ifndef __HardwareProblemException
#define __HardwareProblemException

#include <string>

#include "hal/HardwareAccessException.hh"

namespace HAL {

/**
*
*
*     @short Indicates a problem with the hardware.
*
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/

class HardwareProblemException : public HardwareAccessException {
public:
  ~HardwareProblemException() throw() {};
  HardwareProblemException() 
    : name("HardwareProblemException" ) {
      whatString = "(HardwareProblemException) There is a problem with the hardware."; 
  }

  HardwareProblemException( std::string text ) 
    : name( "HardwareProblemException"),  
      whatString( "(" + name + ") " + text ) {
  }

  const char *what() const throw() {
    return whatString.c_str();
  }

private :
  std::string name;
  std::string whatString ;
};

} /* namespace HAL */

#endif /* __HardwareProblemException */
