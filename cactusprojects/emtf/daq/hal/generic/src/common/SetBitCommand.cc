#include "hal/SetBitCommand.hh"


HAL::SetBitCommand::SetBitCommand( std::string item, 
				   HAL::CommandSequence& sequence )
  : sequence(sequence),
    item( item ) {
  offset = 0;
  offsetPtr = &offset;
  verifyFlag = HAL_NO_VERIFY;
}

void HAL::SetBitCommand::setVerify() {
  verifyFlag = HAL_DO_VERIFY;
}

void HAL::SetBitCommand::excecute( const HAL::HardwareDeviceInterface& device ) const
        throw( HAL::BusAdapterException ) {
  device.setBit( item, verifyFlag, *offsetPtr );
}

void HAL::SetBitCommand::setOffset( uint32_t offset ) {
  this->offset = offset;
}

void HAL::SetBitCommand::setOffsetPointer( uint32_t* offsetPtr ) {
  this->offsetPtr = offsetPtr;
}

