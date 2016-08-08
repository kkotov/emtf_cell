#ifndef __WriteCommand
#define __WriteCommand

#include <string>

#include "hal/SequencerCommand.hh"
#include "hal/CommandSequence.hh"
#include "hal/HardwareDeviceInterface.hh"
#include "hal/BusAdapterException.hh"

namespace HAL {

/**
*
*
*     @short SequencerCommand  to  perform a  write  operation into  a 
*            hardware device.
*            
*            Syntax:
*            
*            write item data [verifyFlag] [offset]
*            
*       @li  'verifyFlag' is an optional parameter which must have the 
*            value "HAL_DO_VERIFY" or "HAL_NO_VERIFY".
*       @li  'offset' is  an optional paramenter.  'data' and 'offset' 
*            might be variables or  constants. If 'offset' is provided
*            also 'verifyFlag' must be provided.
*            
*       @see WriteFactory
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class WriteCommand : public SequencerCommand {
public:
  WriteCommand( std::string item,
                CommandSequence& sequence );

  virtual ~WriteCommand() {};

  void excecute( const HardwareDeviceInterface& device ) const
    throw( BusAdapterException,
           MaskBoundaryException );

  /**
   * This is used by the WriteFactory.
   */
  void setVerify();

  /**
   * This is used by the WriteFactory.
   */  
  void setOffsetPointer( uint32_t* offsetPtr );
        
  /**
   * This is used by the WriteFactory.
   */  
  void setDataPointer( uint32_t* dataPtr );
        
  /**
   * This is used by the WriteFactory.
   */  
  void setData( uint32_t data );
        
  /**
   * This is used by the WriteFactory.
   */  
  void setOffset( uint32_t offset );

private:
  CommandSequence& sequence;
  std::string item;
  uint32_t*  dataPtr;
  uint32_t*  offsetPtr;
  uint32_t   offset, constantData;
  HalVerifyOption verifyFlag;
};

} /* namespace HAL */

#endif /* __WriteCommand */
