#include "hal/PollItemFactory.hh"
#include <list>

HAL::SequencerCommand* HAL::PollItemFactory::create( std::list<std::string>& arguments, 
						     const HAL::AddressTableInterface& addressTable,
						     HAL::CommandSequence& sequence ) 
  throw (HAL::SequencerSyntaxError, HAL::NoSuchItemException) {
  
  if ( arguments.size() < 4 || arguments.size() > 6 ) {
    throw ( HAL::SequencerSyntaxError("pollItem command must have four to six arguments."));
  }
  std::string item = arguments.front();
  arguments.pop_front();
  // we check immediately if the item exists. So the program gets an
  // exception immediately and not only when the script is run
  if ( ! addressTable.exists( item ) ) {
    std::string text = "Item \"" + item +
      "\" does not exist in HAL::AddressTable" +
      "\n    (HAL::PollItemFactory::create)";
    throw (HAL::NoSuchItemException( text, __FILE__, __LINE__, __FUNCTION__ ));
  }

  HAL::PollItemCommand* commandPtr = new HAL::PollItemCommand (item, sequence); 

  std::string referenceValueStr = arguments.front();
  arguments.pop_front();
  if (referenceValueStr[0] != '$') {
    commandPtr->setReferenceValue( stringToNumber( referenceValueStr ) );
  } else {
    commandPtr->setReferenceValuePointer( sequence.getVariablePointer( referenceValueStr ));
  }

  std::string timeoutStr = arguments.front();
  arguments.pop_front();
  if (timeoutStr[0] != '$') {
    commandPtr->setTimeout( stringToNumber( timeoutStr ) );
  } else {
    commandPtr->setTimeoutPointer( sequence.getVariablePointer( timeoutStr ));
  }

  std::string resultStr = arguments.front();
  arguments.pop_front();
  if (resultStr[0] != '$') {
    throw ( HAL::SequencerSyntaxError("PollItem command must have variable as fourth argument\n    (HAL::PollItemFactory::create)"));
  } else {
    commandPtr->setResultPointer( sequence.getVariablePointer( resultStr ));
  }

  if ( arguments.size() >= 1 ) {
    std::string pollMethodStr = arguments.front();
    arguments.pop_front();
    if (pollMethodStr == "HAL_POLL_UNTIL_EQUAL") {
      commandPtr->setPollMethod( HalPollMethod(HAL_POLL_UNTIL_EQUAL));
    } else if (pollMethodStr == "HAL_POLL_UNTIL_DIFFERENT") {
      commandPtr->setPollMethod( HalPollMethod(HAL_POLL_UNTIL_DIFFERENT));
    } else {
      std::string text = "If present fifth argument must be 'HAL_POLL_UNTIL_EQUAL' or 'HAL_POLL_UNTIL_DIFFERENT'\n    (HAL::PollItemFactory::create)";
      throw (HAL::SequencerSyntaxError( text ));
      ;
    }
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
