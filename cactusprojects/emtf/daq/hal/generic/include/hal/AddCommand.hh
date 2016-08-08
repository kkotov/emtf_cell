#ifndef __ResetBitCommand
#define __ResetBitCommand

#include <string>
#include "hal/SequencerCommand.hh"
#include "hal/CommandSequence.hh"
#include "hal/HardwareDeviceInterface.hh"

namespace HAL {

/**
*
*
*     @short Adding a constant to a varible.
*            
*            Syntax in the Sequence:  Add $varname value. Value can be
*            a constant or a variable itself.
*            
*            Examples:
*            
*                   add $myvar 0x5
*            
*                   add $myvar $yourvar
*            
*       @see AddFactory
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/

class AddCommand : public SequencerCommand {
public:
  AddCommand( std::string variable,
              CommandSequence& sequence )
    throw (SequencerSyntaxError) ;

  virtual ~AddCommand() {};

  void excecute( const HardwareDeviceInterface& device ) const
    throw();

  void setOp( long constant );

  void setOpPointer( long* opPtr );

private:
  long constant;
  long* opPtr;
  CommandSequence& sequence;
  uint32_t* variablePointer;
};

} /* namespace HAL */

#endif /* __AddCommand */

