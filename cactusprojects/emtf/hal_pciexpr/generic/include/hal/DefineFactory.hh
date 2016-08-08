#ifndef __DefineFactory
#define __DefineFactory

#include <string>
#include <list>

#include "hal/AbstractCommandFactory.hh" 
#include "hal/DefineCommand.hh"
#include "hal/NoSuchItemException.hh"

namespace HAL {

/**
*
*
*     @short The class constructs Define Commands.
*            
*            For the syntax see DefineCommand.
*
*       @see DefineCommand
*
*    @author Christoph Schwick
* $Revision: 1.2 $
*     $Date: 2009/01/14 18:10:06 $
*
*
**/

class DefineFactory : public AbstractCommandFactory {
public:
  virtual ~DefineFactory() {};
  SequencerCommand* create( std::list<std::string>& arguments, 
                            const AddressTableInterface& addressTable,
                            CommandSequence& sequence ) 
    throw (SequencerSyntaxError);
};

} /* namespace HAL */

#endif /* __DefineFactory */
