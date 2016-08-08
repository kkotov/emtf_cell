#include "hal/DefineFactory.hh"

HAL::SequencerCommand* HAL::DefineFactory::create( std::list<std::string>& arguments, 
						   const HAL::AddressTableInterface& addressTable,
						   HAL::CommandSequence& sequence ) 
  throw (HAL::SequencerSyntaxError) {
  if ( arguments.size() < 1 || arguments.size() > 2 ) {
	std::string text ="\"define\" command must have one or two arguments.\n    syntax : define $name [initValue]";
    throw( HAL::SequencerSyntaxError(text));
  }
  std::string variableName = arguments.front();
  arguments.pop_front();
  bool hasInitialization = false;
  std::string initArg = "";
  if ( arguments.size() == 1 ) {
    hasInitialization = true;
    initArg = arguments.front();
  }
  HAL::DefineCommand* commandPtr =  new HAL::DefineCommand (variableName, 
							    hasInitialization, 
							    sequence); 
  if (hasInitialization) {
    if (initArg[0] != '$') {
      commandPtr->setInitValue( stringToNumber( initArg ) );
    } else {
      commandPtr->setInitValuePointer( sequence.getVariablePointer( initArg ));
    }
  }

  return commandPtr;
}


  


