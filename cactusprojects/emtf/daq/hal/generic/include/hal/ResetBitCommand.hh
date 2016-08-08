#ifndef __ResetBitCommand
#define __ResetBitCommand

#include <string>

#include "hal/SequencerCommand.hh"
#include "hal/CommandSequence.hh"
#include "hal/HardwareDeviceInterface.hh"
#include "hal/BusAdapterException.hh"

namespace HAL {

/**
*
*
*     @short SequencerCommand to set a single bit in the hardware. 
*            
*            The syntax is :
*            
*            resetBit item [verifyFlag] [offset]
*            
*        @li 'verifyFlag' is an optional parameter which must have the 
*            value "HAL_DO_VERIFY" or "HAL_NO_VERIFY".
*        @li 'offset'  is  an  optional   parameter  which  can  be  a 
*            constant or a variable.  If 'offset' is provided
*            also 'verifyFlag' must be provided.
*            
*       @see ResetBitFactory
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class ResetBitCommand : public SequencerCommand {
public:

  ResetBitCommand( std::string item,
                   CommandSequence& sequence );

  virtual ~ResetBitCommand() {};

  void excecute( const HardwareDeviceInterface& device ) const
          throw( BusAdapterException );

  /**
   * This is used by the WriteFactory.
   */
  void setVerify();

  /**
   * This is needed by the ResetBitFactory.
   */
  void setOffset( uint32_t offset );
       
  /**
   * This is needed by the ResetBitFactory.
   */
  void setOffsetPointer( uint32_t* offsetPtr );

private:
  CommandSequence& sequence;
  std::string item;
  uint32_t* offsetPtr;
  uint32_t  offset;
  HalVerifyOption verifyFlag;
};

} /* namespace HAL */

#endif /* __ResetBitCommand */
