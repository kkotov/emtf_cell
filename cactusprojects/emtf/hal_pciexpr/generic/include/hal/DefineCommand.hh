#ifndef __DefineCommand
#define __DefineCommand

#include <string>

#include "hal/SequencerCommand.hh"
#include "hal/CommandSequence.hh"
#include "hal/HardwareDeviceInterface.hh"

namespace HAL {

/**
*
*
*     @short SequencerCommand to define a new Variable. 
*            
*            Variable name must start with a "$".
*            The syntax is "define $name [initValue]";
*            
*            If the initValue is omitted the variable initialized to 0
*            BEFORE  THE  SEQUENCE IS  EXCECUTED  FOR  THE FIRST  TIME
*            (i.e. at creation time  of the sequence). This means that
*            if the sequence is excecuted  a second time, the value of
*            the variable  is the same  as the one  at the end  of the
*            previous excecution of the sequence (if not a program has
*            changed its  value in  the meantime with  the setVariable
*            method of  the CommandSequencer).  If  initValue is given
*            the Variable  is initialized to this value  EACH TIME THE
*            SEQUENCE IS EXCECUTED
*
*       @see DefineFactory
*            
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/

class DefineCommand : public SequencerCommand {
public:
  /**
   */
  DefineCommand( std::string variableName,
		 bool hasInitialization,
		 CommandSequence& sequence );
  
  virtual ~DefineCommand() {};

  /**
   * Used by the DefineFactory.
   */
  void setInitValuePointer( uint32_t* initValPtr );
  /**
   * Used by the DefineFactory.
   */
  void setInitValue( uint32_t initVal );
  void excecute( const HardwareDeviceInterface& device ) const
    throw();

private:
  bool hasInitialization;
  uint32_t initValue;
  uint32_t* initValuePtr;
  std::string variableName;
  CommandSequence& sequence;
  uint32_t* variablePtr;
};

} /* namespace HAL */

#endif /* __DefineCommand */
