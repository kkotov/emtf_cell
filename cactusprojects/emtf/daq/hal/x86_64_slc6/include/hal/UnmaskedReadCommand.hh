#ifndef __UnmaskedReadCommand
#define __UnmaskedReadCommand

#include <string>

#include "hal/SequencerCommand.hh"
#include "hal/CommandSequence.hh"
#include "hal/HardwareDeviceInterface.hh"
#include "hal/BusAdapterException.hh"

namespace HAL {

/**
*
*
*     @short SequencerCommand to perform a unmaskedRead operation from
*            from a hardware device.
*            
*            Syntax :
*            
*            unmaskedRead item $varname [offset]
*            
*            'offset'  is  an  optional  paramenter  and  might  be  a 
*            constant or a variable. 
*
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class UnmaskedReadCommand : public SequencerCommand {
public:
  /**
   * @param sequence denotes the Sequence to which this command belongs. 
   *        This reference is needed to store the read value into the 
   *        variable.
   */
  UnmaskedReadCommand( std::string item,
		       CommandSequence& sequence );

  virtual ~UnmaskedReadCommand() {};

  void excecute( const HardwareDeviceInterface& device ) const
    throw ( IllegalOperationException,
            BusAdapterException );

  /**
   * This is used by the UnmaskedReadFactory.
   */
  void setOffsetPointer( uint32_t* offsetPtr );
        
  /**
   * This is used by the UnmaskedReadFactory.
   */
  void setResultPointer( uint32_t* dataPtr );
        
  /**
   * This is used by the UnmaskedReadFactory.
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

#endif /* __UnmaskedReadCommand */
