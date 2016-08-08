#include "hal/UnmaskedReadCommand.hh"


HAL::UnmaskedReadCommand::UnmaskedReadCommand( std::string item, 
					       HAL::CommandSequence& sequence )
  : sequence(sequence),
    item( item ) {
  offsetPtr = &offset;
  offset = 0;
}

void HAL::UnmaskedReadCommand::excecute( const HAL::HardwareDeviceInterface& device ) const
  throw ( HAL::IllegalOperationException,
          HAL::BusAdapterException ) {
  device.unmaskedRead( item, resultPtr, *offsetPtr );
}

void HAL::UnmaskedReadCommand::setOffset( uint32_t offset ) {
  this->offset = offset;
}

void HAL::UnmaskedReadCommand::setOffsetPointer( uint32_t* offsetPtr ) {
  this->offsetPtr = offsetPtr;
}

void HAL::UnmaskedReadCommand::setResultPointer( uint32_t* resultPtr ) {
  this->resultPtr = resultPtr;
}
