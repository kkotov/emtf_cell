#ifndef __UnmaskedWriteFactory
#define __UnmaskedWriteFactory

#include <string>
#include <list>

#include "hal/AbstractCommandFactory.hh" 
#include "hal/UnmaskedWriteCommand.hh"
#include "hal/NoSuchItemException.hh"

namespace HAL {

/**
*
*
*     @short The class constructs unmaskedWrite Commands.
*            
*            This  class  parses  the  syntax  for  the  unmaskedWrite
*            command.      For     the     detailed     syntax     see
*            UnmaskedWriteCommand.
*
*       @see UnmaskedWriteCommand
*    @author Christoph Schwick
* $Revision: 1.2 $
*     $Date: 2009/01/14 18:10:06 $
*
*
**/

class UnmaskedWriteFactory : public AbstractCommandFactory {
public:
  virtual ~UnmaskedWriteFactory() {};

  SequencerCommand* create( std::list<std::string>& arguments, 
                            const AddressTableInterface& addressTable,
                            CommandSequence& sequence ) 
    throw (SequencerSyntaxError, NoSuchItemException);
  
};

} /* namespace HAL */

#endif /* __UnmaskedWriteFactory */
