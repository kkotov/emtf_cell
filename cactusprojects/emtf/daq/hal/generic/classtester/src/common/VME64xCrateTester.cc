#include "VME64xCrateTester.hh"

HAL::VME64xCrateTester::VME64xCrateTester( string configSpaceDefinitionFile,
                                      string moduleMapperFile,
                                      string addressTableContainerFile )
  throw( HAL::IllegalValueException,
	 HAL::NoSuchFileException,
	 HAL::XMLProcessingException,
	 HAL::HardwareAccessException )
  : busAdapter_( configSpaceDefinitionFile, 
                 HAL::VME64xDummyBusAdapter::VERBOSE_OFF, 
                 HAL::VME64xDummyBusAdapter::MEMORY_MAP_ON ),
    moduleMapper_(moduleMapperFile),
    addressTableContainer_(moduleMapper_,
			   addressTableContainerFile) {

}

bool HAL::VME64xCrateTester::execute() {
  bool result = true;
  try {
    HAL::VME64xCrate vmeCrate( busAdapter_,
			       addressTableContainer_,
			       moduleMapper_ );
    vmeCrate.printAddressMap( cout );
	uint32_t slotId = 0;
	std::cout << "try to get the device in slot 0 (tests boundary checking)" << std::endl;
	vmeCrate.getVMEDevice( slotId );
	std::cout << "a second time..." << std::endl;
	HAL::VMEConfigurationSpaceHandler vmeConfigHandler( busAdapter_ );
	HAL::ConfigCSRDumper csrDumper;
	csrDumper.dumpCSR( vmeConfigHandler, 6 );
	vmeConfigHandler.disableVME64xModule(6);
	vmeConfigHandler.configWrite("bitSet",6,0);
	HAL::VME64xCrate vmeCrate2( busAdapter_,
			      addressTableContainer_,
			      moduleMapper_ );
	vmeCrate2.printAddressMap( cout );
	csrDumper.dumpCSR( vmeConfigHandler, 6 );
  } catch( HAL::HardwareAccessException& e) {
    std::cout << "unexpected exception: test failed:" << std::endl;
    std::cout << e.what();
    result = false;
  }
  return result;
}

string HAL::VME64xCrateTester::getName() const {
  return "VME64xCrateTester";
}

string HAL::VME64xCrateTester::getDescription() const {
  return "Performs a test of the HAL::VME64xCrate";
}
