#ifndef __WriteFactory
#define __WriteFactory

#include <string>
#include <list>

#include "hal/AbstractCommandFactory.hh" 
#include "hal/WriteCommand.hh"
#include "hal/NoSuchItemException.hh"

namespace HAL {

/**
*
*
*     @short The class constructs Write Commands.
*            
*            This class  parses the syntax for  the Write-command. For 
*            the detailed syntax see WriteCommand.
*            
*
*       @see WriteCommand
*    @author Christoph Schwick
* $Revision: 1.2 $
*     $Date: 2009/01/14 18:10:06 $
*
*
**/

class WriteFactory : public AbstractCommandFactory {
public:
  virtual ~WriteFactory() {};

  SequencerCommand* create( std::list<std::string>& arguments, 
                            const AddressTableInterface& addressTable,
                            CommandSequence& sequence ) 
    throw (SequencerSyntaxError, NoSuchItemException);

};

} /* namespace HAL */

#endif /* __WriteFactory */
