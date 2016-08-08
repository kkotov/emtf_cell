#include "hal/CommandSequenceReader.hh"

HAL::CommandSequenceReader::CommandSequenceReader() 
  : firstAccess( true ) {
}

HAL::CommandSequenceReader::~CommandSequenceReader() {
}

bool HAL::CommandSequenceReader::next( std::list< std::string >& nextCommand ) {
  if ( firstAccess ) {
    it = commandList.begin();
    firstAccess = false;
  }
  if ( it != commandList.end() ) {
    nextCommand = *it;
    it++;
    return true;
  } else {
    return false;
  }
}
