#include "hal/PersistentCommandSequencer.hh"
#include <vector>
#include <fstream>

HAL::PersistentCommandSequencer::PersistentCommandSequencer( std::string configurationFile,
							     const AddressTableInterface& addressTable )
  throw(IllegalOperationException)
  : configurationFile( configurationFile ),
    addressTable( addressTable ) { 
  loadSequences();
}

HAL::PersistentCommandSequencer::~PersistentCommandSequencer() {
  saveSequences();
}

void HAL::PersistentCommandSequencer::loadSequences() {

  std::string word;
  std::fstream configurationStream;
  configurationStream.open( configurationFile.c_str(), std::ios::in );

  if ( configurationStream.fail() ) { 
    std::cout << "Cannot open configuration-file " << configurationFile
         << "\n     (HAL::PersistentCommandSequencer::loadSequences)" << std::endl;
  } else {
    while ( ! configurationStream.eof() ) {
      word = "";
      configurationStream >> word;
      
      if (word != "") {
	try {
	  std::cout << "loading sequence : " << word << std::endl;
	  CommandSequenceASCIIReader* rd_ptr = new CommandSequenceASCIIReader( word );
	  CommandSequence* seq_ptr = 
	    new CommandSequence(word, 
				*rd_ptr, 
				addressTable);
	  readerMap[ word ] = rd_ptr;
	  CommandSequencer::registerSequence( *seq_ptr );
	} catch ( HardwareAccessException& e ) {
	  std::cout << e.what() << std::endl;
	}
      }
    }
    // rewrite the configuration file to get rid of all 
    // errorneous sequences.
    saveSequences();
  }
}

void HAL::PersistentCommandSequencer::saveSequences() {

  std::vector<std::string> names = getNameVector();
  uint32_t ic;
  std::fstream configurationStream;
  configurationStream.open( configurationFile.c_str(), std::ios::out );
  if ( configurationStream.fail() ) { 
    std::cout << "Cannot open configuration-file " << configurationFile
         << "\n     (HAL::PersistentCommandSequencer::saveSequences)" << std::endl;
  } else {
    for( ic=0; ic < names.size(); ic++ ) {
      std::cout << "saving : " << names[ic] << std::endl;
      configurationStream << names[ic] << std::endl;
    }
  }
}

void HAL::PersistentCommandSequencer::registerSequence( std::string sequenceName ) { 
  try {
    CommandSequenceASCIIReader* seqrd_ptr = 
      new CommandSequenceASCIIReader( sequenceName );
    CommandSequence* seq_ptr = new CommandSequence(sequenceName, 
                                                   *seqrd_ptr,
                                                   addressTable);
    readerMap[sequenceName] = seqrd_ptr;
    CommandSequencer::registerSequence( *seq_ptr );
    saveSequences();
  } catch ( HardwareAccessException& e ) {
    std::cout << e.what() << std::endl;
  }
}

void HAL::PersistentCommandSequencer::deleteSequence( std::string sequenceName )
  throw( IllegalValueException ) {
  CommandSequencer::deleteSequence( sequenceName );
  saveSequences();
  delete(readerMap[ sequenceName ]);
  readerMap.erase( readerMap.find( sequenceName ));
}

void HAL::PersistentCommandSequencer::deleteSequences( ) {
  CommandSequencer::deleteSequences();
  saveSequences();
  readerMap.clear();
}

