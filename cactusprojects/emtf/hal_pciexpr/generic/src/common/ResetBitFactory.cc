#include "hal/ResetBitFactory.hh"

HAL::SequencerCommand* HAL::ResetBitFactory::create( std::list<std::string>& arguments, 
						     const HAL::AddressTableInterface& addressTable,
						     HAL::CommandSequence& sequence ) 
  throw (HAL::SequencerSyntaxError, HAL::NoSuchItemException) {

  if ( arguments.size() < 1 || arguments.size() > 3 ) {
    throw ( HAL::SequencerSyntaxError("resetBit command must have one to three arguments."));
  }
  std::string item = arguments.front();
  arguments.pop_front();
  // we check immediately if the item exists. So the program gets an
  // exception immediately and not only when the script is run
   if ( ! addressTable.exists( item ) ) {
    std::string text = "Item \"" + item +
      "\" does not exist in HAL::AddressTable" +
      "\n    (HAL::ResetBitFactory::create)";
    throw (HAL::NoSuchItemException( text, __FILE__, __LINE__, __FUNCTION__ ));
  }

  HAL::ResetBitCommand* commandPtr = new HAL::ResetBitCommand( item, sequence );

  if ( arguments.size() >= 1 ) {
    std::string verifyStr = arguments.front();
    arguments.pop_front();
    if (verifyStr == "HAL_DO_VERIFY") {
      commandPtr->setVerify();
    } else if (verifyStr != "HAL_NO_VERIFY") {
      std::string text = "If present second argument must be 'HAL_DO_VERIFY' or 'HAL_NO_VERIFY'\n    (HAL::ResetBitFactory::create)";
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
