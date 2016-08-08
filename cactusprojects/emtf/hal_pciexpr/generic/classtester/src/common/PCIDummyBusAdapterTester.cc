#include "PCIDevice.hh"
#include "PCIAddressTable.hh"
#include "PCIAddressTableASCIIReader.hh"

// adjujst this to your needs (choose the busadapter)
#include "PCIDummyBusAdapter.hh"
#include "CommandSequencer.hh"
#include "CommandSequenceASCIIReader.hh"

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string.h>

// These are veeryyyy ugly hardcoded constand to be modified by
// the user...
#define VENDORID 0x1234
#define DEVICEID 0x5678
#define INDEX 1
#define DEVICE_ADDRESSTABLE "PCIAddressMap.dat"
#define SEQUENCESETTINGS "Sequences.conf"


/**
 * A small routine to load command sequences from the current directory
 */
void loadSequencer( HAL::CommandSequencer& sequencer, HAL::AddressTableInterface& addressTable ) {
  const int MaxLength=1024;
  char buffer[MaxLength];
  std::ifstream input( SEQUENCESETTINGS );
  if ( ! input ) {
    std::string text = "Cannot open file : " + std::string(SEQUENCESETTINGS);
    //    throw (HAL::NoSuchFileException( text, __FILE__, __LINE__, __FUNCTION__ ));
    return;
  }
  std::cout << "trying to load " << SEQUENCESETTINGS << std::endl;
  while ( ! input.eof() ) {
    input.getline(buffer, MaxLength);
    std::istringstream Line(&buffer[0]);
    std::string word;
    Line >> word;
    if (word != "") {
      std::cout << "looking for " << word << std::endl;
      try {
        std::cout << "build reader" << std::endl;
        HAL::CommandSequenceASCIIReader* rd_ptr = new HAL::CommandSequenceASCIIReader( word);
        HAL::CommandSequence* seq_ptr = 
          new HAL::CommandSequence(word, 
                              *rd_ptr, 
                              addressTable);
        sequencer.registerSequence( *seq_ptr );
      } catch ( HAL::HardwareAccessException& e ) {
        std::cout << e.what() << std::endl;
      }
    }
  }
  input.close();
}



/**
 * Here comes the main program
 */
