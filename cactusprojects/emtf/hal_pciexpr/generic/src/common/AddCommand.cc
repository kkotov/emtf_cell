#include "hal/AddCommand.hh"


HAL::AddCommand::AddCommand( std::string variable,
			     HAL::CommandSequence& sequence )
  throw (HAL::SequencerSyntaxError)
  : sequence(sequence) {
  constant = 0;
  opPtr = &constant;
  variablePointer = sequence.getVariablePointer( variable );
}

void HAL::AddCommand::setOp( long constant ) {
  this->constant = constant;
}

void HAL::AddCommand::setOpPointer( long* opPtr ) {
  this->opPtr = opPtr;
}

void HAL::AddCommand::excecute( const HAL::HardwareDeviceInterface& device ) const
  throw() {
  (*variablePointer) += (*opPtr);
}
