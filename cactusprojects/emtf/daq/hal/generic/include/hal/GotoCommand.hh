#ifndef __GotoCommand
#define __GotoCommand

#include <string>

#include "hal/SequencerCommand.hh"
#include "hal/CommandSequence.hh"
#include "hal/HardwareDeviceInterface.hh"

namespace HAL {

/**
*
*
*     @short SequencerCommand to define a new Goto. 
*            
*            The syntax is "goto {labelName} op1 cond op2".
*            
*            The jump is excecuted if  the condition "op1 cond op2" is
*            satisfied. Possible conditions are: "< > == <= >= !=".
*            
*       @see GotoFactory
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/

class GotoCommand : public SequencerCommand {
public:
  GotoCommand( std::string labelName,
               std::string condition,
               CommandSequence& sequence )
    throw (SequencerSyntaxError);
  
  virtual ~GotoCommand() {};
  
  /**
   * Used by the GotoFactory
   */
  void setOp1( uint32_t op1 );

  /**
   * Used by the GotoFactory
   */
  void setOp2( uint32_t op2 );

  /**
   * Used by the GotoFactory
   */
  void setOp1Pointer( uint32_t* op1Ptr );

  /**
   * Used by the GotoFactory
   */
  void setOp2Pointer( uint32_t* op2Ptr );

  void excecute( const HardwareDeviceInterface& device ) const
    throw( SequencerSyntaxError );

private:
  std::string labelName;
  std::string condition;
  CommandSequence& sequence;
  uint32_t op1, op2;
  uint32_t* op1Ptr;
  uint32_t* op2Ptr;
};

} /* namespace HAL */

#endif /* __GotoCommand */
