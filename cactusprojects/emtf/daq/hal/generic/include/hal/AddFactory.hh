#ifndef __AddFactory
#define __AddFactory

#include "hal/AbstractCommandFactory.hh" 
#include "hal/AddCommand.hh"
#include "hal/NoSuchItemException.hh"

#include <string>
#include <list>

namespace HAL {

/**
*
*
*     @short The class constructs Add Commands.
*
*            For the syntax see AddCommand.
*            
*       @see AddCommand
*    @author Christoph Schwick
* $Revision: 1.2 $
*     $Date: 2009/01/14 18:10:06 $
*
*
**/

class AddFactory : public AbstractCommandFactory {
public:
  virtual ~AddFactory() {};
  /**
   * @param arguments must contain two items: the Variable Name and 
   *        the value to be added (constant or variable name)
   * @param addressTable is used to check if the item given in the 
   *        list of arguments is existing. 
   * @throw SequencerSyntaxError is thrown when item does not exist.
   */
  SequencerCommand* create( std::list<std::string>& arguments, 
                            const AddressTableInterface& addressTable,
                            CommandSequence& sequence ) 
    throw (SequencerSyntaxError);
};

} /* namespace HAL */

#endif /* __AddFactory */
