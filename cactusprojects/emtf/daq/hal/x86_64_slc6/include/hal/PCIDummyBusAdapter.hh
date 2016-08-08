#ifndef __PCIDummyBusAdapter
#define __PCIDummyBusAdapter

#include <vector>
#include <iostream>

#include "hal/PCIBusAdapterInterface.hh"
#include "hal/PCIDummyDeviceIdentifier.hh"

namespace HAL {

/**
*
*
*     @short A dummy PCIBusAdapter in order to work without hardware.
*            
*            This  BusAdapter  merely  serves  to  let  you  test  you 
*            software  without actually  having hardware  connected to 
*            your  computer.  All actions  which  in  real life  would 
*            result in  read or write  operations will be  reported on 
*            the standard output.
*
*       @see BusAdapterInterface
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/06 09:54:25 $
*
*
**/

class PCIDummyBusAdapter : public PCIBusAdapterInterface {

public:

  /**
   * This enumerator is used in the constructor. It switches 
   * the versbose behaviour of the BusAdapter on or off. Note
   * that the enumerator is a member of the class PCIDummyBusAdapter.
   * So it must be used. This means that a Scope identifier is
   * needed when calling the constructor of the class. A valid 
   * example would be:
   * <pre>
   * PCIDummyBusAdapter myBa( PCIDummyBusAdapter::VERBOSE_ON,
   *                          PCIDummyBusAdapter::MEMORY_MAP_OFF );
   * </pre>
   * The enumerators have been put into the class in order not
   * to contaminate the global namesapce with objects merely used 
   * for debugging purposes.
   */
  enum VerboseMode { VERBOSE_OFF, VERBOSE_ON };

  /**
   * This enumerator is used in the constructor. It switches 
   * the memory-mapping behaviour of the BusAdapter on or off. 
   * If switched on every address-space  corresponding to a 
   * specific BAR or to the configuration space is mapped into
   * the local memory of the host. It is then used to perform
   * read and write operations into the mapped memory. This 
   * allows to play in a more realistic way with the device.
   * See VerboseMode comments for a note on how to use this 
   * enumerator.
   */
  enum MemoryMode  { MEMORY_MAP_OFF, MEMORY_MAP_ON };

  /**
   * The constructor takes two arguments:
   * @param verbose allows you to monitor all data transfers: If 
   *        set to true every action is reported on the standard
   *        output.
   * @param memoryMode allows you to create memory regions which
   *        fake the hardware device. For every openDevice call
   *        a memory region is created into which or from which 
   *        data is transfered. With this you can write and read
   *        back data from you dummy-device. The size of the 
   *        mapped region depend on the PCIAddressTable given
   *        to the openDevice call. 
   */
  PCIDummyBusAdapter( enum VerboseMode verbose    = VERBOSE_ON,
                      enum MemoryMode  memoryMode = MEMORY_MAP_ON,
		      std::ostream& os = std::cout );

  /**
   * There is nothing to do in the desctructor.
   */
  virtual ~PCIDummyBusAdapter();


  /**
   * If the PCIDummyBusAdapter was created with the memoryMode
   * option set to true, the call scans the PCIAddressTable in
   * order to find out how much memory has to be reserved for 
   * the dummy Hardware device. The pointers to this memory is 
   * then contained in the deviceIdentifier. For each BAR used
   * in the pciAddressTable a separate memory region is created.
   * Similarly if there are items of the configuration space in
   * the pciAddressTable, a separate memory region is created. 
   */
  void findDeviceByVendor( uint32_t vendorID, 
			   uint32_t deviceID,
			   uint32_t index,
			   const PCIAddressTable& pciAddressTable,
			   PCIDeviceIdentifier** deviceIdentifierPtr,
			   std::vector<uint32_t>& barRegisters,
			   bool swapFlag = false ) 
    throw();
  
  void findDeviceByBus( uint32_t busID, 
			uint32_t slotID,
			uint32_t functionID,
			const PCIAddressTable& pciAddressTable,
			PCIDeviceIdentifier** deviceIdentifierPtr,
			std::vector<uint32_t>& barRegisters,
			bool swapFlag = false )
    throw();

  /**
   * Here the memory regions which might have beeen reserved
   * in the openDevice call, are deleted.
   */
  void closeDevice( PCIDeviceIdentifier* deviceIdentifier )
    throw();
  
  void write( PCIDeviceIdentifier& device,
              uint32_t address,
              uint32_t data)
    throw();
  
  void read( PCIDeviceIdentifier& DeviceIdentifier,
             uint32_t address,
             uint32_t* result)
    throw();

  void configWrite( PCIDeviceIdentifier& device, 
                    uint32_t address, 
                    uint32_t data ) 
    throw( ) ;

  void configRead( PCIDeviceIdentifier& device,
                   uint32_t address, 
                   uint32_t* result )
    throw( );

  void writeBlock( PCIDeviceIdentifier& deviceIdentifierPtr,
                   uint32_t startAddress,
                   uint32_t length,      // in bytes
                   char *buffer,
                   HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT )
    throw();
  
  void readBlock( PCIDeviceIdentifier& deviceIdentifierPtr,
                  uint32_t startAddress,
                  uint32_t length,      // in bytes
                  char *buffer,
                  HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT )
    throw();

private:
  // helpers:
  void findDevice( const PCIAddressTable& pciAddressTable,
		   PCIDeviceIdentifier** deviceIdentifierPtr,
		   std::vector<uint32_t>& barRegisters,
		   bool swapFlag )
    throw();

  uint32_t getBARIntervall( const std::vector<uint32_t>& maxAddresses);

private:
  uint32_t deviceNumberCounter;
  enum VerboseMode verbose;
  enum MemoryMode  memoryMode;
  std::ostream& os_;
};

} /* namespace HAL */

#endif /* __PCIDummyBusAdapter */
