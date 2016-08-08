#include "hal/PrintCommand.hh"
#include <iostream>
#include <iomanip>


HAL::PrintCommand::PrintCommand( std::list< std::string > arguments,
				 HAL::CommandSequence& sequence )
  throw (HAL::SequencerSyntaxError)
  : arguments( arguments ),
    sequence(sequence) {
}

void HAL::PrintCommand::excecute( const HAL::HardwareDeviceInterface& device ) const
  throw(){
  std::list< std::string >::const_iterator it;
  for ( it = arguments.begin(); it!=arguments.end(); it++ ) {
    if ( (*it)[0] == '$' ) {
      uint32_t value = sequence.getVariable(*it);
      std::cout << value << " ";
    } else if ( (*it)[0] == '%' ) {
      if ( *it == "%hex" ) {
        std::cout << std::hex << std::setw(8) << std::setfill('0');
      } else if ( *it == "%dec" ) {
        std::cout << std::dec;
      } else {
        std::cout << *it << " ";
      }
    } else {
      std::cout << *it << " ";
    }
  }
  std::cout << std::endl;
}

