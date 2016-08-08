#include "XMLAddressTableTester.hh"
#include <sstream>

std::string HAL::XMLAddressTableTester::getDescription() const {
  std::stringstream text;
  text << "Tests the boundary check if the XMLAddressTable.\n"
       << "The function checkAddressLimits(item, offset) is\n"
       << "used to perform the test" 
       << std::ends;
  return text.str();
}

bool HAL::XMLAddressTableTester::execute() {
  try {
    HAL::VMEAddressTableXMLFileReader reader1( VMEXMLADDRESSTABLE_1 );
    HAL::VMEAddressTable table1( "VMEADDRESSTABLE1", reader1);
    
    // display table:
    std::cout << "\n\n\n\n";
    table1.print();
    std::cout << "\n";

    HAL::VMEAddressTableXMLFileReader reader2( VME64xXMLADDRESSTABLE_1 );
    HAL::VMEAddressTable table2( "VMEADDRESSTABLE2", reader2);
    
    // display table:
    std::cout << "\n\n\n\n";
    table2.print();
    std::cout << "\n";

    HAL::PCIAddressTableXMLFileReader reader3( PCIXMLADDRESSTABLE_1 );
    HAL::PCIAddressTable table3( "PCIADDRESSTABLE1", reader3 );
    
    // display table:
    std::cout << "\n\n\n\n";
    table3.print();
    std::cout << "\n";


  } catch (HAL::HardwareAccessException& e) {
    std::cout << "!!! ERROR !!! UNEXPECTED EXCETPION !!!" << std::endl;
    std::cout << e.what() << std::endl;
    return false;
  }
  // all ok if arrived here
  return true;
}
