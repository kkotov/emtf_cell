#ifndef __PCIDevice
#define __PCIDevice

#include <string>
#include <vector>

#include "hal/IllegalValueException.hh"
#include "hal/PCIAddressTable.hh"
#include "hal/HardwareDevice.hh"
#include "hal/PCIDeviceIdentifier.hh"
#include "hal/PCIBusAdapterInterface.hh"

namespace HAL {

/**
*
*
*     @short Implementation of a PCI Hardware device.
*            
*            This  class is  the first  technology dependent  class in 
*            the  class  hierachy. (But  it  does  not  depend on  the 
*            specific  hardware  to  use  the technology:  It  is  PCI 
*            specific  but does  not depend  on the  implementation of 
*            the PCI-interface.) 
*            
*            The  PCIDevice  implements the  interface  to access  the 
*            hardware defined in the  HardwareDevice class for the PCI
*            technology. 
*
*       @see HardwareDeviceInterface, HardwareDevice
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class PCIDevice : public HardwareDevice {

public:

  typedef struct PCILocationIdentifier_s {
    uint32_t busID;
    uint32_t slotID;
    uint32_t functionID;
  } PCILocationIdentifier;
      
  /**
   * The Constructor of a PCI device if the vendorID and deviceID are given.
   * The  constructor  needs  all  parameters  which  unambiguously
   * define  the device  and the  methods how  to access  it  in the
   * specific hardware implementation. The user must make sure that the
   * addressTable exists as long as the PCIDevice (only a reference is
   * passed to the PCIDevice so that a single table can be shared among
   * several PCIDevices.) The same is valid for the busAdapter.
   *
   * @param addressTable defines all accessable items of the 
   *        device.
   * @param pciBusAdapter actually transfers data between the 
   *        device and the software. It depends on the specific
   *        hardware implementation of the PCI interface and on
   *        the operating system. 
   * @param vendorID is the corresponding value in the 
   *        configuration space of the device.
   * @param deviceID is the corresponding value in the 
   *        configuration space of the device.
   * @param index identifies the device unambigiously in the
   *        system (there could be more than one module of 
   *        the same kind plugged into the same PCI bus).
   * @param swapFlag can be set in order to use big-endian devices
   *        with the HAL library. (Usually PCI peripherals are 
   *        little endian, but there are exotic cards like the 
   *        Myrinet NICs with the LANAI9 processor which are big endian.
   */
  PCIDevice( PCIAddressTable & addressTable,
             PCIBusAdapterInterface & pciBusAdapter,
             uint32_t vendorID,
             uint32_t deviceID,
             uint32_t index,
	     bool swapFlag = false);

  /**
   * VxWorks which is supported by this library is not plug and play. 
   * This means that the BaseAddresses are not determined automatically
   * by the operating system, but the user has to give them to the 
   * system by hand. For this case there exists the following constructor.
   * @param baseAddresses is assumed to contain the desired BARs for the
   *        module. When the device is opened (in a VxWorks specific 
   *        BusAdapter) the busAdapter performs configuration space write
   *        cycles in order to set the baseAddresses and the command 
   *        register in configurationspace of the device. 
   * @param command contains the 10 bits of the equally named bitfield in the
   * PCI space to define the access modes of the module. 
   */
  PCIDevice( PCIAddressTable & addressTable,
             PCIBusAdapterInterface & pciBusAdapter,
             uint32_t vendorID,
             uint32_t deviceID,
             uint32_t index,
             std::vector<uint32_t>& baseAddresses,
             uint32_t command );


   /**
   * The Constructor of a PCI device if the BusID, slotID and functionID
   * of the device are given.
   * The  constructor  needs  all  parameters  which  unambiguously
   * define  the device  and the  methods how  to access  it  in the
   * specific hardware implementation. The user must make sure that the
   * addressTable exists as long as the PCIDevice (only a reference is
   * passed to the PCIDevice so that a single table can be shared among
   * several PCIDevices.) The same is valid for the busAdapter.
   *
   * @param addressTable defines all accessable items of the 
   *        device.
   * @param pciBusAdapter actually transfers data between the 
   *        device and the software. It depends on the specific
   *        hardware implementation of the PCI interface and on
   *        the operating system. 
   * @param location identifies the position in which the hardware 
   *        module is plugged and the function of the module which
   *        should be addressed with this PCIDevice.
   * @param swapFlag can be set in order to use big-endian devices
   *        with the HAL library. (Usually PCI peripherals are 
   *        little endian, but there are exotic cards like the 
   *        Myrinet NICs with the LANAI9 processor which are big endian.
   */
  PCIDevice( PCIAddressTable & addressTable,
	     PCIBusAdapterInterface & pciBusAdapter,
	     const PCILocationIdentifier& location,
	     bool swapFlag );

