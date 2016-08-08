#ifndef __LabelCommand
#define __LabelCommand

#include <string>

#include "hal/SequencerCommand.hh"
#include "hal/CommandSequence.hh"
#include "hal/HardwareDeviceInterface.hh"

namespace HAL {

/**
*
*
*     @short SequencerCommand to define a new Label. 
*            
*            The syntax is "label {name}". The name can be any string.
*            This command defines a jump-target for the Goto command.
*
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/

class LabelCommand : public SequencerCommand {
public:
  LabelCommand( std::string labelName,
                CommandSequence& sequence );
  
  virtual ~LabelCommand() {};

  void excecute( const HardwareDeviceInterface& device ) const
    throw();

private:
  std::string labelName;
  CommandSequence& sequence;
};

} /* namespace HAL */

#endif /* __LabelCommand */
