#ifndef __SequencerSyntaxError
#define __SequencerSyntaxError

#include <string>
#include "hal/HardwareAccessException.hh"

namespace HAL {

/**
*
*
*     @short Indicates a syntax error in a CommandSequence. 
*
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class SequencerSyntaxError : public HardwareAccessException {
public:
  ~SequencerSyntaxError() throw() {};
  SequencerSyntaxError() 
    : name( "SequencerSyntaxError") {    
      whatString = "(SequencerSyntaxError) Syntax Error in Command Sequence."; 
  }

  SequencerSyntaxError( std::string text ) 
    : name( "SequencerSyntaxError"),  
      whatString( "(" + name + ") " + text ) {
  }

  const char *what() const throw() {
    return whatString.c_str();
  }

private :
  std::string name;
  std::string whatString;
};

} /* namespace HAL */

#endif /* __SequencerSyntaxError */
