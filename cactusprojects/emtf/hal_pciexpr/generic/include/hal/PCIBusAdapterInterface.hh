#ifndef __PCIBusAdapterInterface
#define __PCIBusAdapterInterface

#include <vector>

#include "hal/PCIDeviceIdentifier.hh"
#include "hal/PCIAddressTable.hh"
#include "hal/BusAdapterException.hh"
#include "hal/NoSuchDeviceException.hh"
#include "hal/UnsupportedException.hh"
#include "hal/HardwareDeviceInterface.hh"

namespace HAL {

/**
*
*
*     @short Interface to the PCI-bus hardware.
*            
*            The PCIBusAdapterInterface is  the software interface for
*            classes which  actually talk to the  hardware, i.e. which
*            transform  the actual  PCI  transfer. Its  implementation
*            performs all  operations which  are happening on  the PCI
*            bus.   This means  it is  able to  find  Hardware Devices
*            plugged into  the PCI bus,  and it performs  the hardware
*            accesses (currently  only memory and  configuration space
*            accesses are supported).
*            
*            Every PCIDevice has  a handle to a PCIBusAdapterInterface
*            which it uses to perform the read and write accesses.
*
*       @see PCIDevice
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class PCIBusAdapterInterface {
public :

  /**
   * Virtual destructor of the class.
   */
  virtual ~PCIBusAdapterInterface() {};

  /**
   * Find a specific hardware on the PCI Bus.
   *
   * The function first tries to find a specific device on the PCI
   * Bus. For that the user has to provide the vendorID, deviceID 
   * and the index of the device (the index is a number from 0 
   * onwards and devices of the same kind which are plugged into 
   * the same bus.) If the device has been found succesfully it is 
   * registered with the BusAdapter.
   * For this the user  has to supply the AddressTable as an 
   * argument, in order to facilitate address space mapping by the 
   * busAdapter (though: BusAdapter are not forced to do this). 
   * THE ADDRESSTABLE MUST CONTAIN THE HIGHEST ACCESSABLE
   * ADDRESSOFFSET FOR EACH BASEADDRESS. Therefore if for example to
   * one bar the device maps a memory region, the AddressTable must
   * contain entries for the start AND the end of the memory.
   * @param  vendorID is the corresponding PCI parameter of your module.
   * @param  deviceID is the corresponding PCI parameter of your module.
   * @param  index is used if you have more than one module of the 
   *         same kind in your PCI bus. Index numberes these modules
   *         starting from '0'. Put this parameter to '0' if you 
   *         work with unique modules (i.e. all modules have different
   *         vendorID or deviceID)
   * @param  deviceIdentifierPtr is a handle (pointer to a pointer)
   *         to a data structure which contains device relevant 
   *         information which the BusAdapter needs to perform 
   *         configurationspace operations. NOTE: The responsibility
   *         for this DeviceIdentifier stays in the BusAdapter (it
   *         will be deleted there if the device is not neede 
   *         anymore)
   * @param  barRegisters will be loaded with the values of the 
   *         BAR offset registers in the configuration space of the 
   *         device.
   * @param  swapFlag can be set ti true in order to work with big-endian
   *         pci devices.
   */
  virtual void findDeviceByVendor( uint32_t vendorID, 
				   uint32_t deviceID,
				   uint32_t index,
				   const PCIAddressTable& pciAddressTable,
				   PCIDeviceIdentifier** deviceIdentifierPtr,
				   std::vector<uint32_t>& barRegisters,
				   bool swapFlag = false) 
    throw( NoSuchDeviceException,
           BusAdapterException ) = 0;
  /**
   * Find a specific hardware on the PCI Bus.
   *
   * The function first tries to find a specific device on the PCI
   * Bus. For that the user has to provide the busID, slotID 
   * and the fucntionID of the device. On a linux system these 
   * parameters can be found with the "lspci" command. If the device
   * has been found succesfully it is registered with the BusAdapter.
   * For this the user  has to supply the AddressTable as an 
   * argument, in order to facilitate address space mapping by the 
   * busAdapter (though: BusAdapter are not forced to do this). 
   * THE ADDRESSTABLE MUST CONTAIN THE HIGHEST ACCESSABLE
   * ADDRESSOFFSET FOR EACH BASEADDRESS. Therefore if for example to
   * one bar the device maps a memory region, the AddressTable must
   * contain entries for the start AND the end of the memory.
   * @param  busID is the bus number of the PCI bus containing the module.
   * @param  slotID is the slot number in the PCI bus (OS dependend!).
   * @param  functionID is the PCI function of the device. If a device
   *         contains two different functions they must be represented
   *         with two different PCIDevices. 
   * @param  deviceIdentifierPtr is a handle (pointer to a pointer)
   *         to a data structure which contains device relevant 
   *         information which the BusAdapter needs to perform 
   *         configurationspace operations. NOTE: The responsibility
   *         for this DeviceIdentifier stays in the BusAdapter (it
   *         will be deleted there if the device is not neede 
   *         anymore)
   * @param  barRegisters will be loaded with the values of the 
   *         BAR offset registers in the configuration space of the 
   *         device.
   * @param  swapFlag can be set ti true in order to work with big-endian
   *         pci devices.
   */
  virtual void findDeviceByBus( uint32_t busID, 
				uint32_t slotID,
				uint32_t functionID,
				const PCIAddressTable& pciAddressTable,
				PCIDeviceIdentifier** deviceIdentifierPtr,
				std::vector<uint32_t>& barRegisters,
				bool swapFlag = false )
    throw( BusAdapterException,
           NoSuchDeviceException ) = 0;
  
