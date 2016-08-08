#ifndef __ResetBitFactory
#define __ResetBitFactory

#include <string>
#include <list>

#include "hal/AbstractCommandFactory.hh" 
#include "hal/ResetBitCommand.hh"
#include "hal/NoSuchItemException.hh"

namespace HAL {

/**
*
*
*     @short The class constructs resetBit Commands.
*            
*            This class  parses the syntax for  the resetBit-command. For 
*            the detailed syntax see ResetBitCommand.
*
*       @see ResetBitCommand
*    @author Christoph Schwick
* $Revision: 1.2 $
*     $Date: 2009/01/14 18:10:06 $
*
*
**/

class ResetBitFactory : public AbstractCommandFactory {
public:
  virtual ~ResetBitFactory() {};
  SequencerCommand* create( std::list<std::string>& arguments, 
			    const AddressTableInterface& addressTable,
			    CommandSequence& sequence ) 
    throw (SequencerSyntaxError, NoSuchItemException);

};

} /* namespace HAL */

#endif /* __ResetBitFactory */
