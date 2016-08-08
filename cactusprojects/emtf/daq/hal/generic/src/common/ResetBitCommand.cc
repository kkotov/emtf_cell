#include "hal/ResetBitCommand.hh"


HAL::ResetBitCommand::ResetBitCommand( std::string item, 
				       HAL::CommandSequence& sequence )
  : sequence(sequence),
    item( item ) {
  offsetPtr = &offset;
  offset = 0;
  verifyFlag = HAL_NO_VERIFY;
}

void HAL::ResetBitCommand::excecute( const HAL::HardwareDeviceInterface& device ) const
        throw( HAL::BusAdapterException ) {
  device.resetBit( item, verifyFlag, *offsetPtr );
}

void HAL::ResetBitCommand::setVerify() {
  verifyFlag = HAL_DO_VERIFY;
}

void HAL::ResetBitCommand::setOffset( uint32_t offset ) {
  this->offset = offset;
}

void HAL::ResetBitCommand::setOffsetPointer( uint32_t* offsetPtr ) {
  this->offsetPtr = offsetPtr;
}
