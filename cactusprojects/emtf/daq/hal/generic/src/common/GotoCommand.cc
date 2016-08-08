#include "hal/GotoCommand.hh"


HAL::GotoCommand::GotoCommand( std::string labelName,
			       std::string condition,
			       HAL::CommandSequence& sequence )
  throw (HAL::SequencerSyntaxError)
  : labelName(labelName),
    condition(condition),
    sequence(sequence) {
  op1 = 0;
  op2 = 0;
  op1Ptr = &op1;
  op2Ptr = &op2;
  // test the condition string for validity:
  if ( condition != "==" &&
       condition != "!=" && 
       condition != "<=" && 
       condition != ">=" &&
       condition != "<" &&
       condition != ">" )
    throw (HAL::SequencerSyntaxError("wrong condition string goto command.\n    (HAL::GotoCommand::GotoCommand)"));
}

void HAL::GotoCommand::setOp1( uint32_t op1 ) {
  this->op1 = op1;
}

void HAL::GotoCommand::setOp2( uint32_t op2 ) {
  this->op2 = op2;
}

void HAL::GotoCommand::setOp1Pointer( uint32_t* op1Ptr ) {
  this->op1Ptr = op1Ptr;
}

void HAL::GotoCommand::setOp2Pointer( uint32_t* op2Ptr ) {
  this->op2Ptr = op2Ptr;
}

void HAL::GotoCommand::excecute( const HAL::HardwareDeviceInterface& device ) const
  throw( HAL::SequencerSyntaxError ) {
  bool doJump = false;
  if ( condition == "==" ) {
    if ( *op1Ptr == *op2Ptr ) doJump = true;
  } else if ( condition == "<=" ) {
    if ( *op1Ptr <= *op2Ptr ) doJump = true;
  } else if ( condition == ">=" ) {
    if ( *op1Ptr >= *op2Ptr ) doJump = true;
  } else if ( condition == "<" ) {
    if ( *op1Ptr < *op2Ptr ) doJump = true;
  } else if ( condition == ">" ) {
    if ( *op1Ptr > *op2Ptr ) doJump = true;
  } else if ( condition == "!=" ) {
    if ( *op1Ptr != *op2Ptr ) doJump = true;
  } else {
    // here we should never arrive
    std::cout << "fatal software bug in GotoCommand.cc" << std::endl;
  }
  if (doJump) {
    sequence.gotoLabel( labelName );
  }
}

