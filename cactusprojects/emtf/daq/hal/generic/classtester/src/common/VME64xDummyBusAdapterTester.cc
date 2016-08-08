#include "VME64xDummyBusAdapterTester.hh"

HAL::VME64xDummyBusAdapterTester::VME64xDummyBusAdapterTester( string configSpaceDefinitionFile,
							       string moduleMapperFile,
							       string addressTableContainerFile )
  throw( HAL::IllegalValueException,
	 HAL::NoSuchFileException,
	 HAL::XMLProcessingException,
	 HAL::HardwareAccessException )
  : busAdapter_( configSpaceDefinitionFile, 
                 HAL::VME64xDummyBusAdapter::VERBOSE_ON, 
                 HAL::VME64xDummyBusAdapter::MEMORY_MAP_ON ),
    moduleMapper_(moduleMapperFile),
    addressTableContainer_(moduleMapper_,
			   addressTableContainerFile) {

}

bool HAL::VME64xDummyBusAdapterTester::execute() {
  bool result = true;
  try {
    HAL::VME64xCrate vmeCrate( busAdapter_,
                          addressTableContainer_,
                          moduleMapper_ );
    //vmeCrate.printAddressMap( std::cout );
    uint32_t slotId = 6;
    std::cout << "try to get the device in slot 6 (tests boundary checking)" << std::endl;
    HAL::VMEDevice* devicePtr = vmeCrate.getVMEDevice( slotId );

    std::cout << "accessing HAL::VMEDevice via address read and write commands" << std::endl;

    devicePtr->write( "testBase", 0x12345678 );
    devicePtr->write( "testHigh", 0x9abcdef0 );
    
    std::cout << "reading back" << std::endl;
    
    uint32_t result1, result2;
    devicePtr->read("testBase", &result1);
    std::cout << "read from testBase " << std::hex << result1 << std::endl;
    devicePtr->read("testHigh", &result2 );
    std::cout << "read from testHigh " << std::hex << result2 << std::endl;

    if ( result1 != 0x12345678 ||
	 result2 != 0x9abcdef0 ) {
      result = false;
      std::cout << "read back wrong values ! test failed!" << std::endl;
    }

  } catch( HAL::HardwareAccessException& e) {
    std::cout << "unexpected exception: test failed:" << std::endl;
    std::cout << e.what();
    result = false;
  }
  return result;
}

string HAL::VME64xDummyBusAdapterTester::getName() const {
  return "VME64xDummyBusAdapterTester";
}

string HAL::VME64xDummyBusAdapterTester::getDescription() const {
  return "Performs a test of the HAL::VME64xDummyBusAdapter";
}
