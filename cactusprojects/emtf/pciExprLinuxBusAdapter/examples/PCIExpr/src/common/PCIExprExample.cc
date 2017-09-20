#include "emtf/pciExprLinuxBusAdapter/PCIExprDevice.hh"
#include "hal/PCIAddressTable.hh"
#include "hal/PCIAddressTableASCIIReader.hh"
#include "hal/PCIDummyBusAdapter.hh"
#include "emtf/pciExprLinuxBusAdapter/PCIExprLinuxBusAdapter.hh"

#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>

#define ADDRESSTABLE "/nfshome0/kkotov/cactus_trunk/cactusprojects/emtf/pciExprLinuxBusAdapter/examples/PCIExpr/address_table_ascii.dat"

#include <string.h>
#define LB_MEM_SIZE 0x10000 //size of loopback buffer in sp12, bytes
#define DATA_SIZE 0x1000 //one transfer size
#define N_WRITES (LB_MEM_SIZE / DATA_SIZE) // how many transfers for entire buffer
#define N_WRITES_LONG  1//1000 // loopback module addresses

#include <iostream>
#include <iomanip>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

int main() {
  try {

    HAL::PCIExprLinuxBusAdapter     busAdapter;

    HAL::PCIAddressTableASCIIReader addressTableReader( ADDRESSTABLE );
    HAL::PCIAddressTable            addressTable( "Test address table", addressTableReader );

    // uncomment if you want to print the address table content
    //addressTable.print();
    
    // so far there are only 2 boards connected to the crate, thus 
    // a working mtf7_index can be only 0,1
    uint32_t mtf7_index = 0;
    HAL::PCIExprDevice PCICard(addressTable, busAdapter, mtf7_index);

    uint32_t value_1 = 0x0;
    uint32_t value_2 = 0x1;

    // WRITE/READ SINGLE REGISTERS
    std::cout << "\nwriting  " << std::hex << "0x" << value_1 << " into gth_rst\n";
    PCICard.write( "gth_rst", value_1 );

    std::cout << "\nreading gth_rst\n";
    PCICard.read( "gth_rst", &value_1 );
    std::cout << "result : " << std::hex << "0x" << value_1 << std::endl << std::endl;
    
    std::cout << "writing  " << std::hex << "0x" << value_2 << " into  gth_rst\n";
    PCICard.write( "gth_rst", value_2 );   
    
    std::cout << "reading gth_rst\n";                                                                                    
    PCICard.read( "gth_rst", &value_2 );   
    std::cout << "result : " << std::hex << "0x" << value_2 << std::endl << std::endl;

    value_1 = 0x3;
    value_2 = 0x5;

    std::cout << "\nwriting  " << std::hex << "0x" << value_1 << " into sector\n";
    PCICard.write( "sector", value_1 );

    std::cout << "\nreading sector\n";
    PCICard.read( "sector", &value_1 );
    std::cout << "result : " << std::hex << "0x" << value_1 << std::endl << std::endl;
    
    std::cout << "writing  " << std::hex << "0x" << value_2 << " into  sector\n";
    PCICard.write( "sector", value_2 );   
    
    std::cout << "reading sector \n";                                                                                    
    PCICard.read( "sector", &value_2 );   
    std::cout << "result : " << std::hex << "0x" << value_2 << std::endl << std::endl;

    value_1 = 0x33;
    value_2 = 0x55;

    std::cout << "\nwriting  " << std::hex << "0x" << value_1 << " into spy_data_delay\n";
    PCICard.write( "spy_data_delay", value_1 );

    std::cout << "\nreading spy_data_delay\n";
    PCICard.read( "spy_data_delay", &value_1 );
    std::cout << "result : " << std::hex << "0x" << value_1 << std::endl << std::endl;
    
    std::cout << "writing  " << std::hex << "0x" << value_2 << " into  spy_data_delay\n";
    PCICard.write( "spy_data_delay", value_2 );   
    
    std::cout << "reading spy_data_delay \n";                                                                                    
    PCICard.read( "spy_data_delay", &value_2 );   
    std::cout << "result : " << std::hex << "0x" << value_2 << std::endl << std::endl;


/*
    // READ BLOCK
    uint64_t offset = 0x0;
    const int size=0x1000; // 4 KB
    char* write_buf = new char[(size)];

    //fill the buffer with random numbers
    uint64_t ic;
    for (ic = 0; ic<size; ic+=4) {
      *((uint64_t*)(&write_buf[ic])) = random();
    } 

    // this below prints the buffer content

    // write a block
    PCICard.writeBlock64( "test_mem", size, write_buf, HAL::HAL_NO_VERIFY, HAL::HAL_DO_INCREMENT, offset );

    char* read_buf = new char[size];
    PCICard.readBlock64( "test_mem", size, read_buf, HAL::HAL_DO_INCREMENT);

    int mismatch_cnt=0;
    for (int i = 0; i<size/4; i++)
      {
    	//std::cout << std::dec << read_buf[i] << "=" << write_buf[i] << std::endl;
     	if (read_buf[i] != write_buf[i])
     	  {
     	    mismatch_cnt++;
     	  }
      }

    std::cout << std::dec << "Found " << mismatch_cnt << " mismatches" << std::endl;
*/
  } catch ( HAL::HardwareAccessException& e ) {
    std::cout << "*** Exception occurred : " << std::endl;
    std::cout << e.what() << std::endl;
  } catch ( ... ) {
    std::cout << "*** Unknown exception occurred" << std::endl;
  }
  std::cout << "This is the end, my friend..." << std::endl;
  return 0;
}

