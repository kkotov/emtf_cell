#include "hal/CheckFactory.hh"

HAL::SequencerCommand* HAL::CheckFactory::create( std::list<std::string>& arguments, 
						  const HAL::AddressTableInterface& addressTable,
						  HAL::CommandSequence& sequence ) 
  throw (HAL::SequencerSyntaxError, HAL::NoSuchItemException) {

  if ( arguments.size() < 2 ) {
    throw ( HAL::SequencerSyntaxError("check command must have at least two arguments."));
  }
  std::string item = arguments.front();
  arguments.pop_front();
  // we check immediately if the item exists. So the program gets an
  // exception immediately and not only when the script is run
  if ( ! addressTable.exists( item ) ) {
    std::string text = "Item \"" + item +
      "\" does not exist in HAL::AddressTable" +
      "\n    (HAL::CheckFactory::create)";
    throw (HAL::NoSuchItemException( text, __FILE__, __LINE__, __FUNCTION__ ));
  }

  HAL::CheckCommand* commandPtr = new HAL::CheckCommand (item, sequence); 

  std::string expectationStr = arguments.front();
  arguments.pop_front();
  if (expectationStr[0] != '$') {
    commandPtr->setExpectation( stringToNumber( expectationStr ) );
  } else {
    commandPtr->setExpectationPointer( sequence.getVariablePointer( expectationStr ));
  }

  if ( arguments.size() >= 1 ) {
    std::string offsetStr = arguments.front();
    arguments.pop_front();
    if (offsetStr[0] != '$') {
      commandPtr->setOffset( stringToNumber( offsetStr ));
    } else {
      commandPtr->setOffsetPointer( sequence.getVariablePointer( offsetStr ));
    }
  }

  commandPtr->setFailMessage( arguments );
    
  return commandPtr;
}
