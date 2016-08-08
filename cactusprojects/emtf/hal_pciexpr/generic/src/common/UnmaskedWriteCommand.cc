#include "hal/UnmaskedWriteCommand.hh"


HAL::UnmaskedWriteCommand::UnmaskedWriteCommand( std::string item, 
						 HAL::CommandSequence& sequence )
  : sequence(sequence),
    item( item ) {
  dataPtr = &constantData;
  offsetPtr = &offset;
  constantData = 0;
  offset = 0;
  verifyFlag = HAL_NO_VERIFY;
}

void HAL::UnmaskedWriteCommand::excecute( const HAL::HardwareDeviceInterface& device ) const
        throw( HAL::BusAdapterException ) {
  device.unmaskedWrite( item, *dataPtr, verifyFlag, *offsetPtr );
}

void HAL::UnmaskedWriteCommand::setVerify() {
  verifyFlag = HAL_DO_VERIFY;
}

void HAL::UnmaskedWriteCommand::setOffset( uint32_t offset ) {
  this->offset = offset;
}

void HAL::UnmaskedWriteCommand::setData( uint32_t data ) {
  this->constantData = data;
}

void HAL::UnmaskedWriteCommand::setOffsetPointer( uint32_t* offsetPtr ) {
  this->offsetPtr = offsetPtr;
}

void HAL::UnmaskedWriteCommand::setDataPointer( uint32_t* dataPtr ) {
  this->dataPtr = dataPtr;
}
