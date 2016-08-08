#include "hal/GotoFactory.hh"

HAL::SequencerCommand* HAL::GotoFactory::create( std::list<std::string>& arguments, 
						 const HAL::AddressTableInterface& addressTable,
						 HAL::CommandSequence& sequence ) 
  throw (HAL::SequencerSyntaxError) {
  if ( arguments.size() != 4 ) {
    std::string text ="\"goto\" command must have 4 arguments.\n    syntax : goto {labelName} op1 cond op2";
    throw( HAL::SequencerSyntaxError(text));
  }
  std::string labelName = arguments.front();
  arguments.pop_front();
  std::string op1Str = arguments.front();
  arguments.pop_front();
  std::string condStr = arguments.front();
  arguments.pop_front();
  std::string op2Str = arguments.front();
  arguments.pop_front();
  
  HAL::GotoCommand* commandPtr = new HAL::GotoCommand (labelName,
						       condStr,
						       sequence); 
  
  if (op1Str[0] != '$') {
    commandPtr->setOp1( stringToNumber( op1Str ) );
  } else {
    commandPtr->setOp1Pointer( sequence.getVariablePointer( op1Str ));
  }

  if (op2Str[0] != '$') {
    commandPtr->setOp2( stringToNumber( op2Str ) );
  } else {
    commandPtr->setOp2Pointer( sequence.getVariablePointer( op2Str ));
  }

  return commandPtr;
}

  


