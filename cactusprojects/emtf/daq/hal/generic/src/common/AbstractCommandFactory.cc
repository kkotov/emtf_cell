#include <string>
#include "hal/AbstractCommandFactory.hh"


uint32_t 
HAL::AbstractCommandFactory::stringToNumber( std::string numberString ) 
  throw (HAL::SequencerSyntaxError) {
  uint32_t data;
  int success;
  // dirty conversion of the data string to a number:
  if ( numberString.size() >= 2 &&
       numberString[0] == '0' &&
       ( numberString[1] == 'x' || numberString[1] == 'X') ) {
    // we need to scan a hex number
    success = sscanf(numberString.c_str(),"%x", &data);
  } else {
    // we need to scan a dec string
    success = sscanf(numberString.c_str(),"%d", &data);
  }
  if (success == 0) {
    throw (HAL::SequencerSyntaxError("Wrong data number format."));
  }
  return data;
}
