#include "hal/PollItemCommand.hh"


HAL::PollItemCommand::PollItemCommand( std::string item, 
				       HAL::CommandSequence& sequence )
  : sequence(sequence),
    item( item ) {
  offsetPtr = &offset;
  referenceValuePtr = &referenceValue;
  timeoutPtr = &timeout;
  offset = 0;
}

void HAL::PollItemCommand::excecute( const HAL::HardwareDeviceInterface& device ) const
  throw( HAL::IllegalOperationException,
         HAL::BusAdapterException,
         HAL::MaskBoundaryException,
         HAL::TimeoutException ) {
  device.pollItem( item, *referenceValuePtr, *timeoutPtr, 
                   resultPtr, pollMethod, *offsetPtr );
}

void HAL::PollItemCommand::setOffset( uint32_t offset ) {
  this->offset = offset;
}

void HAL::PollItemCommand::setTimeout( uint32_t timeout ) {
  this->timeout = timeout;
}

void HAL::PollItemCommand::setReferenceValue( uint32_t referenceValue ) {
  this->referenceValue = referenceValue;
}

void HAL::PollItemCommand::setOffsetPointer( uint32_t* offsetPtr ) {
  this->offsetPtr = offsetPtr;
}

void HAL::PollItemCommand::setPollMethod( HalPollMethod pollMethod ) {
  this->pollMethod = pollMethod;
}

void HAL::PollItemCommand::setReferenceValuePointer( uint32_t* referenceValuePtr ) {
  this->referenceValuePtr = referenceValuePtr;
}

void HAL::PollItemCommand::setResultPointer( uint32_t* resultPtr ) {
  this->resultPtr = resultPtr;
}

void HAL::PollItemCommand::setTimeoutPointer( uint32_t* timeoutPtr ) {
  this->timeoutPtr = timeoutPtr;
}
