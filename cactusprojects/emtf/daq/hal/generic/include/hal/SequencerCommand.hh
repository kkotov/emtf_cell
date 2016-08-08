#ifndef __SequencerCommand
#define __SequencerCommand

#include "hal/HardwareDeviceInterface.hh"
#include "hal/SequencerSyntaxError.hh"
#include "hal/BusAdapterException.hh"
#include "hal/MaskBoundaryException.hh"

namespace HAL {

/**
*
*
*     @short Interface for the a command for the CommandSequencer.
*
*    @author Christoph Schwick
* $Revision: 1.3 $
*     $Date: 2009/04/29 10:23:07 $
*
*
**/

class SequencerCommand {
public:


  virtual ~SequencerCommand() {};

  /**
   * This routine is common to all Sequencer commands. It does what 
   * it pretends to do.
   */
  virtual void excecute( const HardwareDeviceInterface& device ) const
    throw (BusAdapterException,
           SequencerSyntaxError,
           IllegalOperationException,
           MaskBoundaryException,
           TimeoutException ) = 0;

};

} /* namespace HAL */

#endif /* __SequencerCommand */
