#include "hal/PCIDevice.hh"
#include "hal/PCIAddressTable.hh"
#include "hal/PCIAddressTableASCIIReader.hh"
#include "hal/PCIDummyBusAdapter.hh"
#include "hal/PCIExprLinuxBusAdapter.hh"

#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>

#define ADDRESSTABLE "address_table_ascii.dat"

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
    HAL::PCIDevice PCICard(addressTable, busAdapter, mtf7_index);
    
    // the single registers are 32 bits long (4 bytes)
    uint32_t value_1 = 0x12345678;
    uint32_t value_2 = 0x91234567;

    // WRITE SINGLE REGISTERS
    std::cout << "\nwriting  " << std::hex << "0x" << value_1 << " into test_reg1 \n";                                                                                    
    PCICard.write( "test_reg1", value_1 );   
    
    std::cout << "writing  " << std::hex << "0x" << value_2 << " into test_reg2 \n";
    PCICard.write( "test_reg2", value_2 );   
    
    // READ SINGLE REGISTERS
    std::cout << "\nreading  test_reg1 \n";                                                                                    
    PCICard.read( "test_reg1", &value_1 );   
    std::cout << "result : " << std::hex << std::setfill('0') << std::setw(8) << value_1 << std::endl << std::endl;

    std::cout << "reading  test_reg2 \n";                                                                                    
    PCICard.read( "test_reg2", &value_2 );   
    std::cout << "result : " << std::hex << std::setfill('0') << std::setw(8) << value_2 << std::endl << std::endl;

    /*
    // A TEST
    std::cout << "LB_MEM_SIZE="   << std::dec << LB_MEM_SIZE   <<std::endl;
    std::cout << "DATA_SIZE="     << std::dec << DATA_SIZE     <<std::endl;
    std::cout << "N_WRITES="      << std::dec << N_WRITES      <<std::endl; 
    std::cout << "N_WRITES_LONG=" << std::dec << N_WRITES_LONG <<std::endl;
 
    uint32_t write_buf[2][LB_MEM_SIZE/4]; //LB_MEM_SIZE/4=16384
    uint32_t read_buf[LB_MEM_SIZE/4];

    // clean the buffer
    memset(write_buf, 0x0, LB_MEM_SIZE);
    memset(read_buf, 0x0, LB_MEM_SIZE);

    srandom(time(NULL));

    // fill part of the buffer
    for (size_t i=0; i<DATA_SIZE/4; i++)
      {
        write_buf[0][i] = random();
        write_buf[1][i] = random();
      }

    for (int n = 0; n < N_WRITES_LONG; n++)
      {
	std::cout << std::dec << "n=" << n << " - n&1=" << (n&1) << std::endl;
        for (size_t i=0; i<LB_MEM_SIZE/4; i++)
	  {
            write_buf[n&1][i] = i+10;
	    std::cout << "write_buf[" << (n&1) << "][" << i << "]=" << write_buf[n&1][i] << std::endl;
	  }

        for (int j = 0; j < N_WRITES; j++)
	  {
	    //std::cout << "j=" << j << "mwrite bla bla" << std::endl;
	    //mwrite(fd, &(write_buf[n&1][j * DATA_SIZE/4]), DATA_SIZE, MEM_BASE + j * DATA_SIZE);
	    std::cout << "write_buf[" << (n&1) << "][" << j * DATA_SIZE/4 << "]=" << write_buf[n&1][j * DATA_SIZE/4] << ", " << DATA_SIZE << ", " << j * DATA_SIZE << std::endl;
	  }
      }
    */
    
    // READ BLOCK
    uint32_t offset = 0x0;
    const int size=0x1000; // 4 KB
    char* write_buf = new char[(size)];

    //fill the buffer with random numbers
    uint32_t ic;
    for (ic = 0; ic<size; ic+=4) {
      *((uint32_t*)(&write_buf[ic])) = random();
    } 

    // this below prints the buffer content

    // write a block
    PCICard.writeBlock( "test_mem", size, write_buf, HAL::HAL_NO_VERIFY, HAL::HAL_DO_INCREMENT, offset );

    //uint32_t* sourcePtr_w = (uint32_t*) write_buf;
    //uint32_t nWords = size/4;
    //for ( ic = 0; ic<nWords; ic++, sourcePtr_w++ ) {
    //  std::cout << "sourcePtr_w= " << std::dec << *sourcePtr_w << std::endl;
    //}

    // for (int i = 0; i<size/4; i++)
    //   {
    // 	std::cout << std::dec << write_buf[i] << std::endl;
    //   }


    // // read a block
    //char* read_buf_char = new char[size];
    //PCICard.readBlock( "test_mem", size, read_buf_char, HAL::HAL_DO_INCREMENT);

    // const int nWords = size/4;                                                                                                                                                                                  
    // uint32_t read_buf[nWords];
   
    // uint32_t* sourcePtr = (uint32_t*) read_buf_char;
    // for ( ic = 0; ic<nWords; ic++, sourcePtr++ ) {
    //   read_buf[ic] = *sourcePtr;                                                                                                                                               
    //   //std::cout << std::dec << "read_buf[" << ic << "]=" << read_buf[ic] << std::endl; 
    // }    

    char* read_buf = new char[size];
    PCICard.readBlock( "test_mem", size, read_buf, HAL::HAL_DO_INCREMENT);

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






    //char *p;
    //p = new char[100];
    //std::cin >> p;                      //forexample: haha
    //std::cout << "p = "  << p << std::endl;   //q = h
  
    //PCICard.readBlock( "test_mem", size, buffer, HAL::HAL_DO_INCREMENT);
    //std::cout << "test_mem : " << std::hex << std::setfill('0') << std::setw(8) << buffer << std::endl;

    // reading 
    // std::cout << "Reading bar0\n";
    // PCICard.read( "bar0", &value );
    // std::cout << "result : " << std::hex << std::setfill('0') << std::setw(8) << value << std::endl << std::endl;

    // std::cout << "Reading bar1\n";
    // PCICard.read( "bar1", &value );
    // std::cout << "result : " << std::hex << std::setfill('0') << std::setw(8) << value << std::endl;

    // std::cout << "Reading bar2\n";
    // PCICard.read( "bar2", &value );
    // std::cout << "result : " << std::hex << std::setfill('0') << std::setw(8) << value << std::endl;

    // std::cout << "Reading vendor_id\n";
    // PCICard.read( "Vendor_Id", &value );
    // std::cout << "result : " << std::hex << std::setfill('0') << std::setw(8) << value << std::endl;

    // std::cout << "Reading device_id\n";
    // PCICard.read( "Device_Id", &value );
    // std::cout << "result : " << std::hex << std::setfill('0') << std::setw(8) << value << std::endl;

    // reading memory blocks
    // const int size=0x400;
    // char* buffer = new char[size];
    
    // PCICard.readBlock( "screen_bar0", size, buffer, HAL::HAL_DO_INCREMENT);
    // std::cout << "screen_bar0 : " << std::hex << std::setfill('0') << std::setw(8) << buffer << std::endl;

  } catch ( HAL::HardwareAccessException& e ) {
    std::cout << "*** Exception occurred : " << std::endl;
    std::cout << e.what() << std::endl;
  } catch ( ... ) {
    std::cout << "*** Unknown exception occurred" << std::endl;
  }
  std::cout << "This is the end, my friend..." << std::endl;
  return 0;
}

