#include "hal/CheckCommand.hh"
#include <sstream>
#include <iomanip>

HAL::CheckCommand::CheckCommand( std::string item, 
				 HAL::CommandSequence& sequence )
  : sequence(sequence),
    item( item ) {
  offsetPtr = &offset;
  expectationPtr = &expectation;
  offset = 0;
}

void HAL::CheckCommand::excecute( const HAL::HardwareDeviceInterface& device ) const
  throw( HAL::IllegalOperationException,
         HAL::BusAdapterException ) {
  std::string failMessage = "";
  if ( failMessageList.size() > 0 ) {
    std::list< std::string >::const_iterator it;
    std::stringstream text;
    for ( it = failMessageList.begin(); it!=failMessageList.end(); it++ ) {
      if ( (*it)[0] == '$' ) {
        uint32_t value = sequence.getVariable(*it);
        text << value << " ";
      } else if ( (*it)[0] == '%' ) {
        if ( *it == "%hex" ) {
          text << std::hex << std::setw(8) << std::setfill('0');
        } else if ( *it == "%dec" ) {
          text << std::dec;
        } else {
          text << *it << " ";
        }
      } else {
        text << *it << " ";
      }
    }
    text << std::ends;
    failMessage = text.str();
  }
  device.check( item, *expectationPtr, failMessage, *offsetPtr );
}

void HAL::CheckCommand::setOffset( uint32_t offset ) {
  this->offset = offset;
}

void HAL::CheckCommand::setExpectation( uint32_t expectation ) {
  this->expectation = expectation;
}

void HAL::CheckCommand::setOffsetPointer( uint32_t* offsetPtr ) {
  this->offsetPtr = offsetPtr;
}

void HAL::CheckCommand::setExpectationPointer( uint32_t* expectationPtr ) {
  this->expectationPtr = expectationPtr;
}

void HAL::CheckCommand::setFailMessage( std::list<std::string>  failMessageList ) {
  this->failMessageList = failMessageList;
}
