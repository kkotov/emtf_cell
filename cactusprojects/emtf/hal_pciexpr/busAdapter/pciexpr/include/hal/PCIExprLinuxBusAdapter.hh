#ifndef __PCIExprLinuxBusAdapter
#define __PCIExprLinuxBusAdapter

#include <vector>
#include "hal/HardwareDeviceInterface.hh"
#include "hal/PCIBusAdapterInterface.hh"
#include "hal/PCIExprLinuxDeviceIdentifier.hh"
#include "hal/NoSuchDeviceException.hh"
#include "hal/BusAdapterException.hh"
#include "hal/UnsupportedException.hh"
#include "xpci/Bus.h"
#include "xpci/Address.h"
#include "byteswap.h"

#include <fcntl.h>

//#include "/home/hvuser/projects/sp12_qt/sp12_init.h"
#define mwrite(a,b,c,d) if (pwrite (a,b,c,d) != c) printf("pwrite error: w: %d file: %s line: %s", c, __FILE__, __LINE__);
#define mread(a,b,c,d) if (pread (a,b,c,d) != c) printf("pread error: w: %d file: %s line: %s", c, __FILE__, __LINE__);

#define PCIEXPR_DEFAULT_FILE "/dev/utca_sp12"

namespace HAL {

/**
*
*
*     @short A memory mapped PCI bus adapter for use in Linux PCs.
*            
*            This  class implements  the  PCIBusAdapterInterface using
*            the  I2O-core library  developed  for accessing  hardware
*            plugged into  Linux PCs.  The functions are  described in
*            detail in the PCIBusAdapterInterface.
*            
*            This  BusAdapter  uses memory  mapping  in  order to  map 
*            memory space  accesses to the user  space. This optimizes 
*            the access time for the user. 
*
*       @see PCIBusAdapterInterface, I2O core library documentation
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 17:54:12 $
*
*
**/

class PCIExprLinuxBusAdapter : public PCIBusAdapterInterface {
public:

  PCIExprLinuxBusAdapter() throw (HAL::BusAdapterException);

  virtual ~PCIExprLinuxBusAdapter();

  /**
   * Registers the device with the i2ocore driver and maps it into
   * the address space of the user program. The vector barRegisters
   * which is returned by this routine contains the base addresses 
   * to be used by the user in order to access the memory reagions
   * corresponding to the various PCI-BARs. (All this is of course 
   * automatically handled by the HAL-library.)
   * @param swapFlag can be given if the device does only handle 
   *        big endian accesses. (The Myrinet Lanai9 card is such
   *        an exotic card.) If this option is set to true all 
   *        data accesses for this device are byte-swapped.
   */
  void findDeviceByVendor( uint32_t vendorID, 
			   uint32_t deviceID,
			   uint32_t index,
			   const PCIAddressTable& pciAddressTable,
			   PCIDeviceIdentifier** deviceIdentifierPtr,
			   std::vector<uint32_t>& barRegisters,
			   bool swapFlag = false )
    throw( BusAdapterException,
           NoSuchDeviceException );
  
  /**
   * Registers the device with the i2ocore driver and maps it into
   * the address space of the user program. The vector barRegisters
   * which is returned by this routine contains the base addresses 
   * to be used by the user in order to access the memory reagions
   * corresponding to the various PCI-BARs. (All this is of course 
   * automatically handled by the HAL-library.)
   * @param swapFlag can be given if the device does only handle 
   *        big endian accesses. (The Myrinet Lanai9 card is such
   *        an exotic card.) If this option is set to true all 
   *        data accesses for this device are byte-swapped.
   */
  void findDeviceByBus( uint32_t busID, 
			uint32_t slotID,
			uint32_t functionID,
			const PCIAddressTable& pciAddressTable,
			PCIDeviceIdentifier** deviceIdentifierPtr,
			std::vector<uint32_t>& barRegisters,
			bool swapFlag = false )
    throw( BusAdapterException,
           NoSuchDeviceException );

  /** 
   * MTF6/7 specific
   *
   */
  void findDeviceByIndex( uint32_t index )
    throw( BusAdapterException,
           NoSuchDeviceException );

  
  /**
   * Destroys the deviceIdentifier and releases the resources taken
   * for the memory mapping.
   */
  void closeDevice( PCIDeviceIdentifier* deviceIdentifier )
    throw();

  void write( PCIDeviceIdentifier& device,
              uint32_t address,
              uint32_t data)
    throw( BusAdapterException );
  
  void read( PCIDeviceIdentifier& device,
             uint32_t address,
             uint32_t* result)
    throw( BusAdapterException );

  void configWrite( PCIDeviceIdentifier& device, 
                    uint32_t address, 
                    uint32_t data )
    throw( BusAdapterException );

  void configRead( PCIDeviceIdentifier& device,
                   uint32_t address, 
                   uint32_t* result )
    throw( BusAdapterException );

  void writeBlock(  PCIDeviceIdentifier& device,
                    uint32_t startAddress,
                    uint32_t length,
                    char *buffer,
                    HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT ) 
    throw( BusAdapterException );

  void readBlock(  PCIDeviceIdentifier& device,
                   uint32_t startAddress,
                   uint32_t length,
                   char *buffer,
                   HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT )
  throw( BusAdapterException );

private:
  /**
   * Common Helper funcition for the public findDeviceByXXX methods. 
   */
  void findDevice( xpci::Address& deviceConfigAddress,
                   const PCIAddressTable& pciAddressTable,
                   PCIDeviceIdentifier** deviceIdentifierPtr,
                   std::vector<uint32_t>& barRegisters,
		   bool swapFlag = false )
    throw( BusAdapterException,
           NoSuchDeviceException );

  xpci::Bus pciBus_; 
  int device_d;

};

} /* namespace HAL */

#endif /* __PCIExprLinuxBusAdapter */