  /**
   * Method needed for the MTF6/7 identification
   *
   **/
  PCIDevice( PCIAddressTable & addressTable,
	     PCIBusAdapterInterface & pciBusAdapter,
	     uint32_t index );

  virtual ~PCIDevice();

  /**
   * Implementation  of  the  abstract function  in  HardwareDevice.
   * Accesses  to  the configuration  space  are excecuted  directly
   * here.   Accesses  to the  memory  space  are  delegated to  the
   * memoryWrite function.  (The reason for this is explained in the
   * memoryWrite documentation.)
   */
  void hardwareWrite( const GeneralHardwareAddress& pciAddress, 
                      uint32_t data,
                      uint32_t offset = 0 ) const
    throw (IllegalValueException,
           BusAdapterException);
  /**
   * Implementation  of  the  abstract function  in  HardwareDevice.
   * Accesses  to  the configuration  space  are excecuted  directly
   * here.   Accesses  to the  memory  space  are  delegated to  the
   * memoryRead function.  (The reason  for this is explained in the
   * memoryWrite documentation.)
   */
  void hardwareRead( const GeneralHardwareAddress& pciAddress, 
                     uint32_t* result,
                     uint32_t offset = 0 ) const
    throw (IllegalValueException,
           BusAdapterException);

  /**
   * A function to perform a PCI memory-write.
   * This  function is used  by the  hardwareWrite function.  It has
   * been  made  public so  that  time  critical applications  which
   * cannot  afford  the  overhead  in  the  HardwareDeviceInterface
   * functions can  call this method directly. The  only overhead of
   * this function is to check  the barID for validity (i.e. that it
   * is in the range from 0...5). It is strongly recommended to use this 
   * function only if ABSOLUTELY necessary (in fact using this function
   * bypasses almost all services and "sanity" checks of the HAL.)
   *
   *
   * @param address is the the plain address with respect to the BAR. 
   * @param barId chooses one out of the six PCI BARS (0...5)
   */
  void memoryWrite( uint32_t address,
                    uint32_t barId,
                    uint32_t data ) const
    throw (IllegalValueException,
           BusAdapterException);
  
  /**
   * A function to perform a PCI memory-read.
   * This function is used by the hardwareREAD function. It has been
   * made  public so  that time  critical applications  which cannot
   * afford  the overhead  in the  HardwareDeviceInterface functions
   * can  call  this method  directly.  The  only  overhead of  this
   * function is to check the barID for validity (i.e. that it is in
   * the range from 0...5). It is strongly recommended to use this 
   * function only if ABSOLUTELY necessary (in fact using this function
   * bypasses almost all services and "sanity" checks of the HAL.)
   *
   * @param address is the the plain address with respect to the BAR. 
   * @param barId chooses one out of the six PCI BARS (0...5)
   */
  void memoryRead( uint32_t address,
                   uint32_t barId,
                   uint32_t* result ) const
    throw (IllegalValueException,
           BusAdapterException);

   /**
   * A function to perform a PCI config-write. 
   * This method is provided so  that time  critical applications  which
   * cannot  afford  the  overhead  in the  HardwareDeviceInterface
   * functions can  call this method directly. It is strongly recommended to use this 
   * function only if ABSOLUTELY necessary (in fact using this function
   * bypasses almost all services and "sanity" checks of the HAL.)
   *
   * This function was introduced to accelerate the JAL of Hannes
   * Sakulin.
   *
   * @param address is the the plain address with respect to the BAR. 
   */
  void configWrite( uint32_t address,
                    uint32_t data ) const
    throw (BusAdapterException);
  
