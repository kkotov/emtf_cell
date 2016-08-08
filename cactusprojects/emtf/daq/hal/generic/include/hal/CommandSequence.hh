#ifndef __CommandSequence
#define __CommandSequence

#include <string>
#include <tr1/unordered_map>
#include <vector>

#include "hal/CommandSequenceReader.hh"
#include "hal/HardwareDeviceInterface.hh"
#include "hal/SequencerCommand.hh"
#include "hal/CommandCreator.hh"
#include "hal/AddressTableInterface.hh"
#include "hal/SequencerSyntaxError.hh"
#include "hal/NoSuchItemException.hh"
#include "hal/BusAdapterException.hh"
#include "hal/TimeoutException.hh"

namespace HAL {

/**
*
*
*     @short A series of commands which can be excecuted at once.
*            
*            The Command  sequence is similar  to a poor  mans script.
*            It  is able  to  store and  later  excecute sequences  of
*            hardware access commands. Which commands are available is
*            described in the CommandCreator.  (The system is designed
*            to easily be extendable for new commands.)
*            
*            Please note that commands in sequences are CASE SENSITIVE.
*
*       @see CommandCreator
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/

class CommandSequence {
public :
  /**
   * Build a command sequence.
   * The sequence  is built by  reading all commands from the reader
   * and  storing them  for  later excecution  in  an internal  data
   * structure.   The reader only delivers  command names  and their
   * parameters. The constructor uses the CommandCreator in order to
   * create for each command  an object (SequencerCommand) and store
   * it internally in a list.  The CommandSequence aquires ownership
   * of  the  commands.  This  means  it is  responsible  for  their
   * destruction. On the other hand it is important to note that the
   * CommandSequence  only keeps  references to the AddressTable and
   * to the CommandSequenceReader. Therefore these two must exist at
   * least as long as the CommandSequencer. It is the responsibility
   * of the user  to ensure  this. (The technical  reason why  these
   * two items are not passed by value so that a local copy is made,
   * is that both items must be polymorphic types: for example the 
   * addressTable could be a VME or a PCIAddressTable.) 
   * @param name is an arbitray name for the sequence.
   * @param reader is a "Reader" for the CommandSequence. It is possible
   *        to read CommandSequences from different sources (e.g. ASCII
   *        files, XML-representations, databases).
   * @param addressTable refers to the addressTable which is used by
   *        this CommandSequence. 
   */
  CommandSequence( std::string name,
		   CommandSequenceReader& reader,
		   const AddressTableInterface& addressTable )
    throw( SequencerSyntaxError, NoSuchItemException );

  /**
   * The Destructor deletes all commands of this CommandSequence.
   */
  virtual ~CommandSequence();

  /**
   * Run the CommandSequence on a specific device.
   */
  void run( const HardwareDeviceInterface& device )
    throw( SequencerSyntaxError,
           IllegalOperationException,
           BusAdapterException,
           MaskBoundaryException,
           TimeoutException);

  std::string getName() const { return name; }

  /**
   * This is used by the constructor of the DefineCommand. A new 
   * variable is registered with the sequence in this way. 
   */
  uint32_t* addVariable( std::string name, 
                              uint32_t initValue = 0 );

  /**
   * This is used by the constructor of the LabelCommand. A new
   * label is registered with the sequence in this way.
   */
  void addLabel( std::string name );

  /**
   * Retrieves the value of a variable registered with this sequence.
   * @throws SequencerSyntaxError is thrown if the variable does not 
   *         exist in this sequence.
   */
  uint32_t getVariable( std::string name ) const
    throw( SequencerSyntaxError );
  
  /**
   * Retrieves the pointer to a specific variale. This is used by the 
   * ReadCommand and UmaskedReadCommand. 
   * @throws SequencerSyntaxError is thrown if the variable does not 
   *         exist in this sequence.
   */
  uint32_t* getVariablePointer( std::string name )
    throw( SequencerSyntaxError );

  /**
   * Sets a variable to specific value.
   * This command can be used from within the application to set 
   * specific parameters in a sequence before excecuting them. 
   * @throws SequencerSyntaxError is thrown if the variable does not 
   *         exist in this sequence.
   */
  void setVariable( std::string name,
                    uint32_t value) 
    throw( SequencerSyntaxError );
  /**
   * Checks if a variable is existing in this sequence. 
   * @throws SequencerSyntaxError is thrown if the variable does not 
   *         exist in this sequence.
   */
  void existVariable( std::string name ) const
    throw( SequencerSyntaxError );

  /**
   * Excecution of a sequence is continued at a specific label. 
   * This is used by the GotoCommand. 
   * @throws SequencerSyntaxError is thrown if the label does not 
   *         exist in this sequence.
   */
  void gotoLabel( std::string labelName )
    throw (SequencerSyntaxError );

  /**
   * This function re-reads the whole sequence from the reader given 
   * in the constructor. It allows you to change sequences while the 
   * program is running. For example if you have a menu driven program
   * and one menu point is to excecute a specific sequence, you can 
   * change the sequence while your program is running. If you rescan 
   * the sequence always before excecution you can try out your new 
   * version of the sequence without restarting the program. 
   */
  void rescan() 
    throw( NoSuchItemException,
           SequencerSyntaxError );

  /**
   * Checks the existance of a specific label in the sequence. 
   * @throws SequencerSyntaxError is thrown if the label does not 
   *         exist in this sequence.
   */
  void existLabel( std::string labelName ) const
    throw ( SequencerSyntaxError );

private:
  std::string name;
  CommandSequenceReader& reader;
  const AddressTableInterface& addressTable;
  int commandIndex;
  std::vector< SequencerCommand* > commandList;
  std::vector< SequencerCommand* >::iterator commandIterator;
  std::tr1::unordered_map< std::string, uint32_t > variableMap;
  std::tr1::unordered_map< std::string, int > labelMap;
};

} /* namespace HAL */

#endif /* __CommandSequence */
