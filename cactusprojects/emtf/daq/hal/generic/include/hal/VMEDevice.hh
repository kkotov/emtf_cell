#ifndef __VMEDevice
#define __VMEDevice

#include <vector>

#include "hal/HardwareDevice.hh"
#include "hal/VMEAddressTable.hh"
#include "hal/VMEBusAdapterInterface.hh"
#include "hal/DeviceIdentifier.hh"
#include "hal/IllegalValueException.hh"

namespace HAL {

/**
*
*
*     @short Implementation of a VME Hardware device.
*
*            This  class is  the first  technology dependent  class in 
*            the  class  hierachy. (But  it  does  not  depend on  the 
*            specific  hardware  to  use  the technology:  It  is  VME 
*            specific  but does  not depend  on the  implementation of 
*            the VME-interface.) 
*            The  VMEDevice  implements the  interface  to access  the 
*            hardware defined in the  HardwareDevice class for the VME
*            technology. 
*            
*            Note: This class does not have a low level function which
*            allows high performance access  to the device. The reason
*            for  this is  that  it  is better  to  directly call  the
*            apropriate function  in the vmeBusAdapter.  A function in
*            this class would not  do anything but forwarding the call
*            to  the vmeBusAdapter.   The  only thing  which could  be
*            checked is  the AddressModifier. But this has  to be done
*            in the vmeBusAdapter since  it is the only instance which
*            knows  what vme-accesses are  really supported.   (In the
*            case of the  PCIDevice it made sense to  implement a high
*            performance  function  since  it  checked the  barID  for
*            validity.
*
*       @see HardwareDeviceInterface, HardwareDevice
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class VMEDevice : public HardwareDevice {
public:
  /**
   * The Constructor of a VME device.
   * The  constructor  needs  all  parameters  which  unambiguously
   * define  the device  and the  methods how  to access  it  in the
   * specific hardware implementation. The user must make sure that the
   * addressTable exists as long as the VMEDevice (only a reference is
   * passed to the VMEDevice so that a single table can be shared among
   * several VMEDevices.) The same is valid for the busAdapter.
   *
   * @param AddressTable defines all accessable items of the 
   *        device.
   * @param vmeBusAdapter actually transfers data between the 
   *        device and the software. It depends on the specific
   *        hardware implementation of the VME interface and on
   *        the operating system. 
   * @param baseaddress specifies the device's baseaddress which 
   *        is usually defined by a set of switches. In the old
   *        VME norm it unambigously defined the Module in the 
   *        VME-crate.
   * @param doSwapping can be set to true in order to swap the bytes
   *        in data accesses d16 and d32. 
   */
  VMEDevice( VMEAddressTable & vmeAddressTable,
             VMEBusAdapterInterface & vmeBusAdapter,
             uint32_t baseaddress,
	     bool doSwapping = false)
	throw(BusAdapterException,
	      UnsupportedException);


  /**
   * The Constructor of a VME64x device.
   * The  constructor  needs  all  parameters  which  unambiguously
   * define  the device  and the  methods how  to access  it  in the
   * specific hardware implementation. The user must make sure that the
   * addressTable exists as long as the VMEDevice (only a reference is
   * passed to the VMEDevice so that a single table can be shared among
   * several VMEDevices.) The same is valid for the busAdapter. This 
   * constructor passes a vector of baseaddresses, one for each of the 
   * 8 possible VME64x functions.
   *
   * @param vmeAddressTable defines all accessable items of the 
   *        device.
   * @param vmeBusAdapter actually transfers data between the 
   *        device and the software. It depends on the specific
   *        hardware implementation of the VME interface and on
   *        the operating system. 
   * @param baseaddresses specifies the device's baseaddresses. In
   *        VME64x there are 8 possible functions each of which have
   *        a separate baseaddress. The baseaddresses for functions 
   *        which are not implemented in the module are ignored.
   * @param doSwapping can be set to true in order to swap the bytes
   *        in data accesses d16 and d32. 
   */
  VMEDevice( VMEAddressTable & vmeAddressTable,
             VMEBusAdapterInterface & vmeBusAdapter,
             std::vector<uint32_t>& baseaddresses,
             bool doSwapping = false )
	throw(BusAdapterException,
	      IllegalValueException,
	      UnsupportedException);

  virtual ~VMEDevice();

  /**
   * Implementation  of  the  abstract function  in  HardwareDevice.
   */

  void hardwareWrite( const GeneralHardwareAddress& vmeAddress, 
                      uint32_t data,
                      uint32_t offset = 0 ) const
    throw( BusAdapterException);

  void hardwareWrite64( const GeneralHardwareAddress& vmeAddress, 
                      uint64_t data,
                      uint64_t offset = 0 ) const
    throw( BusAdapterException)
  {
        throw( HAL::BusAdapterException( "VME doesn't support 64-bit architecture", __FILE__, __LINE__, __FUNCTION__  ) );
  }

  /**
   * Implementation  of  the  abstract function  in  HardwareDevice.
   */
  void hardwareRead( const GeneralHardwareAddress& vmeAddress, 
                     uint32_t* result,
                     uint32_t offset = 0 ) const 
    throw( BusAdapterException);

