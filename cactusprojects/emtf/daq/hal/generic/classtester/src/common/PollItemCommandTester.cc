#include "PollItemCommandTester.hh"

std::string HAL::PollItemCommandTester::getDescription() const {
  std::string result = "Tests if a sequence can be constructed and run";
  return result;
}

bool HAL::PollItemCommandTester::execute() {
  bool result = true;
  try {
    HAL::VMEDummyBusAdapter busAdapter(HAL::VMEDummyBusAdapter::VERBOSE_OFF,
                                  HAL::VMEDummyBusAdapter::MEMORY_MAP_ON);
    HAL::VMEAddressTableASCIIReader addressTableReader ( ADDRESSTABLE );
    HAL::VMEAddressTable addressTable( "Test Table", addressTableReader );
    HAL::VMEDevice device(addressTable, busAdapter, 0 );
    
    HAL::CommandSequenceASCIIReader sequenceReader( SEQUENCE );
    HAL::CommandSequence sequence( "testSequence", sequenceReader, addressTable );
    
    sequence.run( device );
    std::cout << "seq has run" << std::endl;
  } catch( HAL::HardwareAccessException &e ) {
    std::cout << e.what() << std::endl;
    result = false;
  }
  return result;
}
