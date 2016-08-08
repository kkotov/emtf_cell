#ifndef __UnmaskedReadFactory
#define __UnmaskedReadFactory

#include <string>
#include <list>

#include "hal/AbstractCommandFactory.hh" 
#include "hal/UnmaskedReadCommand.hh"
#include "hal/NoSuchItemException.hh"

namespace HAL {

/**
*
*
*     @short The class constructs UnmaskedRead Commands.
*            
*            This class  parses the syntax for  the UnmaskedRead-command. For 
*            the detailed syntax of the command see UnmaskedReadCommand.
*
*       @see UnmaskedReadCommand
*    @author Christoph Schwick
* $Revision: 1.2 $
*     $Date: 2009/01/14 18:10:06 $
*
*
**/

class UnmaskedReadFactory : public AbstractCommandFactory {
public:
  virtual ~UnmaskedReadFactory() {};
  SequencerCommand* create( std::list<std::string>& arguments, 
                            const AddressTableInterface& addressTable,
                            CommandSequence& sequence ) 
    throw (SequencerSyntaxError, NoSuchItemException);

};

} /* namespace HAL */

#endif /* __UnmaskedReadFactory */
