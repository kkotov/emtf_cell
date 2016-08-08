#include "hal/VMEAddressTableXMLFileReader.hh"
#include "hal/PCIAddressTableXMLFileReader.hh"
#include "hal/VMEAddressTable.hh"
#include "hal/PCIAddressTable.hh"

#define CORRECT_VME_XML_TABLE   "VMEXMLAddressTable_correct.xml"
#define PCI_XML_TABLE   "PCIXMLAddressTable.xml"
#define INCORRECT_VME_XML_TABLE "VMEXMLAddressTable_incorrect.xml"

int main() {
  try {
    // read in the correct VME-table and print it out:
    HAL::VMEAddressTableXMLFileReader VMEXmlReader_correct( CORRECT_VME_XML_TABLE );
    HAL::VMEAddressTable VMEAddresses( "my VME Table", VMEXmlReader_correct);
    std::cout << "\n\n\n";
    VMEAddresses.print( std::cout );
    
    // read in a correct PCI-table and print it out:
    HAL::PCIAddressTableXMLFileReader PCIXmlReader( PCI_XML_TABLE );
    HAL::PCIAddressTable PCIAddresses( "my PCI Table", PCIXmlReader);
    std::cout << "\n\n\n";
    PCIAddresses.print( std::cout );
    
    // Here is an example what happens if the table is not 
    // correct (with respect to the schema). The file is parsed
    // in the constructor, so the exception is thrown already
    // in the following line:
    HAL::VMEAddressTableXMLFileReader VMEXmlReader_incorrect( INCORRECT_VME_XML_TABLE );
    HAL::VMEAddressTable VMEAddresses2( "myTable", VMEXmlReader_incorrect);
    std::cout << "\nfinally you made it and found all errors : here the table\n\n";
    VMEAddresses2.print( std::cout );
    
  } catch (HAL::HardwareAccessException& e) {
    std::cout << e.what() << std::endl;
  } catch ( ... ) {
    std::cout << "another exeption was thrown " << std::endl;
  }
}
