#include "hal/CommandSequencer.hh"


void HAL::CommandSequencer::registerSequence( HAL::CommandSequence& sequence ) {
  sequenceMap[ sequence.getName() ] = &sequence;
}

void HAL::CommandSequencer::run( std::string sequenceName,
			    const HAL::HardwareDeviceInterface& device ) const
  throw( HAL::IllegalValueException, 
         HAL::SequencerSyntaxError,
         HAL::IllegalOperationException,
         HAL::BusAdapterException,
         HAL::MaskBoundaryException,
         HAL::TimeoutException ) {
  std::tr1::unordered_map<std::string, HAL::CommandSequence*>::const_iterator it;
  if ( (it = sequenceMap.find( sequenceName )) == sequenceMap.end() ) {
    std::string text = "No such Sequence registered : " + sequenceName +
      "\n    (HAL::CommandSequencer::registerSequence)";
    throw( HAL::IllegalValueException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }
  (*it).second->run( device );
}

std::vector<std::string> HAL::CommandSequencer::getNameVector() const {
  std::vector<std::string> nameVector;
  std::tr1::unordered_map< std::string, HAL::CommandSequence* >::const_iterator it;
  for ( it=sequenceMap.begin(); it != sequenceMap.end(); it++ ) {
    nameVector.push_back( (*it).first );
  }
  return nameVector;
}

void HAL::CommandSequencer::deleteSequence( std::string sequenceName) 
  throw( HAL::IllegalValueException ) {
  std::tr1::unordered_map< std::string, HAL::CommandSequence* >::iterator it;
  if ( (it=sequenceMap.find( sequenceName )) == sequenceMap.end() ) {
    std::string text = "No such Sequence registered : " + sequenceName +
      "\n    (HAL::CommandSequencer::registerSequence)";
    throw( HAL::IllegalValueException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }
  delete( (*it).second );
  sequenceMap.erase(it);
}

void HAL::CommandSequencer::deleteSequences() {
  std::tr1::unordered_map< std::string, HAL::CommandSequence* >::iterator it;
  for ( it=sequenceMap.begin(); it != sequenceMap.end(); it++ ) {
    delete( (*it).second );
    sequenceMap.erase(it);
  }  
}

void HAL::CommandSequencer::rescan( std::string sequenceName )
  throw (HAL::IllegalValueException,
         HAL::NoSuchItemException,
         HAL::SequencerSyntaxError ) {
  if ( sequenceMap.find( sequenceName ) == sequenceMap.end() ) {
    std::string text = "No such Sequence registered : " + sequenceName +
      "\n    (HAL::CommandSequencer::rescan)";
    throw( HAL::IllegalValueException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }
  sequenceMap[ sequenceName ]->rescan();
}


HAL::CommandSequence& HAL::CommandSequencer::getSequence( std::string sequenceName) 
  throw( HAL::IllegalValueException ) {
    std::tr1::unordered_map< std::string, HAL::CommandSequence* >::iterator it;
    if ( (it=sequenceMap.find( sequenceName )) == sequenceMap.end() ) {
      std::string text = "No such Sequence registered : " + sequenceName +
	"\n    (HAL::CommandSequencer::registerSequence)";
      throw( HAL::IllegalValueException( text, __FILE__, __LINE__, __FUNCTION__ ) );
    }
    return *sequenceMap[ sequenceName ];
}
