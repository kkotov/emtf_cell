#ifndef __CommandCreator
#define __CommandCreator

#include <list>
#include <string>
#include <tr1/unordered_map>

#include "hal/SequencerCommand.hh"
#include "hal/AbstractCommandFactory.hh"
#include "hal/SequencerSyntaxError.hh"
#include "hal/NoSuchItemException.hh"
#include "hal/AddressTableInterface.hh"

// 12 Commands
#include "hal/WriteFactory.hh"
#include "hal/UnmaskedWriteFactory.hh"
#include "hal/SetBitFactory.hh"
#include "hal/ResetBitFactory.hh"
#include "hal/DefineFactory.hh"
#include "hal/AddFactory.hh"
#include "hal/ReadFactory.hh"
#include "hal/UnmaskedReadFactory.hh"
#include "hal/LabelFactory.hh"
#include "hal/GotoFactory.hh"
#include "hal/PrintFactory.hh"
#include "hal/CheckFactory.hh"
#include "hal/PollItemFactory.hh"

namespace HAL {

class CommandSequence;

/**
*
*
*     @short A class to construct SequencerCommand Objects.
*
*            The CommandCreator  "knows" which commands  are available
*            in  the system.  It can  create  SequencerCommand Objects
*            from  strings   containing  the  command   name  and  the
*            parameters.  Internally  it contains a  map with abstract
*            command-factories for each  available command.  These are
*            used to create the commands. 
*            
*            Note that commands in sequences are CASE SENSITIVE !
*            
*            Available   commands  are:  read,   unmaskedRead,  write, 
*            unmaskedWrite,  setBit,  resetBit,  define,  label,  add, 
*            goto, print, check.
*
*       @see ReadCommand UnmaskedReadCommand WriteCommand 
*            UnmaskedWriteCommand SetBitCommand ResetBitCommand 
*            DefineCommand LabelCommand AddCommand GotoCommand 
*            PrintCommand CheckCommand
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/

class CommandCreator {
public:
  /**
   * The construtor sets up the internal map of command factories.
   * Here it is hardcoed which commands are available for the system.
   * If a new command is added to the system, this constructor has to
   * be changed. 
   */
  CommandCreator();
  virtual ~CommandCreator();

  /**
   * Create an instantiation of a specific SequencerCommand.
   * @param commandStringList specifies the SequencerCommand to be 
   *        instantiated. The first string in the list is the command
   *        name followed by it's parameters. 
   * @param addressTable is forwarded to the constructor of the command.
   * @param sequence is forwarded to the contructor of the command.
   */
  SequencerCommand* create( std::list<std::string> commandStringList,
			    const AddressTableInterface& addressTable,
			    CommandSequence& sequence) 
    throw (SequencerSyntaxError, NoSuchItemException);
  
private: 
  std::tr1::unordered_map< std::string, AbstractCommandFactory* > commandFactoryMap;
};

} /* namespace HAL */

#endif /* __CommandCreator */
