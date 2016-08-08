#ifndef __SetBitFactory
#define __SetBitFactory

#include <string>
#include <list>

#include "hal/AbstractCommandFactory.hh" 
#include "hal/SetBitCommand.hh"
#include "hal/NoSuchItemException.hh"

namespace HAL {

/**
*
*
*     @short The class constructs setBit Commands.
*            
*            This class  parses the syntax for  the setBit-command. For 
*            the detailed syntax see SetBitCommand.
*
*       @see SetBitCommand
*    @author Christoph Schwick
* $Revision: 1.2 $
*     $Date: 2009/01/14 18:10:06 $
*
*
**/

class SetBitFactory : public AbstractCommandFactory {
public:
  virtual ~SetBitFactory() {};
  SequencerCommand* create( std::list<std::string>& arguments, 
                            const AddressTableInterface& addressTable,
                            CommandSequence& sequence ) 
    throw (SequencerSyntaxError, NoSuchItemException);

};

} /* namespace HAL */

#endif /* __SetBitFactory */
