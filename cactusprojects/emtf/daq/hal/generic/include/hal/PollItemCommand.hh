#ifndef __PollItemCommand
#define __PollItemCommand

#include <string>

#include "hal/SequencerCommand.hh"
#include "hal/CommandSequence.hh"
#include "hal/HardwareDeviceInterface.hh"
#include "hal/BusAdapterException.hh"

namespace HAL {

/**
*
*
*     @short SequencerCommand to poll an item.
*            
*            This  Command  corresponds to  the pollItem method in the 
*            HardwareDeviceInterface. The syntax is :
*            
*            pollItem item referenceValue timeout $varname [pollMethod] [offset] 
*            
*        @li 'referenceValue'  which   is  used  to   define  the  end 
*            condition  of  the  polling.  This  parameter  can  be  a 
*            constant or a variable.
*        @li 'timeout' is the  time in ms after which  an exception is 
*            thrown   if   the  poll-condition   has   not  yet   been 
*            encountered.  This  parameter  can  be a  constant  or  a 
*            variable. 
*        @li '$varname' is  the name of  a varible which  will contain 
*            the value read from the item during the last poll cycle.
*        @li 'pollMethod'  is an  optional parameter  which  must have 
*            one    of    the    values    "HAL_POLL_UNTIL_EQUAL"   or 
*            "HAL_POLL_UNTIL_DIFFERENT". If the  first value is chosen
*            the item will  be polled until its value  is equal to the 
*            referenceValue.  If "HAL_POLL_UNTIL_DIFFERENT"  is chosen 
*            the item will  be polled until its value  is not equal to 
*            the referenceceValue.
*        @li 'offset' is an optional  parameter (default is 0) and can
*            be a constant  or a variable name. It  contains an offset 
*            added to address of the item.
*            
*  
*       @see PollItemFactory
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class PollItemCommand : public SequencerCommand {
public:
  /**
   * @param sequence denotes the Sequence to which this command belongs. 
   *        This reference is needed to store the read value in a variable 
   *        of the sequence.
   */
  PollItemCommand( std::string item,
                   CommandSequence& sequence );

  virtual ~PollItemCommand() {};

  void excecute( const HardwareDeviceInterface& device ) const
    throw( IllegalOperationException,
           BusAdapterException,
           MaskBoundaryException,
           TimeoutException  );

  /**
   * This is used by the PollItemFactory.
   */
  void setOffsetPointer( uint32_t* offsetPtr );
        
  /**
   * This is used by the PollItemFactory.
   */
  void setReferenceValuePointer( uint32_t* dataPtr );
     
  /**
   * This is used by the PollItemFactory.
   */
  void setTimeoutPointer( uint32_t* timeoutPtr );
     
  /**
   * This is used by the PollItemFactory.
   */
  void setResultPointer( uint32_t* resultPtr );
     
  /**
   * This is used by the PollItemFactory.
   */   
  void setReferenceValue( uint32_t offset );

  /**
   * This is used by the PollItemFactory.
   */
  void setOffset( uint32_t offset );

  /**
   * This is used by the PollItemFactory.
   */
  void setTimeout( uint32_t timeout );

  /**
   * This is used by the PollItemFactory.
   */
  void setPollMethod( HalPollMethod pollMethod );

private:
  CommandSequence& sequence;
  std::string item;
  uint32_t* referenceValuePtr;
  uint32_t  referenceValue;
  uint32_t* timeoutPtr;
  uint32_t  timeout;
  uint32_t* offsetPtr;
  uint32_t  offset;
  uint32_t* resultPtr;
  HalPollMethod pollMethod;
};

} /* namespace HAL */

#endif /* __PollItemCommand */

