#include "hal/LabelCommand.hh"


HAL::LabelCommand::LabelCommand( std::string labelName, 
				 HAL::CommandSequence& sequence )
  : sequence(sequence) {
  this->labelName = labelName;
  sequence.addLabel( labelName );
}

void HAL::LabelCommand::excecute( const HAL::HardwareDeviceInterface& device ) const
  throw() {
  /* nothing to do during command excecution */
}

