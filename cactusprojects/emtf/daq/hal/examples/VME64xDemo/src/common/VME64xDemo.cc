#include "hal/VME64xCrate.hh"
#include "hal/VMEAddressTable.hh"
#include "hal/VMEConfigurationSpaceHandler.hh"
#include "hal/VMEConfigurationSpaceAddressReader.hh"
#include "hal/VME64xDummyBusAdapter.hh"
#include "hal/ConfigROMDumper.hh"
#include "hal/ConfigCSRDumper.hh"

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>

#define CRATE_MAPFILE "VME64xCrateConfigMap.dat"
#define SEQUENCE_SETTINGS "Sequences.dat"

int main() {
  try {
    HAL::VME64xDummyBusAdapter busAdapter(CRATE_MAPFILE);
    HAL::VMEConfigurationSpaceHandler configHandler( busAdapter );
    
    // this is only needed to print out the configuration space items"
    HAL::VMEConfigurationSpaceAddressReader* configAdrReader = new HAL::VMEConfigurationSpaceAddressReader();
    HAL::VMEAddressTable configTable( "CR/CSR space", *configAdrReader );

    bool loop = true;
    std::string item, name, serialNumberStr;
    uint32_t option, value, slotId, romStart, romEnd;
    std::vector<std::string> names;
    std::vector<unsigned char> romVector;

    slotId = 0;

    // Here the interactive loop for the user interaction starts.
    // All this is very bad spaghetti code and should not be taken
    // serious. It just shows how to use some of the HAL components.

    while ( loop ) {
      std::cout << std::endl;
      std::cout << "CURRENT SLOT NUMBER IS " << std::dec << slotId << std::endl;
      std::cout << "===========================" << std::endl;
      std::cout << "0) end the program" << std::endl;
      std::cout << "1) write to a configuraiton item" << std::endl; 
      std::cout << "2) read from a configuration item" << std::endl;
      std::cout << "3) dump a region of the CR/CSR space" << std::endl;
      std::cout << "4) check if slot contains VME64x module" << std::endl;
      std::cout << "5) read the serial number" << std::endl; 
      std::cout << "6) change the current slot number" << std::endl;
      std::cout << "7) print configuration items address table " << std::endl;
      std::cout << "8) dump the VME64x CR space " << std::endl;
      std::cout << "9) dump the VME64x CSR space " << std::endl;
      std::cout << "Enter option : ";
      std::cin >> option;

      switch (option) {
      case 0:
        loop = false;
        delete configAdrReader;
        break;
      case 1:
        // if the item is defined readable in the addressmap than the address 
        // corresponding to the item is first read and only the bits belonging
        // to the item are changed. The obtained word is then written back.
        std::cout << "Enter item : ";
        std::cin >> item;
        std::cout << "Enter value (hex) : ";
        std::cin >> std::hex >> value;
        configHandler.configWrite( item, slotId, value );
        break;
      case 2:
        std::cout << "Enter item : ";
        std::cin >> item;
        configHandler.configRead( item, slotId, &value );
        std::cout << "result in hex : " << std::hex << std::setfill('0') << std::setw(8) << value << std::endl;
        break;
      case 3:
	romVector.clear();
	std::cout << "Enter startAddress (hex): ";
	std::cin  >> std::hex >> romStart;
	std::cout << "Enter endAddress (hex): " ;
	std::cin  >> std::hex >> romEnd;
	configHandler.readROMBlock(slotId, romStart, romEnd, &romVector);
	// dump the contents:
	{
	  std::vector<unsigned char>::const_iterator it;
	  uint32_t adrCount = romStart;

	  for ( it = romVector.begin(); it != romVector.end(); it++ ) {
	    std::cout << std::hex << std::setw(8) << std::setfill('0') << adrCount << " : " 
		 << ((uint32_t)*it) << std::endl;
	    adrCount += 4;
	  }
	}
	break;
      case 4:
        if ( configHandler.containsVME64xModule( slotId ) ) {
                std::cout << "The slot contains a VME64x module" << std::endl;
        } else {
                std::cout << "The slot contains NO VME64x module" << std::endl;
        }
        break;
      case 5:
            serialNumberStr = configHandler.readSerialNumber( slotId );
            std::cout << "serial number : " << serialNumberStr << std::endl;
        break;
      case 6:
        std::cout << "Enter the new slotId (dec) : ";
        std::cin >> std::dec >> slotId;
        break;
      case 7:
        configTable.print();
        break;
      case 8:
	{
	  HAL::ConfigROMDumper dumper;
	  dumper.dumpROM( configHandler, slotId );
	}
	break;
      case 9:
	{
	  HAL::ConfigCSRDumper dumper;
	  dumper.dumpCSR( configHandler, slotId );
	}
	break;
      default:
        std::cout << "You typed an invalid option number!" << std::endl;
        break;
      }
    }

  } catch (HAL:: HardwareAccessException& e ) {
    std::cout << "*** Exception occurred : " << std::endl;
    std::cout << e.what() << std::endl;
  } catch ( ... ) {
    std::cout << "*** Unknown exception occurred" << std::endl;
  }
  std::cout << "This is the end, my friend..." << std::endl;
  return 0;
}

