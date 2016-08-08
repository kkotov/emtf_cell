#include "hal/UnmaskedWriteFactory.hh"

HAL::SequencerCommand* HAL::UnmaskedWriteFactory::create( std::list<std::string>& arguments, 
							  const 	HAL::AddressTableInterface& addressTable,
							  HAL::CommandSequence& sequence ) 
  throw (HAL::SequencerSyntaxError, HAL::NoSuchItemException) {
  
  if ( arguments.size() < 2 || arguments.size() > 4 ) {
    throw ( HAL::SequencerSyntaxError("unmaskedWrite command must have two to four arguments."));
  }
  std::string item = arguments.front();
  arguments.pop_front();
  // we check immediately if the item exists. So the program gets an
  // exception immediately and not only when the script is run
  if ( ! addressTable.exists( item ) ) {
    std::string text = "Item \"" + item +
      "\" does not exist in HAL::AddressTable" +
      "\n    (HAL::UnmaskedWriteFactory::create)";
    throw (HAL::NoSuchItemException( text, __FILE__, __LINE__, __FUNCTION__ ));
  }
  
  HAL::UnmaskedWriteCommand* commandPtr = new HAL::UnmaskedWriteCommand( item, sequence );
  
  std::string dataStr = arguments.front();
  arguments.pop_front();
  
  if (dataStr[0] != '$') {
    commandPtr->setData( stringToNumber( dataStr ) );
  } else {
    commandPtr->setDataPointer( sequence.getVariablePointer( dataStr ));
  }

  if ( arguments.size() >= 1 ) {
    std::string verifyStr = arguments.front();
    arguments.pop_front();
    if (verifyStr == "HAL_DO_VERIFY") {
      commandPtr->setVerify();
    } else if (verifyStr != "HAL_NO_VERIFY") {
      std::string text = "If present third argument must be 'HAL_DO_VERIFY' or 'HAL_NO_VERIFY'\n    (HAL::UnmaskedWriteFactory::create)";
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