  void hardwareRead64( const GeneralHardwareAddress& vmeAddress, 
                     uint64_t* result,
                     uint64_t offset = 0 ) const 
    throw( BusAdapterException)
  {
        throw( HAL::BusAdapterException( "VME doesn't support 64-bit architecture", __FILE__, __LINE__, __FUNCTION__  ) );
  }

  /**
   * Implementation of the abstract function in HardwareDevice.
   * Here you will only find VME specific (BUT IMPORTANT) issues of 
   * this function. Refer to the HardwareDeviceInterface for general
   * documentation.
   * @param addressBehaviour determines if the address in the hardware
   *        device is incremented during the data transfer or not. (The
   *        latter you need if you read out a Fifo.) If the AM of the 
   *        startItem is set to blocktransfer and the used BusAdapter 
   *        supports these transfers, than it is the hardware
   *        of the VME-slave which decides what to do with the addresses.
   *        It will not increment the address if the first address
   *        of the block points to a Fifo. But you will have to give 
   *        the correct value of the addressBehaviour flag in any
   *        case, since the VME specification does not allow
   *        block transfers to cross a 256-byte border. This means 
   *        at every 256 byte border a new blocktransfer cycle with 
   *        the startAddress will be initiated by the master. If the
   *        addressBehaviour is set to HAL_NO_INCREMENT he must make 
   *        sure that this is always the same address. If a BusAdapter 
   *        does not support this option it must throw an 
   *        UnsupportedException if the addressBehaviour is set to 
   *        HAL_NO_INCREMENT. Note that this does not necessarily mean 
   *        that you cannot read out Fifos with a blockTransfer: if the 
   *        VMESlave has been cleverly implemented it will map an 
   *        adequatly large addressSpace to the address of the Fifo. 
   *        Then a "traditional" blocktransfer can be issued to this 
   *        address.
   *
   *        If this all confuses you read in the VME specification 
   *        about the block transfer.
   */
  void hardwareReadBlock( const GeneralHardwareAddress& vmeAddress, 
                          uint32_t length,
                          char *buffer,
                          HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT,
                          uint32_t offset = 0) const
    throw (IllegalValueException,
           BusAdapterException,
           UnsupportedException);

  void hardwareReadBlock64( const GeneralHardwareAddress& vmeAddress, 
                          uint64_t length,
                          char *buffer,
                          HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT,
                          uint64_t offset = 0) const
    throw (IllegalValueException,
           BusAdapterException,
           UnsupportedException)
  {
        throw( HAL::BusAdapterException( "VME doesn't support 64-bit architecture", __FILE__, __LINE__, __FUNCTION__  ) );
  }
 
  /**
   * Implementation of the abstract function in HardwareDevice.
   * Here you will only find VME specific (BUT IMPORTANT) issues of 
   * this function. Refer to the HardwareDeviceInterface for general
   * documentation.
   * @param addressBehaviour determines if the address in the hardware
   *        device is incremented during the data transfer or not. (The
   *        latter you need if you read out a Fifo.) If the AM of the 
   *        startItem is set to blocktransfer and the used BusAdapter 
   *        supports these transfers, than it is the hardware
   *        of the VME-slave which decides what to do with the addresses.
   *        It will not increment the address if the first address
   *        of the block points to a Fifo. But you will have to give 
   *        the correct value of the addressBehaviour flag in any
   *        case, since the VME specification does not allow
   *        block transfers to cross a 256-byte border. This means 
   *        at every 256 byte border a new blocktransfer cycle with 
   *        the startAddress will be initiated by the master. If the
   *        addressBehaviour is set to HAL_NO_INCREMENT he must make 
   *        sure that this is always the same address. If a BusAdapter 
   *        does not support this option it must throw an 
   *        UnsupportedException if the addressBehaviour is set to 
   *        HAL_NO_INCREMENT. Note that this does not necessarily mean 
   *        that you cannot read out Fifos with a blockTransfer: if the 
   *        VMESlave has been cleverly implemented it will map an 
   *        adequatly large addressSpace to the address of the Fifo. 
   *        Then a "traditional" blocktransfer can be issued to this 
   *        address.
   *
   *        If this all confuses you read in the VME specification 
   *        about the block transfer.
   */
  void hardwareWriteBlock( const GeneralHardwareAddress& vmeAddress, 
                           uint32_t length,
                           char *buffer,
                           HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT,
                           uint32_t offset = 0) const
    throw (IllegalValueException,
           BusAdapterException,
           UnsupportedException);

  void hardwareWriteBlock64( const GeneralHardwareAddress& vmeAddress, 
                           uint64_t length,
                           char *buffer,
                           HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT,
                           uint64_t offset = 0) const
    throw (IllegalValueException,
           BusAdapterException,
           UnsupportedException)
  {
        throw( HAL::BusAdapterException( "VME doesn't support 64-bit architecture", __FILE__, __LINE__, __FUNCTION__  ) );
  }
 
protected:
  VMEBusAdapterInterface& vmeBusAdapter;
  DeviceIdentifier* deviceIdentifierPtr;
  std::vector<uint32_t> baseaddresses_;
  bool doSwapping_;
};

} /* namespace HAL */

#endif /* __VMEDevice */
