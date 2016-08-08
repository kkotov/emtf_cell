#include "hal/AddFactory.hh"

HAL::SequencerCommand* HAL::AddFactory::create( std::list<std::string>& arguments, 
						const HAL::AddressTableInterface& addressTable,
						HAL::CommandSequence& sequence ) 
  throw (HAL::SequencerSyntaxError) {

  if ( arguments.size() != 2 ) {
    throw ( HAL::SequencerSyntaxError("add command must have two arguments."));
  }
  std::string variable = arguments.front();
  arguments.pop_front();
  sequence.existVariable( variable );
  std::string dataStr = arguments.front();
  arguments.pop_front();
  
  HAL::AddCommand* commandPtr = new HAL::AddCommand(variable, 
						    sequence); 
  if ( dataStr[0] != '$' ) {
    commandPtr->setOp( stringToNumber( dataStr ) );
  } else {
    commandPtr->setOpPointer( (long*)sequence.getVariablePointer( dataStr ) );
  }
  return commandPtr;
}

