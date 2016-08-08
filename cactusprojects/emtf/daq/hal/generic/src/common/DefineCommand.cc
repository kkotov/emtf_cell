#include "hal/DefineCommand.hh"


HAL::DefineCommand::DefineCommand( std::string variableName, 
				   bool hasInitialization,
				   HAL::CommandSequence& sequence )
  : hasInitialization( hasInitialization ),
    initValue( initValue ),
	sequence(sequence) {

  initValuePtr = &initValue;
  if (variableName[0] != '$') {
	std::string text = "Unvalid Variable name : " + variableName +
	  "\n    Variables must start with a \"$\"." +
	  "\n    (HAL::DefineCommand::DefineCommand)";
	throw(HAL::SequencerSyntaxError( text ));
  }
  this->variableName = variableName;
  variablePtr = sequence.addVariable( variableName, initValue );
}

void HAL::DefineCommand::setInitValuePointer( uint32_t* initValPtr ) {
  this->initValuePtr = initValPtr;
}

void HAL::DefineCommand::setInitValue( uint32_t initValue ) {
  this->initValue = initValue;
}

void HAL::DefineCommand::excecute( const HAL::HardwareDeviceInterface& device ) const
  throw() {
  if (hasInitialization) {
	*variablePtr = *initValuePtr;
  }
}

