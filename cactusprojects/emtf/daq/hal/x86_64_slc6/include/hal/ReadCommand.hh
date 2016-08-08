#ifndef __ReadCommand
#define __ReadCommand

#include <string>

#include "hal/SequencerCommand.hh"
#include "hal/CommandSequence.hh"
#include "hal/HardwareDeviceInterface.hh"
#include "hal/BusAdapterException.hh"

namespace HAL {

/**
*
*
*     @short SequencerCommand to perform a read  operation into from a
*            hardware device.
*            
*            This  Command  corresponds to  the  read  method in  the 
*            HardwareDeviceInterface.  The read  value is  put  into a 
*            variable. The syntax is :
*            
*            read item $varname [offset]
*            
*        @li 'offset' is an optional  parameter (default is 0) and can
*            be a constant or a variable name. 
*            
*            
*       @see ReadFactory
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class ReadCommand : public SequencerCommand {
public:
  /**
   * @param sequence denotes the Sequence to which this command belongs. 
   *        This reference is needed to store the read value in a variable 
   *        of the sequence.
   */
  ReadCommand( std::string item,
               CommandSequence& sequence );

  virtual ~ReadCommand() {};

  void excecute( const HardwareDeviceInterface& device ) const
    throw( IllegalOperationException,
           BusAdapterException,
           MaskBoundaryException);

  /**
   * This is used by the ReadFactory.
   */
  void setOffsetPointer( uint32_t* offsetPtr );
        
  /**
   * This is used by the ReadFactory.
   */
  void setResultPointer( uint32_t* dataPtr );
        
  /**
   * This is used by the ReadFactory.
   */
  void setOffset( uint32_t offset );

private:
  CommandSequence& sequence;
  std::string item;
  uint32_t* resultPtr;
  uint32_t* offsetPtr;
  uint32_t  offset;
};

} /* namespace HAL */

#endif /* __ReadCommand */
