#ifndef __GotoFactory
#define __GotoFactory

#include <string>
#include <list>

#include "hal/AbstractCommandFactory.hh" 
#include "hal/GotoCommand.hh"
#include "hal/NoSuchItemException.hh"

namespace HAL {

/**
*
*
*     @short The class constructs Goto Commands.
*            
*            For the syntax see GotoCommand.
*            
*       @see GotoCommand
*    @author Christoph Schwick
* $Revision: 1.2 $
*     $Date: 2009/01/14 18:10:06 $
*
*
**/

class GotoFactory : public AbstractCommandFactory {
public:
  virtual ~GotoFactory() {};
  SequencerCommand* create( std::list<std::string>& arguments, 
                            const AddressTableInterface& addressTable,
                            CommandSequence& sequence ) 
    throw (SequencerSyntaxError);
};

} /* namespace HAL */

#endif /* __GotoFactory */
