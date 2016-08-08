#include "VMEConfigurationSpaceAddressReaderTester.hh"

std::string HAL::VMEConfigurationSpaceAddressReaderTester::getName() const {
  return "VMEConfigurationSpaceAddressReaderTester";
}

std::string HAL::VMEConfigurationSpaceAddressReaderTester::getDescription() const {
  return "Just reads in a VMETable with the configuration space items.";
}

bool HAL::VMEConfigurationSpaceAddressReaderTester::execute() {

  bool result = true;
  try {
    HAL::VMEConfigurationSpaceAddressReader reader;
    HAL::VMEAddressTable table("VME Configuration Space", reader);  
    table.print();
  } catch( HAL::HardwareAccessException& e ) {
    std::cout << "Unexpected exception: " << std::endl;
    std::cout << e.what() << std::endl;
    result = false;
  }
  return result;
}