  /**
   * Initialization for MT6/7
   *
   */
  virtual void findDeviceByIndex( uint32_t index) 
    throw( BusAdapterException,
	   NoSuchDeviceException ) = 0;

  /**
   * Unregister a device from the BusAdapter.
   * If a device is not needed anymore it must be unregistered so that
   * the memory mapping (if it has been used) can be deleted. This 
   * operation also deletes the deviceIdentifier.
   */
  virtual void closeDevice( PCIDeviceIdentifier* deviceIdentifier ) 
    throw( BusAdapterException ) = 0;

  /**
   * The interface for memory write operations.
   * @param device is needed for drivers which do not map the modules
   *        AddressSpace into memory. It contains the information
   *        necessary for the BusAdapter to identify the hardware device. 
   */
  virtual void write( PCIDeviceIdentifier& device, 
                      uint32_t address,
                      uint32_t data) 
    throw( BusAdapterException ) = 0;

  /**
   * The interface for memory read operations.
   * @param device is needed for drivers which do not map the modules
   *        AddressSpace into memory. It contains the information
   *        necessary for the BusAdapter to identify the hardware device. 
   */
  virtual void read( PCIDeviceIdentifier& device, 
                     uint32_t address,
                     uint32_t* result) 
    throw( BusAdapterException ) = 0;
 /** 
  * The interface for configuration space write operations.
  * @param device is needed since configuration-space operations are
  *        usually not memory mapped. It contains the information
  *        necessary for the BusAdapter to identify the hardware device. 
  */
  virtual void configWrite( PCIDeviceIdentifier& device, 
                            uint32_t address, 
                            uint32_t data ) 
    throw( BusAdapterException ) = 0;
 /** 
  * The interface for configuration space read operations.
  * @param device is needed since configuration-space operations are
  *        usually not memory mapped. It contains the information
  *        necessary for the BusAdapter to identify the hardware device. 
  */
  virtual void configRead( PCIDeviceIdentifier& device,
                           uint32_t address, 
                           uint32_t* result )
    throw( BusAdapterException ) = 0;

  /**
   * Interface for writing a block of data to a PCI device.
   *
   * A PCIBusAdapter should implement this function in the fasted 
   * possible way. If the hardware of the BusAdapter contains DMA
   * engines, those should be used in order to perform the transfer.
   * Note that this is a blocking call so that the busAdapter might
   * poll the DMA engine for completion of the DMA. Implementations
   * with Interrupts are also possible if the BusAdapter is used in 
   * a "very hard realtime" environment. Note that this library does
   * not foresee that the user has to deal in any way with the interrupt
   * of the "End-Of-DMA".
   *
   * For hardware without DMA engines (like standard PCs) this routine 
   * must be implemented with single accesses. Depending on the driver
   * and the operating system an implementation might consider to 
   * implement this call with memory mapping in order to speed up the 
   * transfer. 
   *
   * Read the documentation of in the VMEDevice for this call for 
   * further details on this call and its parameters.
   */
  virtual void writeBlock(  PCIDeviceIdentifier& device,
                            uint32_t startAddress,
                            uint32_t length,
                            char *buffer,
                            HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT )
    throw( BusAdapterException,
           UnsupportedException ) = 0;
  /**
   * Interface for reading a block of data from a PCI device.
   *
   * A PCIBusAdapter should implement this function in the fasted 
   * possible way. If the hardware of the BusAdapter contains DMA
   * engines, those should be used in order to perform the transfer.
   * Note that this is a blocking call so that the busAdapter might
   * poll the DMA engine for completion of the DMA. Implementations
   * with Interrupts are also possible if the BusAdapter is used in 
   * a "very hard realtime" environment. Note that this library does
   * not foresee that the user has to deal in any way with the interrupt
   * of the "End-Of-DMA".
   *
   * For hardware without DMA engines (like standard PCs) this routine 
   * must be implemented with single accesses. Depending on the driver
   * and the operating system an implementation might consider to 
   * implement this call with memory mapping in order to speed up the 
   * transfer. 
   *
   * Read the documentation of in the VMEDevice for this call for 
   * further details on this call and its parameters.
   */
  virtual void readBlock(  PCIDeviceIdentifier& device,
                           uint32_t startAddress,
                           uint32_t length,
                           char *buffer,
                           HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT )
  throw( BusAdapterException,
         UnsupportedException ) = 0;
};
} /* namespace HAL */

#endif /* __PCIBusAdapterInterface */
