#ifndef __LabelFactory
#define __LabelFactory

#include <string>
#include <list>

#include "hal/AbstractCommandFactory.hh" 
#include "hal/LabelCommand.hh"
#include "hal/NoSuchItemException.hh"

namespace HAL {

/**
*
*
*     @short The class constructs Label Commands.
*            
*            For the syntax see LabelCommand.
*
*       @see LabelCommand
*    @author Christoph Schwick
* $Revision: 1.2 $
*     $Date: 2009/01/14 18:10:06 $
*
*
**/

class LabelFactory : public AbstractCommandFactory {
public:
  virtual ~LabelFactory() {};
  SequencerCommand* create( std::list<std::string>& arguments, 
                            const AddressTableInterface& addressTable,
                            CommandSequence& sequence ) 
    throw (SequencerSyntaxError);
};

} /* namespace HAL */

#endif /* __LabelFactory */
