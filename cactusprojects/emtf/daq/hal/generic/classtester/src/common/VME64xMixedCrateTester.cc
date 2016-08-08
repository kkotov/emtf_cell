#include "VME64xMixedCrateTester.hh"

HAL::VME64xMixedCrateTester::VME64xMixedCrateTester( string configSpaceDefinitionFile,
						string moduleMapperFile,
						string addressTableContainerFile,
						string staticConfigurationFile)
  throw( HAL::IllegalValueException,
	 HAL::NoSuchFileException,
	 HAL::XMLProcessingException )
  : busAdapter_( configSpaceDefinitionFile, 
                 HAL::VME64xDummyBusAdapter::VERBOSE_OFF, 
                 HAL::VME64xDummyBusAdapter::MEMORY_MAP_ON ),
    moduleMapper_(moduleMapperFile),
    addressTableContainer_(moduleMapper_,
			   addressTableContainerFile),
    staticConfigurationFilerReader_(staticConfigurationFile),
    staticVMEConfiguration_(staticConfigurationFilerReader_) {
}

bool HAL::VME64xMixedCrateTester::execute() {
  bool result = true;
  try {
    HAL::VME64xCrate vmeCrate( busAdapter_,
                          addressTableContainer_,
                          moduleMapper_,
			  staticVMEConfiguration_);
    vmeCrate.printAddressMap( cout );
    // try again: the crate should not be reconfigured but the
    // configuration should just be read out of the modules.
    HAL::VME64xCrate vmeCrate2( busAdapter_,
                           addressTableContainer_,
                           moduleMapper_,
                           staticVMEConfiguration_);
    vmeCrate2.printAddressMap( std::cout );
  } catch( HAL::HardwareAccessException& e) {
    std::cout << "unexpected exception: test failed:" << std::endl;
    std::cout << e.what();
    result = false;
  }
  return result;
}

string HAL::VME64xMixedCrateTester::getName() const {
  return "VME64xMixedCrateTester";
}

string HAL::VME64xMixedCrateTester::getDescription() const {
  return "Performs a test of the HAL::VME64xCrate with a fixed standard VME configuration";
}
