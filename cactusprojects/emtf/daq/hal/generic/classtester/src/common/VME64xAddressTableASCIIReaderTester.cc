#include "VME64xAddressTableASCIIReaderTester.hh"
#include <sstream>

std::string HAL::VME64xAddressTableASCIIReaderTester::getDescription() const {
  std::stringstream text;
  text << "Tests the HAL::VME64xAddressTableASCIIReader."
       << std::ends;
  return text.str();
}

bool HAL::VME64xAddressTableASCIIReaderTester::execute() {
  try {
    HAL::VMEAddressTableASCIIReader reader( VME64xASCIIADDRESSTABLE );
    HAL::VMEAddressTable table( "VME64xASCIIADDRESSTABLE", reader);
    
    // display table:
    std::cout << "\n\n\n\n";
    table.print();
    std::cout << "\n";
  } catch (HAL::HardwareAccessException& e) {
    std::cout << "!!! ERROR !!! UNEXPECTED EXCETPION !!!" << std::endl;
    std::cout << e.what() << std::endl;
    return false;
  }
  // all ok if arrived here
  return true;
}
