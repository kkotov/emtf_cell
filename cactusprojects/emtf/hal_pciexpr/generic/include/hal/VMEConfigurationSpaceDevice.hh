#ifndef __VMEConfigurationSpaceDevice
#define __VMEConfigurationSpaceDevice

#include "hal/VMEDevice.hh"
#include "hal/VMEAddressTable.hh"

namespace HAL {

/**
*
*
*     @short A   VME  Device  with   customized  hardware   read-  and 
*            write-methods   in    order   to   access    the   VME64x 
*            configuration space.
*            
*            This  class  overwrites   the  methods  hardwareRead  and 
*            hardwareWrite of  the VMEDevice in  order to re-interpret 
*            the width parameter  of the GeneralHardwareAddress. Since
*            in  the  predefined configuration  space  of VME64x  only 
*            every 4th  byte is used,  the methods read or  write only 
*            every fourth byte for items which are longer than 1 byte.
*            
*            This approach  may deem dirty but may  be tolerable since 
*            this class is  thought to be used only  internally by the 
*            HAL. 
*            
*
*       @see 
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class VMEConfigurationSpaceDevice : public VMEDevice {
public:
  /**
   * The vmeAddresstable must be given in the constructor
   * since this class inherits from VMEDevice which in the 
   * constructor already needs the table. This Table should
   * a and VMEAddressTable which has been generated with the
   * VMEConfigurationSpaceAddressReader. 
   */
  VMEConfigurationSpaceDevice( VMEAddressTable & vmeAddressTable,
			       VMEBusAdapterInterface & vmeBusAdapter );

  virtual ~VMEConfigurationSpaceDevice();

  /**
   * Overwrites the corresponding function of the VMEDevice.
   * For the VMEConfigurationSpaceDevice the parameters of the AddressTable
   * are slightly reinterpreted: Data in the configuration
   * space of VME64x, is contained at address locations 
   * 4*n+3, where n=0,1,2,...,0x3ff. This function reads out
   * the bytes at the correct addresses and composes the result
   * in usigned long words (4 bytes wide). When the data width
   * is larger than 1 byte (i.e. 2 to 4 bytes), all bytes are
   * read out via single byte accesses and they are concatenated
   * in a way that the most significant byte of the item is 
   * put at the higher bit numbers of the uint32_t. 
   */
  void hardwareRead( const GeneralHardwareAddress& vmeAddress, 
                     uint32_t* result,
                     uint32_t offset = 0 ) const
    throw (BusAdapterException);


  /**
   * Overwrites the corresponding function of the VMEDevice.
   * For the VMEConfigurationSpaceDevice the parameters of the 
   * are slightly reinterpreted: Data in the configuration
   * space of VME64x, is contained at address locations 
   * 4*n+3, where n=0,1,2,...,0x3ff. This function writes
   * as many bytes as indicated by the datawidth contained 
   * in the address. The addresses of the single bytes are
   * obtained from the address-parameter and the formula
   * mentioned above.
   */
  void hardwareWrite( const GeneralHardwareAddress& vmeAddress, 
                      uint32_t data,
                      uint32_t offset = 0 ) const
    throw (BusAdapterException);
  

  /**
   * Block transfers are not supported in configuration space.
   * This method merely throws the UnsupportedException.
   */
  void hardwareReadBlock( const GeneralHardwareAddress& vmeAddress,
                          uint32_t length,
                          char *buffer,
                          HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT,
                          uint32_t offset = 0 ) const
    throw (UnsupportedException);
  
  /**
   * Block transfers are not supported in configuration space.
   * This method merely throws the UnsupportedException.
   */
  void hardwareWriteBlock( const GeneralHardwareAddress& vmeAddress,
			   uint32_t length,
			   char *buffer,
			   HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT,
			   uint32_t offset = 0 ) const
    throw (UnsupportedException);
};

} /* namespace HAL */

#endif /* __VMEConfigurationSpaceDevice */
