#ifndef __PrintCommand
#define __PrintCommand

#include <string>
#include <list>

#include "hal/SequencerCommand.hh"
#include "hal/CommandSequence.hh"
#include "hal/HardwareDeviceInterface.hh"

namespace HAL {

/**
*
*
*     @short SequencerCommand to print something on the console. 
*            
*            The syntax is "print  {string}". Any word starting with a
*            "$" sign is interpreted as  a variable name and the value
*            of the variable is  printed.  There are two special which
*            define  the  print  format:  %dec  prints  the  following
*            variables  in  decimal  format  (defaults)  whereas  %hex
*            prints them in hexadecimal format. Examples:
*            
*            print My variable in hex is %hex $myVar
*            
*            print My variable in dec is %dec $myVar
*                        
*       @see PrintFactory
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class PrintCommand : public SequencerCommand {
public:
  PrintCommand( std::list<std::string> arguments,
                CommandSequence& sequence )
    throw (SequencerSyntaxError);
  
  virtual ~PrintCommand() {};
  
  void excecute( const HardwareDeviceInterface& device ) const
    throw();
  
private:
  std::list<std::string> arguments;
  CommandSequence& sequence;
};

} /* namespace HAL */

#endif /* __PrintCommand */
