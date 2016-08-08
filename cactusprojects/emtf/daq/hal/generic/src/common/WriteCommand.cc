#include "hal/WriteCommand.hh"


HAL::WriteCommand::WriteCommand( std::string item, 
				 HAL::CommandSequence& sequence )
  : sequence(sequence),
    item( item ) {
  dataPtr = &constantData;
  offsetPtr = &offset;
  constantData = 0;
  offset = 0;
  verifyFlag = HAL_NO_VERIFY;
}

void HAL::WriteCommand::setVerify() {
  verifyFlag = HAL_DO_VERIFY;
}

void HAL::WriteCommand::excecute( const HAL::HardwareDeviceInterface& device ) const
  throw ( HAL::BusAdapterException, 
          HAL::MaskBoundaryException ) {
  device.write( item, *dataPtr, verifyFlag, *offsetPtr );
}

void HAL::WriteCommand::setOffset( uint32_t offset ) {
  this->offset = offset;
}

void HAL::WriteCommand::setData( uint32_t data ) {
  this->constantData = data;
}

void HAL::WriteCommand::setOffsetPointer( uint32_t* offsetPtr ) {
  this->offsetPtr = offsetPtr;
}

void HAL::WriteCommand::setDataPointer( uint32_t* dataPtr ) {
  this->dataPtr = dataPtr;
}
