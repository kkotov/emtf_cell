#ifndef __VMEBusAdapterInterface
#define __VMEBusAdapterInterface

#include <string>
#include <vector>

#include "hal/IllegalValueException.hh"
#include "hal/VMEAddressTable.hh"
#include "hal/DeviceIdentifier.hh"
#include "hal/BusAdapterException.hh"
#include "hal/UnsupportedException.hh"
#include "hal/HardwareDeviceInterface.hh"

namespace HAL {

/**
*
*
*     @short Interface to the VME-bus hardware.
*            
*            The VMEBusAdapterInterface is  the software interface for
*            classes which  actually talk to the  hardware, i.e. which 
*            transform the  actual VME  transfer. In this  library for 
*            each VME bus there  is one VMEBusAdapter. It performs all
*            operations  which  are happening  on  the  VME bus. 
*            
*            Every VMEDevice has  a handle to a VMEBusAdapterInterface
*            which it uses to perform the read and write accesses.
*
*       @see VMEDevice
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class VMEBusAdapterInterface {

public:

  /**
   * The virtual desctructor of the class.
   */
  virtual ~VMEBusAdapterInterface() {};

  /**
   * Any hardware device must be registered with the busAdapter with
   * a call to this routine. Only afterwards the device can be accessed.
   * @param vmeAddressTable is needed  in order to allow a busAdapter to 
   *        do memory mapping. (But a busAdapter is not forced to do so.)
   *        The AddressTable allows the BusAdapter to determine how much
   *        memory must be mapped.
   * @param vmeBaseaddress is the baseaddress of the module. The BusAdapter
   *        needs this value if it wants to map the VME space of the module
   *        into memory. Together with the VMEAddressTable it defines the
   *        "destination addresses" of the mapping in the VME crate.
   * @param deviceIdentifierPtr stores all device specific data which
   *        the busAdapter needs in order to access the module. It is 
   *        an argument to the write and read methods. Note that the
   *        call needs to be given a handle (a pointer to a pointer).
   *        The busAdapter creates the DeviceIdentifier for the 
   *        device to be registered. After the call "*deviceIdentifierPtr"
   *        contains a pointer to this DeviceIdentifier.
   *        IMPORTANT: The DeviceIdentifier is deleted in the closeDevice
   *        method. Therefore this method is called in the destructor of
   *        the VMEDevice. This also means that the VMEBusAdapter stays
   *        the owner of the DeviceIdentifiers!
   * @param baseAddressPtr is a pointer to a location where the openDevice
   *        writes an address-offset which should be added to every address 
   *        address before it is passed to the read and write routines. 
   *        For a busAdapter which does not do memory mapping the value
   *        of this offset is usually the baseaddress itself. The address value 
   *        passed to the read and write functions is then the address value
   *        seen on the VME backplane. For memory mapped BusAdapters this 
   *        offset depends on the mapping-algorithm of the driver.
   *                 
   */
  virtual void openDevice( const VMEAddressTable& vmeAddressTable,
                           uint32_t vmeBaseaddress,
                           DeviceIdentifier** deviceIdentifierPtr,
                           uint32_t* baseAddressPtr,
			   bool doSwapping )
    throw (BusAdapterException,
	   UnsupportedException) = 0;
  
  /**
   * The VME64x specific way to open a device.
   * The only difference with respect to the openDevice method for 
   * "simple" VME methods is, that the call needs to be given one
   * baseaddress for each of the six possible functions.
   * A default implementation has been provided so that BusAdapters
   * are not obliged to implement this method. (This means they only
   * work with standard VME).
   * @param vmeBaseaddresses is a vector containing exactly 8
   *        baseAddresses. The values for baseAddresses of functions
   *        which are not implemented are ignored.
   * @param baseAddressesPtr must point to a vector with 
   *        exactly 8 entries. The functionality of the vector
   *        corresponds to that of the baseAddressPtr in the 
   *        openDevice call for standard VME modules.
   */
  virtual void openDevice( const VMEAddressTable& vmeAddressTable,
                           std::vector<uint32_t> vmeBaseaddresses,
                           DeviceIdentifier** deviceIdentifierPtr,
                           std::vector<uint32_t>* baseAddressesPtr,
			   bool doSwapping )
    throw (BusAdapterException,
	   UnsupportedException) {
    std::string text = "openDevice for VME64x modules not implemented.\n    (VMEBusAdapterInterface::openDevice)";
    throw (UnsupportedException( text, __FILE__, __LINE__, __FUNCTION__  ) );
  }
  
