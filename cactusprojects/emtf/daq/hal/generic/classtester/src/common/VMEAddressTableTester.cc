#include "VMEAddressTableTester.hh"
#include <sstream>

std::string HAL::VMEAddressTableTester::getDescription() const {
  std::stringstream text;
  text << "Tests the boundary check if the HAL::VMEAddressTable.\n"
       << "The function checkAddressLimits(item, offset) is\n"
       << "used to perform the test" 
       << std::ends;
  return text.str();
}

bool HAL::VMEAddressTableTester::execute() {
  try {
    HAL::VMEAddressTableASCIIReader reader( VMEADDRESSTABLE );
    HAL::VMEAddressTable table( "VMEADDRESSTABLE", reader);
    
    // display table:
    std::cout << "\n\n\n\n";
    table.print();
    std::cout << "\n";

    //display calculated boundaries
    std::vector<uint32_t> minimum, maximum;
    table.getAddressBoundaries( minimum, maximum );
    std::cout << std::hex << "Minumum offset  : " << minimum[0] << std::endl;
    std::cout << std::hex << "Maximum offset  : " << maximum[0] << std::endl;
    std::cout << "\n\n";
    if ( minimum[0] != 0x10 || maximum[0] != 0x103 ) return false;
    // test the limit checker
    std::cout << "\ntry first and last item of table" << std::endl;
    table.checkAddressLimits( "firstItem", 0);
    table.checkAddressLimits( "lastItem" ,0);
    // below limit:
    try {
      std::cout << "\ntry above limit" << std::endl;
      table.checkAddressLimits( "lastItem", 1);
    } catch ( HAL::HardwareAccessException& e ) {
      std::cout << "expected exception: " << std::endl;
      std::cout << e.what() << std::endl;
    }
    // above limit:
    try {
      std::cout << "\ntry below limit" << std::endl;
      table.checkAddressLimits( "firstItem", (uint32_t)-1);
    } catch ( HAL::HardwareAccessException& e ) {
      std::cout << "expected exception: " << std::endl;
      std::cout << e.what() << std::endl;
    }
    
  } catch (HAL::HardwareAccessException& e) {
    std::cout << "!!! ERROR !!! UNEXPECTED EXCETPION !!!" << std::endl;
    std::cout << e.what() << std::endl;
    return false;
  }
  // all ok if arrived here
  return true;
}
