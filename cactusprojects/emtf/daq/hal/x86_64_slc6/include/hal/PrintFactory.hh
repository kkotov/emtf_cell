#ifndef __PrintFactory
#define __PrintFactory

#include <string>
#include <list>

#include "hal/AbstractCommandFactory.hh" 
#include "hal/PrintCommand.hh"
#include "hal/NoSuchItemException.hh"

namespace HAL {

/**
*
*
*     @short The class constructs Print Commands.
*            
*            See PrintCommand for the syntax in the sequence.
*
*       @see PrintCommand
*    @author Christoph Schwick
* $Revision: 1.2 $
*     $Date: 2009/01/14 18:10:06 $
*
*
**/

class PrintFactory : public AbstractCommandFactory {
public:
  virtual ~PrintFactory() {};
  SequencerCommand* create( std::list<std::string>& arguments, 
                            const AddressTableInterface& addressTable,
                            CommandSequence& sequence ) 
    throw (SequencerSyntaxError);
};

} /* namespace HAL */

#endif /* __PrintFactory */
