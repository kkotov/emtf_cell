#ifndef __VerifyException
#define __VerifyException

#include <string>
#include "hal/HardwareAccessException.hh"

namespace HAL {

/**
*
*
*     @short Indicates  an  Error  during  verification of  a  written 
*            value. 
* 
*            For   the  operations   "write",  "unmasked   write"  and 
*            "setBit" there  is the possibility  to set a  verify flag 
*            in the command. If set the operations read back the value
*            which has  been previously written, and if  the result is 
*            not the  same as the orgiginally written  data word, this 
*            exception is thrown.
*
*            The what() methods reveals further explanations.
*
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class VerifyException : public HardwareAccessException {
public:
  VerifyException( const std::string message, 
		   const std::string module,
		   const int line,
		   const std::string function ) :
    HardwareAccessException( "VerifyException", 
			     message, 
			     module,
			     line, 
			     function ) {};

  VerifyException( const std::string message,
		   const std::string module,
		   const int line,
		   const std::string function,
			     xcept::Exception& previous ) :
    HardwareAccessException( "VerifyException",
			     message, 
			     module,
			     line, 
			     function, 
			     previous ) {};
};

} /* namespace HAL */

#endif /* __VerifyException */
