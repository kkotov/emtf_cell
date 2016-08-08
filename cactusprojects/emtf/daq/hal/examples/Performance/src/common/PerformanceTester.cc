#include <iostream>
#include <iomanip>

#include "hal/linux/StopWatch.hh"
#include "hal/VMEDummyBusAdapter.hh"
#include "hal/VMEAddressTableASCIIReader.hh"
#include "hal/VMEDevice.hh"
#include "hal/VMEAddressTable.hh"

#define ADDRESSTABLE "PerformanceTesterAddressTable.dat"

#define REPETITION 100000.0

int main() {
  try {
    HAL::StopWatch myWatch(10);
    HAL::VMEAddressTableASCIIReader reader( ADDRESSTABLE );
    HAL::VMEAddressTable addressTable( "test address table", reader );
    HAL::VMEDummyBusAdapter busAdapter(HAL::VMEDummyBusAdapter::VERBOSE_OFF, HAL::VMEDummyBusAdapter::MEMORY_MAP_OFF);
    HAL::VMEDevice dummyDevice( addressTable, busAdapter, 0x00000000 );
    int icount1;
    uint32_t value;
    double time;
    std::cout << "\n\n\n";
    std::cout << "===============================================================================\n";
    std::cout << "ATTENTION ==> the measured time values do not measure the process time but\n"
         << "              the total time elapsed during the measurement (laboratory-time)\n"
         << "              So, in order to get meaningfull measurements,  be sure not to  \n"
         << "              have other active processes on your machine." 
         << std::endl;
    std::cout << "===============================================================================";
    std::cout << "\n\n\n";
 
    //
    // test the poll rate
    //
    try {
      dummyDevice.pollItem ("rwFlagMask",
                            1,
                            1000,
                            &value);
    } catch( HAL::TimeoutException &e ) {
          std::cout << e.what() << std::endl;
    }
    std::cout << std::endl;
    //
    // test r/w-item with full mask (read)
    //
    myWatch.start();
    for ( icount1=0; icount1 < REPETITION; icount1++ ) {
      dummyDevice.read( "rwFullMask", &value );
    }
    myWatch.stop();
    time = myWatch.read() / REPETITION;
    std::cout << "read                      r/w-item with full mask : " << std::dec << time << "us" << std::endl;

    //
    // test r/w-item with full mask (read) and offset
    //
    myWatch.start();
    for ( icount1=0; icount1 < REPETITION; icount1++ ) {
      dummyDevice.read( "rwFullMask", &value, 0x04 );
    }
    myWatch.stop();
    time = myWatch.read() / REPETITION;
    std::cout << "read           r/w-item with full mask and offset : " << std::dec << time << "us" << std::endl;

    //
    // test r/w-item with full mask (write)
    //
    myWatch.start();
    for ( icount1=0; icount1 < REPETITION; icount1++ ) {
      dummyDevice.write( "rwFullMask", value );
    }
    myWatch.stop();
    time = myWatch.read() / REPETITION;
    std::cout << "write                     r/w-item with full mask : " << std::dec << time << "us" << std::endl;

    //
    // test r/w-item with full mask (write) and offset
    //
    myWatch.start();
    for ( icount1=0; icount1 < REPETITION; icount1++ ) {
      dummyDevice.write( "rwFullMask", value, HAL::HAL_NO_VERIFY, 0x04 );
    }
    myWatch.stop();
    time = myWatch.read() / REPETITION;
    std::cout << "write          r/w-item with full mask and offset : " << std::dec << time << "us" << std::endl;

    //
    // test r-item with full mask (read)
    // 
    myWatch.start();
    for ( icount1=0; icount1 < REPETITION; icount1++ ) {
      dummyDevice.read( "rFullMask", &value );
    }
    myWatch.stop();
    time = myWatch.read() / REPETITION;
    std::cout << "read                        r-item with full mask : " << std::dec << time << "us" << std::endl;
    
    //
    // test r-item with full mask (read) and offset
    // 
    myWatch.start();
    for ( icount1=0; icount1 < REPETITION; icount1++ ) {
      dummyDevice.read( "rFullMask", &value, 0x04 );
    }
    myWatch.stop();
    time = myWatch.read() / REPETITION;
    std::cout << "read             r-item with full mask and offset : " << std::dec << time << "us" << std::endl;
    
    //
    // test w-item with full mask (write)
    // 
    myWatch.start();
    for ( icount1=0; icount1 < REPETITION; icount1++ ) {
      dummyDevice.write( "wFullMask", value );
    }
    myWatch.stop();
    time = myWatch.read() / REPETITION;
    std::cout << "write                       w-item with full mask : " << std::dec << time << "us" << std::endl;

    //
    // test w-item with full mask (write) and offset
    // 
    myWatch.start();
    for ( icount1=0; icount1 < REPETITION; icount1++ ) {
      dummyDevice.write( "wFullMask", value, HAL::HAL_NO_VERIFY, 0x04 );
    }
    myWatch.stop();
    time = myWatch.read() / REPETITION;
    std::cout << "write             w-item with full mask and offset: " << std::dec << time << "us" << std::endl;

    //
    // test r/w-item with full mask (unmaskedRead)
    //
    myWatch.start();
    for ( icount1=0; icount1 < REPETITION; icount1++ ) {
      dummyDevice.unmaskedRead( "rwFullMask", &value );
    }
    myWatch.stop();
    time = myWatch.read() / REPETITION;
    std::cout << "unmaskedRead              r/w-item with full mask : " << std::dec << time << "us" << std::endl;

    //
    // test r/w-item with full mask (unmaskedRead) and offset
    //
    myWatch.start();
    for ( icount1=0; icount1 < REPETITION; icount1++ ) {
      dummyDevice.unmaskedRead( "rwFullMask", &value, 0x04 );
    }
    myWatch.stop();
    time = myWatch.read() / REPETITION;
    std::cout << "unmaskedRead   r/w-item with full mask and offset : " << std::dec << time << "us" << std::endl;

    //
    // test r/w-item with full mask (unMaskedWrite)
    //
    myWatch.start();
    for ( icount1=0; icount1 < REPETITION; icount1++ ) {
      dummyDevice.unmaskedWrite( "rwFullMask", value );
    }
    myWatch.stop();
    time = myWatch.read() / REPETITION;
    std::cout << "unmaskedWrite             r/w-item with full mask : " << std::dec << time << "us" << std::endl;

    //
    // test r/w-item with full mask (unMaskedWrite) and offset
    //
    myWatch.start();
    for ( icount1=0; icount1 < REPETITION; icount1++ ) {
      dummyDevice.unmaskedWrite( "rwFullMask", value, HAL::HAL_NO_VERIFY, 0x04 );
    }
    myWatch.stop();
    time = myWatch.read() / REPETITION;
    std::cout << "unmaskedWrite  r/w-item with full mask and offset : " << std::dec << time << "us" << std::endl;

    //
    // test r/w-item with single bit mask (setBit)
    //
    myWatch.start();
    for ( icount1=0; icount1 < REPETITION; icount1++ ) {
      dummyDevice.setBit( "rwFlagMask" );
    }
    myWatch.stop();
    time = myWatch.read() / REPETITION;
    std::cout << "setBit                    r/w-item with 1bit mask : " << std::dec << time << "us" << std::endl;

    //
    // test r/w-item with single bit mask (setBit) and offset
    //
    myWatch.start();
    for ( icount1=0; icount1 < REPETITION; icount1++ ) {
      dummyDevice.setBit( "rwFlagMask", HAL::HAL_NO_VERIFY, 0x04 );
    }
    myWatch.stop();
    time = myWatch.read() / REPETITION;
    std::cout << "setBit         r/w-item with 1bit mask and offset : " << std::dec << time << "us" << std::endl;

    //
    // test r/w-item with single bit mask (resetBit)
    //
    myWatch.start();
    for ( icount1=0; icount1 < REPETITION; icount1++ ) {
      dummyDevice.resetBit( "rwFlagMask" );
    }
    myWatch.stop();
    time = myWatch.read() / REPETITION;
    std::cout << "resetBit                  r/w-item with 1bit mask : " << std::dec << time << "us" << std::endl;

    //
    // test r/w-item with single bit mask (resetBit) and offset
    //
    myWatch.start();
    for ( icount1=0; icount1 < REPETITION; icount1++ ) {
      dummyDevice.resetBit( "rwFlagMask", HAL::HAL_NO_VERIFY, 0x04 );
    }
    myWatch.stop();
    time = myWatch.read() / REPETITION;
    std::cout << "resetBit       r/w-item with 1bit mask and offset : " << std::dec << time << "us" << std::endl;

    //
    // test w-item with single bit mask (setBit)
    //
    myWatch.start();
    for ( icount1=0; icount1 < REPETITION; icount1++ ) {
      dummyDevice.setBit( "wFlagMask" );
    }
    myWatch.stop();
    time = myWatch.read() / REPETITION;
    std::cout << "setBit                      w-item with 1bit mask : " << std::dec << time << "us" << std::endl;

    //
    // test w-item with single bit mask (setBit) and offset
    //
    myWatch.start();
    for ( icount1=0; icount1 < REPETITION; icount1++ ) {
      dummyDevice.setBit( "wFlagMask", HAL::HAL_NO_VERIFY, 0x04 );
    }
    myWatch.stop();
    time = myWatch.read() / REPETITION;
    std::cout << "setBit           w-item with 1bit mask and offset : " << std::dec << time << "us" << std::endl;

    //
    // test w-item with single bit mask (resetBit)
    //
    myWatch.start();
    for ( icount1=0; icount1 < REPETITION; icount1++ ) {
      dummyDevice.resetBit( "wFlagMask" );
    }
    myWatch.stop();
    time = myWatch.read() / REPETITION;
    std::cout << "resetBit                    w-item with 1bit mask : " << std::dec << time << "us" << std::endl;

    //
    // test w-item with single bit mask (resetBit) and offset
    //
    myWatch.start();
    for ( icount1=0; icount1 < REPETITION; icount1++ ) {
      dummyDevice.resetBit( "wFlagMask", HAL::HAL_NO_VERIFY, 0x04 );
    }
    myWatch.stop();
    time = myWatch.read() / REPETITION;
    std::cout << "resetBit         w-item with 1bit mask and offset : " << std::dec << time << "us" << std::endl;

     std::cout << "===============================================================================";
    std::cout << "\n\n\n";


  } catch( HAL::HardwareAccessException& e ) {
    std::cout << e.what() << std::endl;
  }

}
