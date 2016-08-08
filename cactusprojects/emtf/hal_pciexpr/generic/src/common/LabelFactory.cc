#include "hal/LabelFactory.hh"

HAL::SequencerCommand* HAL::LabelFactory::create( std::list<std::string>& arguments, 
						  const HAL::AddressTableInterface& addressTable,
						  HAL::CommandSequence& sequence ) 
  throw (HAL::SequencerSyntaxError) {
  if ( arguments.size() != 1 ) {
	std::string text ="\"define\" command must have one argument.\n    syntax : label {name}";
    throw( HAL::SequencerSyntaxError(text));
  }
  std::string labelName = arguments.front();
  arguments.pop_front();
  return new HAL::LabelCommand(labelName, sequence); 
}

  