int main() {

  try {
    HAL::PCIDummyBusAdapter busAdapter( HAL::PCIDummyBusAdapter::VERBOSE_OFF, 
                                   HAL::PCIDummyBusAdapter::MEMORY_MAP_ON ); // verbose with memory mapping

    HAL::PCIAddressTableASCIIReader addressTableReader( DEVICE_ADDRESSTABLE );

    HAL::PCIAddressTable addressTable( "Test address table", addressTableReader );

    // just to have some output:
    addressTable.print();

    HAL::PCIDevice device(addressTable, busAdapter, VENDORID, DEVICEID, INDEX);

    // we want to work with sequences (scripts) of the HAL
    HAL::CommandSequencer sequencer;
    loadSequencer( sequencer, addressTable );

    // at this point we have done everything to use the module
    // with all features of the HAL (sequences).

    char *buffer;
    bool loop = true;
    std::string item;
    uint32_t option;
    uint32_t ic, ic1, ic2;
    uint32_t value, offset, size, dumpOffset, dumpSize, dataPtr ;
    uint32_t limit, maxLimit, data;
    std::vector<std::string> names;
    std::ofstream* output;
    HAL::CommandSequence* seq_ptr;
    HAL::CommandSequenceReader* seqrd_ptr;

    // this is a primitive interactive menu loop:
    // here you hack in whatever you want to debug your module:
    while ( loop ) {
      std::cout << std::endl;
      std::cout << "1) write to item" << std::endl; 
      std::cout << "2) read from item" << std::endl;
      std::cout << "3) register a new sequence" << std::endl;
      std::cout << "4) Excecute a sequence (after rescan)" << std::endl;
      std::cout << "5) Save registered sequences" << std::endl;
      std::cout << "6) try a block read" << std::endl;
      std::cout << "7) write a test block" << std::endl;
      std::cout << "8) write a test block with verify option" << std::endl;
      std::cout << std::endl;
      std::cout << "0) end program" << std::endl;
      std::cout << std::endl;
      std::cout << "Enter option : ";
      
      std::cin >> option;

      switch (option) {
      case 0:
        loop = false;
        break;
      case 1:
        std::cout << "Enter item : ";
        std::cin >> item;
        std::cout << "Enter value (hex) : ";
        std::cin >> std::hex >> value;
        device.write( item, value );
        break;
      case 2:
        std::cout << "Enter item : ";
        std::cin >> item;
        device.read( item, &value );
        std::cout << "result : " << std::hex << std::setfill('0') << std::setw(8) << value << std::endl;
        break;
      case 3:
        std::cout << "Register new sequence : ";
        std::cin >> item;
        try {
          seqrd_ptr = new HAL::CommandSequenceASCIIReader( item );
          seq_ptr = 
            new HAL::CommandSequence(item, 
                                *seqrd_ptr,
                                addressTable);
          sequencer.registerSequence( *seq_ptr );
        } catch ( HAL::HardwareAccessException& e ) {
          std::cout << e.what() << std::endl;
        }
        break;
      case 4:
        names.clear();
        names = sequencer.getNameVector();
        std::cout << std::endl;
        if (names.size() == 0) {
          std::cout << "\nthere are no registered sequneces\n" << std::endl;
        } else {
          for( ic=0; ic < names.size(); ic++ ) {
            std::cout << "   " << (ic+1) << ") " << names[ic] << std::endl;
          }
          std::cout << "Enter Number (0: do nothing): ";
          std::cin >> option;
          if ((option < 0) || (option > names.size())) { 
            std::cout << "invalid number ! " << std::endl;
          } else if( option != 0 ) {
            sequencer.rescan( names[option-1] );
            sequencer.run( names[option-1], device);
          }
          std::cout << std::endl;
        }
        break;
      case 5:
        output = new std::ofstream( SEQUENCESETTINGS );
        names.clear();
        names = sequencer.getNameVector();
        for( ic=0; ic < names.size(); ic++ ) {
          *output << names[ic] << std::endl;
        }
        output->close();
        delete(output);
        break;

      case 6:
        std::cout << "Enter offset from where to start (hex) : ";
        std::cin >> std::hex >> offset;
        std::cout << "Enter number of bytes of the transfer (hex) : ";
        std::cin >> std::hex >> size;
        buffer = new char[(size+0x10)];
        memset( buffer, 0, (size+0x10));

        std::cout << "size : " << std::hex << size << std::endl;
        std::cout << "buffer : " << std::hex << (uint32_t) buffer << std::endl;
        std::cout << "offset : " << std::hex << offset << std::endl;

        device.readBlock( "RAMLow", size, buffer, HAL::HAL_DO_INCREMENT, offset );
        
        std::cout << "Enter offset for dumping (hex) :";
        std::cin >> std::hex >> dumpOffset;
        std::cout << "How many bytes do you want to dump (hex) : ";
        std::cin >> std::hex >> dumpSize;
        dataPtr = (uint32_t)buffer + dumpOffset;
        limit = (uint32_t)buffer + dumpOffset + dumpSize;
        maxLimit = (uint32_t) (&(buffer[size]));
        ic2 = 0;
        while (dataPtr < limit) {
          if (dataPtr >= maxLimit){
            std::cout << " out of range" << std::endl;
          } else {
            std::cout << std::hex << std::setw(8) << std::setfill('0') << dataPtr << " , " 
                 << std::setw(8) << std::setfill('0') << ic2 << "  :  ";
            for (ic1 = dataPtr; ic1 < (dataPtr+0x10); ic1+=4) {
              data = *((uint32_t*)ic1);
              std::cout << std::hex << std::setw(8) << std::setfill('0') << data << "  ";
            }
            std::cout << std::endl;
          }
          dataPtr += 0x10;
          ic2 += 0x10;
          }
        delete (buffer);
        break;

      case 7:
        std::cout << "Enter offset from where to start (hex) : ";
        std::cin >> std::hex >> offset;
        std::cout << "Enter number of bytes of the transfer (hex) : ";
        std::cin >> std::hex >> size;
        buffer  = new char[(size+0x10)];
        std::cout << "size : " << std::hex << size << std::endl;
        std::cout <<  "buffer : " << std::hex << (uint32_t) buffer << std::endl;
        std::cout << "offset : " << std::hex << offset << std::endl;
        for (ic = 0; ic<size; ic+=4) {
          *((uint32_t*)(&buffer[ic])) = ic;
        } 

        device.writeBlock( "RAMLow", size, buffer, HAL::HAL_NO_VERIFY, HAL::HAL_DO_INCREMENT, offset );

        delete (buffer);

        break;

      case 8:
        std::cout << "Enter offset from where to start (hex) : ";
        std::cin >> std::hex >> offset;
        std::cout << "Enter number of bytes of the transfer (hex) : ";
        std::cin >> std::hex >> size;
        buffer  = new char[(size+0x10)];
        std::cout << "size : " << std::hex << size << std::endl;
        std::cout <<  "buffer : " << std::hex << (uint32_t) buffer << std::endl;
        std::cout << "offset : " << std::hex << offset << std::endl;
        for (ic = 0; ic<size; ic+=4) {
          *((uint32_t*)(&buffer[ic])) = ic;
        } 

        device.writeBlock( "RAMLow", size, buffer, HAL::HAL_DO_VERIFY, HAL::HAL_DO_INCREMENT, offset );

        delete (buffer);
        break;
      }
    }
    sequencer.deleteSequences();

    // if somehting goes wrong in the HAL an excpetion will 
    // bw thrown : here we get it and print out some info:
    // (look at the HAL-API to find out more about the exceptions
    // thrown)
  } catch (HAL::HardwareAccessException& e ) {
    std::cout << "exceptional exception : " << std::endl;
    std::cout << e.what() << std::endl;
  } catch ( ... ) {
    std::cout << "another exception" << std::endl;
  }
  std::cout << "This is the end, my friend..." << std::endl;
  return 0;
}

