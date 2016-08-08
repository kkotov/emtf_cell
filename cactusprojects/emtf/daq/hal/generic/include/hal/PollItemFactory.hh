#ifndef __PollItemFactory
#define __PollItemFactory

#include <string>
#include <list>

#include "hal/AbstractCommandFactory.hh" 
#include "hal/PollItemCommand.hh"
#include "hal/NoSuchItemException.hh"

namespace HAL {

/**
*
*
*     @short The class constructs PollItem Commands.
*            
*            This class  parses the syntax for  the PollItem-command. For 
*            the detailed syntax see PollItemCommand.
*            
*
*       @see PollItemCommand
*    @author Christoph Schwick
* $Revision: 1.2 $
*     $Date: 2009/01/14 18:10:06 $
*
*
**/

class PollItemFactory : public AbstractCommandFactory {
public:
  virtual ~PollItemFactory() {};
  SequencerCommand* create( std::list<std::string>& arguments, 
                            const AddressTableInterface& addressTable,
                            CommandSequence& sequence ) 
    throw (SequencerSyntaxError, NoSuchItemException);
  
};

} /* namespace HAL */

#endif /* __PollItemFactory */
