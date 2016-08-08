#ifndef __CheckFactory
#define __CheckFactory

#include <list>
#include <string>

#include "hal/AbstractCommandFactory.hh" 
#include "hal/CheckCommand.hh"
#include "hal/NoSuchItemException.hh"

namespace HAL {

/**
*
*
*     @short The class constructs Check Commands.
*            
*            See CheckCommand for the syntax of the command.
*
*       @see CheckCommand
*    @author Christoph Schwick
* $Revision: 1.2 $
*     $Date: 2009/01/14 18:10:06 $
*
*
**/

class CheckFactory : public AbstractCommandFactory {
public:
  virtual ~CheckFactory() {};
  SequencerCommand* create( std::list<std::string>& arguments, 
                            const AddressTableInterface& addressTable,
                            CommandSequence& sequence ) 
    throw (SequencerSyntaxError, NoSuchItemException);

};

} /* namespace HAL */

#endif /* __CheckFactory */
