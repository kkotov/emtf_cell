#include "hal/ReadCommand.hh"


HAL::ReadCommand::ReadCommand( std::string item, 
			       HAL::CommandSequence& sequence )
  : sequence(sequence),
    item( item ) {
  offsetPtr = &offset;
  offset = 0;
}

void HAL::ReadCommand::excecute( const HAL::HardwareDeviceInterface& device ) const 
throw( HAL::IllegalOperationException,
       HAL::BusAdapterException,
       HAL::MaskBoundaryException ) {
  device.read( item, resultPtr, *offsetPtr );
}

void HAL::ReadCommand::setOffset( uint32_t offset ) {
  this->offset = offset;
}

void HAL::ReadCommand::setOffsetPointer( uint32_t* offsetPtr ) {
  this->offsetPtr = offsetPtr;
}

void HAL::ReadCommand::setResultPointer( uint32_t* resultPtr ) {
  this->resultPtr = resultPtr;
}
