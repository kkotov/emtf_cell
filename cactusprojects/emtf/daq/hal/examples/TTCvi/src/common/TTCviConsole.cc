#include "hal/VMEDevice.hh"
#include "hal/VMEAddressTable.hh"
#include "hal/VMEAddressTableASCIIReader.hh"
#include "hal/VMEDummyBusAdapter.hh"
#include "hal/PersistentCommandSequencer.hh"
#include "hal/CommandSequenceASCIIReader.hh"

#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>

#define TTCVI_BASEADDRESS 0x900800
#define TTCVIADDRESSTABLE "TTCviAddressMap.dat"
#define SEQUENCE_SETTINGS "Sequences.dat"

int main() {
  try {
    // if you want to play with real hardware you need a real busAdapter:
    // change the comments below:
    // MXI2x86LinuxBusAdapter busAdapter(0);
    // SBS620x86LinuxBusAdapter busAdapter(0);
    HAL::VMEDummyBusAdapter busAdapter;

    HAL::VMEAddressTableASCIIReader addressTableReader( TTCVIADDRESSTABLE );
    HAL::VMEAddressTable addressTable( "Test address table", addressTableReader );
    HAL::VMEDevice TTCviCard(addressTable, busAdapter, TTCVI_BASEADDRESS);
    
    HAL::PersistentCommandSequencer sequencer( SEQUENCE_SETTINGS, addressTable );

    bool loop = true;
    std::string item, name;
    uint32_t option, ic, value;
    std::vector<std::string> names;

    // Here the interactive loop for the user interaction starts.
    // All this is very bad spaghetti code and should not be taken
    // serious. It just shows how to use some of the HAL components.

    while ( loop ) {
      std::cout << std::endl;
      std::cout << "0) end the program" << std::endl;
      std::cout << "1) write to item" << std::endl; 
      std::cout << "2) read from item" << std::endl;
      std::cout << "3) register a new sequence" << std::endl;
      std::cout << "4) excecute a sequence (after rescan)" << std::endl;
      std::cout << "5) delete a sequence" << std::endl;
      std::cout << "6) print address table " << std::endl;
      std::cout << "Enter option : ";
      std::cin >> option;

      switch (option) {
      case 0:
        loop = false;
        break;
      case 1:
        // if the item is defined readable in the addressmap than the address 
        // corresponding to the item is first read and only the bits belonging
        // to the item are changed. The obtained word is then written back.
        std::cout << "Enter item : ";
        std::cin >> item;
        std::cout << "Enter value (hex) : ";
        std::cin >> std::hex >> value;
        TTCviCard.write( item, value );
        break;
      case 2:
        std::cout << "Enter item : ";
        std::cin >> item;
        TTCviCard.read( item, &value );
        std::cout << "result : " << std::hex << std::setfill('0') << std::setw(8) << value << std::endl;
        break;
      case 3:
        std::cout << "Filename of new sequence : ";
        std::cin >> name;
        try {
          sequencer.registerSequence( name );
        } catch ( HAL::HardwareAccessException& e ) {
          std::cout << e.what() << std::endl;
        }
        break;
      case 4:
        names.clear();
        names = sequencer.getNameVector();
        std::cout << std::endl;
        if ( names.size() == 0 ) {
          std::cout << "There are no sequences registered!" << std::endl;
          break;
        }
        for( ic=0; ic < names.size(); ic++ ) {
          std::cout << "   " << (ic+1) << ") " << names[ic] << std::endl;
        }
        std::cout << "Enter Number : ";
        std::cin >> option;
        // the sequence is rescaned in case the user has changed 
        // it in the meanwhile in an editor. This is useful in 
        // the debugging phase. You do not need to restart the 
        // program. 
        if ( option > 0 && option <= names.size() ) {
          sequencer.rescan( names[option-1] );
          sequencer.run( names[option-1], TTCviCard);
        } else {
          std::cout << "Invalid number !" << std::endl;
        }
        break;
      case 5:
        std::cout << "Filename of sequence to be deleted : ";
        std::cin >> name;
        try {
          sequencer.deleteSequence( name );
        } catch ( HAL::HardwareAccessException& e ) {
          std::cout << e.what() << std::endl;
        }
        break;
      case 6:
        addressTable.print();
        break;
      default:
        std::cout << "You typed an invalid option number!" << std::endl;
        break;
      }
    }

  } catch ( HAL::HardwareAccessException& e ) {
    std::cout << "*** Exception occurred : " << std::endl;
    std::cout << e.what() << std::endl;
  } catch ( ... ) {
    std::cout << "*** Unknown exception occurred" << std::endl;
  }
  std::cout << "This is the end, my friend..." << std::endl;
  return 0;
}

