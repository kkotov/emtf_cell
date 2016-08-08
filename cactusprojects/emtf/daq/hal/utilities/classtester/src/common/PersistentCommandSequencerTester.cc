#include "PersistentCommandSequencer.hh"
#include "PCIAddressTableASCIIReader.hh"
#include "PCIAddressTable.hh"
#include "HardwareAccessException.hh"

#define CONFIGURATIONFILE "CommandConfiguration.dat"
#define PCITESTADDRESSTABLE "PCITestAddressTable.dat"

#include <iostream>

int main() {
  bool loop = true;
  uint32_t ic;
  int option;
  std::string name;
  std::vector<std::string> names;

  HAL::PCIAddressTableASCIIReader addressTableReader( PCITESTADDRESSTABLE );
  HAL::PCIAddressTable addressTable( "Test address table", addressTableReader );
  HAL::PersistentCommandSequencer test( CONFIGURATIONFILE, addressTable );

    try {
      while ( loop ) {
        std::cout << "1) register sequence" << std::endl; 
        std::cout << "2) delete sequence" << std::endl;
        std::cout << "3) delete all sequences"<< std::endl;
        std::cout << "4) list sequences" << std::endl;
        std::cout << "Enter option : ";
        std::cin >> option;
        
        switch (option) {
        case 0:
          loop = false;
          break;
        case 1:
          std::cout << "Enter sequence name : ";
          std::cin >> name;
          test.registerSequence( name );
          break;
        case 2:
          std::cout << "Enter sequence name : ";
          std::cin >> name;
          test.deleteSequence( name );
          break;        
        case 3:
          std::cout << "Deleting all sequences" << std::endl;
          test.deleteSequences();
          break;
        case 4:
          names = test.getNameVector();
          for ( ic=0; ic<names.size(); ic++) {
            std::cout << names[ic] << std::endl;
          }
          break;
        }
      }
    } catch  ( HAL::HardwareAccessException& e ) {
      std::cout << "exceptional exception : " << std::endl;
      std::cout << e.what() << std::endl;
    } catch ( std::exception& e ) {
      std::cout << "another exception" << std::endl;
    }
    std::cout << "This is the end, my friend..." << std::endl;
    return 0;
}
