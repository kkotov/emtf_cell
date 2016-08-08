#ifndef __CommandSequencer
#define __CommandSequencer

#include <string>
#include <vector>
#include <tr1/unordered_map>

#include "hal/CommandSequence.hh"
#include "hal/HardwareDeviceInterface.hh"
#include "hal/IllegalValueException.hh"
#include "hal/SequencerSyntaxError.hh"
#include "hal/BusAdapterException.hh"
#include "hal/TimeoutException.hh"

namespace HAL {

class CommandSequencer {
/**
*
*
*     @short A class to excecute sequences of hardware access commands.
*            
*            The Sequencer stores a  number of command sequences which
*            can   be   excecuted   at   any   time.   Look   at   the 
*            CommandSequence  documentation to  learn more  about the 
*            sequences themselves.
*
*       @see CommandSequence
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/
public:

  virtual ~CommandSequencer() {}

  /**
   * Register a reference to a new Sequence.
   * The  internal  data container  does  NOT  hold  a copy  to  the
   * sequence  but  only  stores  a  pointer to  it.  Therefore  the
   * Sequence must  not be destroyed before  the CommandSequencer is
   * destroyed since only  a pointer to the sequence  is stored. But
   * the  caller stays  the  owner of  the  sequence !  (i.e. he  is
   * responsible for deleting sequences if necessary). But look at the
   * deleteSequences method, too! (There the user can explicitly 
   * delete all registered sequences.) The function is virtual in case
   * a user wants to derive from this class and overwrite the function.
   */
  virtual void registerSequence( CommandSequence& sequence );

  /**
   * Run a sequence on an explicit hardware device.
   * The device is not stored  with the sequence itself because then
   * a sequence  could not  be shared among  several devices  of the
   * same kind.
   */
  void run(std::string sequenceName, 
	   const HardwareDeviceInterface& device) const 
    throw (IllegalValueException, 
           SequencerSyntaxError,
           IllegalOperationException,
           BusAdapterException,
           MaskBoundaryException,
           TimeoutException );

  /**
   * Return a vector of registered sequence names.
   */
  std::vector<std::string> getNameVector() const;

  /**
   * Deletes a sequences registered with the Sequencer and erases
   * it from the internal data structures. It is virtual in case 
   * a user wants to derive from this class and overwrite the method.
   */
  virtual void deleteSequence( std::string sequenceName )
    throw( IllegalValueException );

  /**
   * Deletes all Sequences registered with the Sequencer and erases
   * them from the internal data structures. It is virtual in case 
   * a user wants to derive from this class.
   */
  virtual void deleteSequences() ;

  /**
   * Rescan the specific sequence.
   */
  void rescan( std::string sequenceName )
    throw (IllegalValueException,
           NoSuchItemException,
           SequencerSyntaxError);

  /**
   * Get a Sequence if it exists.
   * This function can be useful if access to the variables
   * of the sequence is needed.
   */
  CommandSequence& getSequence( std::string sequenceName )
    throw( IllegalValueException );

private:
  std::tr1::unordered_map<std::string, CommandSequence*> sequenceMap;
};

} /* namespace HAL */

#endif /* __CommandSequencer */
