#ifndef __UnmaskedWriteCommand
#define __UnmaskedWriteCommand

#include <string>

#include "hal/SequencerCommand.hh"
#include "hal/CommandSequence.hh"
#include "hal/HardwareDeviceInterface.hh"
#include "hal/BusAdapterException.hh"

namespace HAL {

/**
*
*
*     @short SequencerCommand  to  perform a  unmaskedWrite  operation 
*            into  a hardware device.
*            
*            Syntax:
*            
*            unmaskedWrite item data [verifyFlag] [offset]
*            
*        @li 'verifyFlag' is an optional parameter which must have the 
*            value "HAL_DO_VERIFY" or "HAL_NO_VERIFY".
*        @li 'offset'  is an optional  parameter. 'data'  and 'offset' 
*            might be variables or constants. If 'offset' is provided
*            also 'verifyFlag' must be provided.
*            
*       @see UnmaskedWriteFactory
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class UnmaskedWriteCommand : public SequencerCommand {
public:
  UnmaskedWriteCommand( std::string item,
                        CommandSequence& sequence );
  
  virtual ~UnmaskedWriteCommand() {};
  
  void excecute( const HardwareDeviceInterface& device ) const
          throw( BusAdapterException );

  /**
   * This is used by the WriteFactory.
   */
  void setVerify();

  /**
   * This is used by the UnmaskedWriteFactory.
   */  
  void setOffsetPointer( uint32_t* offsetPtr );
       
  /**
   * This is used by the UnmaskedWriteFactory.
   */  
  void setDataPointer( uint32_t* dataPtr );
       
  /**
   * This is used by the UnmaskedWriteFactory.
   */  
  void setData( uint32_t data );
       
  /**
   * This is used by the UnmaskedWriteFactory.
   */  
  void setOffset( uint32_t offset );
  
private:
  CommandSequence& sequence;
  std::string item;
  uint32_t* dataPtr;
  uint32_t* offsetPtr;
  uint32_t  offset, constantData;
  HalVerifyOption verifyFlag;
};

} /* namespace HAL */

#endif /* __UnmaskedWriteCommand */