  /**
   * A function to perform a PCI config-read.
   * This method is provided so  that time  critical applications  which cannot
   * afford  the overhead  in the  HardwareDeviceInterface functions
   * can  call  this method  directly. It is strongly recommended to use this 
   * function only if ABSOLUTELY necessary (in fact using this function
   * bypasses almost all services and "sanity" checks of the HAL.)
   *
   * This function was introduced to accelerate the JAL of Hannes
   * Sakulin.
   *
   * @param address is the the plain address with respect to the BAR. 
   */
  void configRead( uint32_t address,
                   uint32_t* result ) const
    throw (BusAdapterException);


 /**
   * Implementation of the abstract function in HardwareDevice.
   * Here you will only find PCI specific (BUT IMPORTANT) issues of 
   * this function. Refer to the HardwareDeviceInterface for general
   * documentation.
   *
   * IMPORTANT NOTE: If you read out 
   * a Fifo, you have to be sure that possible retry cylces on 
   * the PCI bus do not corrupt the data of your Fifo (that means
   * it is not always clear that a PCI interface can be programmed
   * not to repeat a local access to a Fifo in this case. This means
   * that one data word of the Fifo (the one corresponding to the 
   * aborted cycle) is lost. This is a known problem in PCI 
   * technology but there are of course implementations which handle
   * these cases correctly. This remark has only been added here in 
   * order to put you in an alert state if you deal with Fifos on PCI
   * busses.
   *
   * @param addressBehaviour determines if the address in the hardware
   *        device is incremented during the data transfer or not. (The
   *        latter you need if you read out a Fifo.) BusAdapters for 
   *        hardware which contain DMA machines have to program the 
   *        DMA engines accordingly. 
   * @throws IllegalOperationException is thrown if the function is 
   *        called on an item other than a memory space item.
   *
   */
  void hardwareReadBlock( const GeneralHardwareAddress& pciAddress, 
                          uint32_t length,
                          char *buffer,
                          HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT,
                          uint32_t offset = 0) const
    throw (IllegalValueException, 
           UnsupportedException, 
           IllegalOperationException,
           BusAdapterException);

  /**
   * Implementation of the abstract function in HardwareDevice.
   * Here you will only find PCI specific (BUT IMPORTANT) issues of 
   * this function. Refer to the HardwareDeviceInterface for general
   * documentation.
   *
   * IMPORTANT NOTE: If you read out 
   * a Fifo, you have to be sure that possible retry cylces on 
   * the PCI bus do not corrupt the data of your Fifo (that means
   * it is not always clear that a PCI interface can be programmed
   * not to repeat a local access to a Fifo in this case. This means
   * that one data word of the Fifo (the one corresponding to the 
   * aborted cycle) is lost. This is a known problem in PCI 
   * technology but there are of course implementations which handle
   * these cases correctly. This remark has only been added here in 
   * order to put you in an alert state if you deal with Fifos on PCI
   * busses.
   *
   * @param addressBehaviour determines if the address in the hardware
   *        device is incremented during the data transfer or not. (The
   *        latter you need if you read out a Fifo.) BusAdapters for 
   *        hardware which contain DMA machines have to program the 
   *        DMA engines accordingly. 
   * @throws IllegalOperationException is thrown if the function is 
   *        called on an item other than a memory space item.
   */
  void hardwareWriteBlock( const GeneralHardwareAddress& pciAddress, 
                           uint32_t length,
                           char *buffer,
                           HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT,
                           uint32_t offset = 0) const
    throw (IllegalValueException, 
           UnsupportedException, 
           IllegalOperationException,
           BusAdapterException);

  /**
   * undocumented feature...do not use: might dissapear in future, 
   * makes your application BusAdapter - dependent.
   */
  uint32_t getItemAddress( std::string ) const
    throw ( NoSuchItemException,
            IllegalOperationException );

  /**
   * Retrieves the PCI Bus-Address for a BAR to the user
   * This function is useful if the user has to set up DMA 
   * engines which transfer data to or from this device.
   */
  uint32_t getPCIBusAddress( uint32_t iBar )
    throw (BusAdapterException);



private:
  PCIAddressTable& pciAddressTable;
  PCIBusAdapterInterface& pciBusAdapter;
  PCIDeviceIdentifier* deviceIdentifierPtr;
  std::vector<uint32_t> baseRegisters;
};

} /* namespace HAL */

#endif /* __PCIDevice */


