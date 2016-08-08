#ifndef __ReadFactory
#define __ReadFactory

#include <string>
#include <list>

#include "hal/AbstractCommandFactory.hh" 
#include "hal/ReadCommand.hh"
#include "hal/NoSuchItemException.hh"

namespace HAL {

/**
*
*
*     @short The class constructs Read Commands.
*            
*            See ReadCommand for the syntax of the command.
*
*       @see ReadCommand
*    @author Christoph Schwick
* $Revision: 1.2 $
*     $Date: 2009/01/14 18:10:06 $
*
*
**/

class ReadFactory : public AbstractCommandFactory {
public:
  virtual ~ReadFactory() {};
  SequencerCommand* create( std::list<std::string>& arguments, 
                            const AddressTableInterface& addressTable,
                            CommandSequence& sequence ) 
    throw (SequencerSyntaxError, NoSuchItemException);

};

} /* namespace HAL */

#endif /* __ReadFactory */
