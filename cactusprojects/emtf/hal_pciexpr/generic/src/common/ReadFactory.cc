#include "hal/ReadFactory.hh"

HAL::SequencerCommand* HAL::ReadFactory::create( std::list<std::string>& arguments, 
						 const HAL::AddressTableInterface& addressTable,
						 HAL::CommandSequence& sequence ) 
  throw (HAL::SequencerSyntaxError, HAL::NoSuchItemException) {

  if ( arguments.size() < 2 || arguments.size() > 3 ) {
    throw ( HAL::SequencerSyntaxError("read command must have two or three arguments."));
  }
  std::string item = arguments.front();
  arguments.pop_front();
  // we check immediately if the item exists. So the program gets an
  // exception immediately and not only when the script is run
  if ( ! addressTable.exists( item ) ) {
    std::string text = "Item \"" + item +
      "\" does not exist in HAL::AddressTable" +
      "\n    (HAL::ReadFactory::create)";
    throw (HAL::NoSuchItemException( text, __FILE__, __LINE__, __FUNCTION__ ));
  }

  HAL::ReadCommand* commandPtr = new HAL::ReadCommand (item, sequence); 

  std::string resultStr = arguments.front();
  arguments.pop_front();

  if (resultStr[0] != '$') {
    throw ( HAL::SequencerSyntaxError("read command must have variable as second argument"));
  } else {
    commandPtr->setResultPointer( sequence.getVariablePointer( resultStr ));
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
  return commandPtr;
}
