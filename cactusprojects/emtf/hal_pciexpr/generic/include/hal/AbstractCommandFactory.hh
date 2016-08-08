#ifndef __AbstractCommandFactory
#define __AbstractCommandFactory

#include <list>
#include <string>

#include "hal/SequencerCommand.hh"
#include "hal/AddressTableInterface.hh"
#include "hal/SequencerSyntaxError.hh"
#include "hal/NoSuchItemException.hh"

namespace HAL {

/**
*
*
*     @short Defines the interface for instantiating commands. 
*            
*            This interface is used  by the CommandCreator in order to
*            create the  commands of  a sequence. The  Factory pattern 
*            has been  chosen since in  principle different activities 
*            are necessary in order  to create the different commands. 
*            
*            The AbstractCommandFactory  contains the code  to convert 
*            a constant  argument-string to a  number. Constant values 
*            are   accepted  in  two   formats:  decimal   values  or 
*            hexadecimal values: 7823 or 0x2af8.
*            
*       @see CommandCreator
*
*    @author Christoph Schwick
* $Revision: 1.3 $
*     $Date: 2009/04/29 10:23:07 $
*
*
**/

class CommandSequence;

class AbstractCommandFactory {

public: 

  virtual ~AbstractCommandFactory() {};

  /**
   * Create an instantiation of a specific SequencerCommand.
   * @param addressTable might be used in the implementations of this.
   *        interface.
   * @param sequence is forwarded to the contructor of the command.
   */
  virtual SequencerCommand* create( std::list<std::string>& arguments, 
				    const AddressTableInterface& addressTable,
				    CommandSequence& sequence ) 
    throw (SequencerSyntaxError, NoSuchItemException) = 0;


protected:
  /**
   * A helper function to convert constant parameters.
   * Often parameters are numbers which can be given in hexadecimal 
   * notation (e.g. "0xfe") or in decimal notation (e.g. "254"). This
   * function converts the strings apropriately to a number.
   */
  uint32_t stringToNumber( std::string numberString ) 
    throw (SequencerSyntaxError);

};

} /* namespace HAL */

#endif /* __AbstractCommandFactory */

