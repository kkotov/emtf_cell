#ifndef __CheckCommand
#define __CheckCommand

#include <string>
#include <list>

#include "hal/SequencerCommand.hh"
#include "hal/CommandSequence.hh"
#include "hal/HardwareDeviceInterface.hh"
#include "hal/BusAdapterException.hh"

namespace HAL {

/**
*
*
*     @short SequencerCommand to check the value of an item agains an
*            expectation.
*            
*            This  Command  corresponds to  the  check  method in  the 
*            HardwareDeviceInterface. The syntax is :
*            
*            check item expectation [offset] [faultMessage] 
*            
*        @li 'expectation' is  a  constant  or a  variable  containing
*            the expected value for the item.
*        @li 'faultMessage' contains an  optional string to be printed
*            in case  the read value for  the item is not  the same as 
*            the expectation.
*        @li 'offset' is an optional  parameter (default is 0) and can
*            be a constant or a variable name. 
*            
*            
*       @see CheckFactory
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/

class CheckCommand : public SequencerCommand {
public:
  /**
   * @param sequence denotes the Sequence to which this command belongs. 
   *        This reference is needed to store the read value in a variable 
   *        of the sequence.
   */
  CheckCommand( std::string item,
               CommandSequence& sequence );

  virtual ~CheckCommand() {};

  void excecute( const HardwareDeviceInterface& device ) const
    throw( IllegalOperationException,
           BusAdapterException );

  /**
   * This is used by the CheckFactory.
   */
  void setOffsetPointer( uint32_t* offsetPtr );
        
  /**
   * This is used by the CheckFactory.
   */
  void setExpectationPointer( uint32_t* dataPtr );
     
  /**
   * This is used by the CheckFactory.
   */   
  void setExpectation( uint32_t offset );

  /**
   * This is used by the CheckFactory.
   */
  void setOffset( uint32_t offset );

  /**
   * This is used by the CheckFactory.
   */
  void setFailMessage( std::list<std::string>  failMessageList );

private:
  CommandSequence& sequence;
  std::string item;
  uint32_t* expectationPtr;
  uint32_t  expectation;
  uint32_t* offsetPtr;
  uint32_t  offset;
  std::list< std::string > failMessageList;
};

} /* namespace HAL */

#endif /* __CheckCommand */