  /**
   * Unregister a device from the BusAdapter.
   * If a device is not needed anymore it must be unregistered so that
   * the memory mapping (if it was done) can be deleted. This operation 
   * also deletes the deviceIdentifier.
   */
  virtual void closeDevice( DeviceIdentifier* deviceIdentifier )
    throw( BusAdapterException ) = 0;

  virtual void write( DeviceIdentifier* deviceIdentifierPtr,
                      uint32_t address,
                      uint32_t addressModifier,
                      uint32_t dataWidth,
                      uint32_t data)
    throw( IllegalValueException, BusAdapterException ) = 0;

  virtual void read( DeviceIdentifier* deviceIdentifierPtr,
                     uint32_t address,
                     uint32_t addressModifier,
                     uint32_t dataWidth,
                     uint32_t* result)
    throw( IllegalValueException, BusAdapterException ) = 0;
  
  /**
   * A VMEBusAdapter should implement this function with the fastest 
   * way to transfer a block of data. Since the addressModifier defines
   * how to transfer the data on the VME backplane, there is not much
   * freedom for the implementation of this function. 
   * But a choice to be made for a non-block transfer access 
   * could be to do a memory mapped access for data lengths 
   * which are large enough so that the faster transfer compared to 
   * non-mapped accesses pays off the overhead in creating the maps.
   * (This of course is irrelevant for BusAdapters which anyway use memory
   * mapped access to the hardware.)
   * @param addressBehaviour see the documentation of the VMEDevice 
   *        for remarks relevant for this parameter.
   * @param length has to be given in bytes.
   */
  virtual void readBlock( DeviceIdentifier* deviceIdentifierPtr,
                          uint32_t startAddress,
                          uint32_t length,      // in bytes
                          uint32_t addressModifier,
                          uint32_t dataWidth,
                          char *buffer,
                          HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT )
    throw( UnsupportedException,
           BusAdapterException ) = 0;
  /**
   * A VMEBusAdapter should implement this function with the fastest 
   * way to transfer a block of data. Since the addressModifier defines
   * how to transfer the data on the VME backplane, there is not much
   * freedom for the implementation of this function. 
   * But a choice to be made for a non-block transfer access 
   * could be to do a memory mapped access for data lengths 
   * which are large enough so that the faster transfer compared to 
   * non-mapped accesses pays off the overhead in creating the maps.
   * (This of course is irrelevant for BusAdapters which anyway use memory
   * mapped access to the hardware.)
   * @param addressBehaviour see the documentation of the VMEDevice 
   *        for remarks relevant for this parameter.
   * @param length has to be given in bytes.
   */
  virtual void writeBlock( DeviceIdentifier* deviceIdentifierPtr,
                          uint32_t startAddress,
                          uint32_t length,      // in bytes
                          uint32_t addressModifier,
                          uint32_t dataWidth,
                          char *buffer,
                          HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT )
    throw( UnsupportedException,
           BusAdapterException ) = 0;

  /**
   * Reset the VME Bus.
   * The VME bus has a global reset line. This command should activate this
   * line. 
   * @throws UnSupportedException is thrown if the BusAdapter cannot implement
   *         this function for some reason.
   */
  virtual void resetBus( )
    throw( BusAdapterException,
           UnsupportedException ) = 0; 
};

} /* namespace HAL */

#endif /* __VMEBusAdapterInterface */
