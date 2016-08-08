#include "hal/PrintFactory.hh"

HAL::SequencerCommand* HAL::PrintFactory::create( std::list<std::string>& arguments, 
						  const HAL::AddressTableInterface& addressTable,
						  HAL::CommandSequence& sequence ) 
  throw (HAL::SequencerSyntaxError) {
  return new HAL::PrintCommand (arguments, sequence);   
}


