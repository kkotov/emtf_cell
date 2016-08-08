#ifndef __GeneralHardwareAddress
#define __GeneralHardwareAddress

#include <stdint.h>
#include <vector>

#include "hal/AddressSpace.hh"
#include "hal/IllegalOperationException.hh"


namespace HAL {

/**
*
*
*     @short Specifies the address and access mode for an item. 
*            
*            In  order to have  technology independent  interfaces for
*            the  HardwareDevice and the  AddressTable the  concept of
*            the  General  Hardware Address  is  introduced with  this
*            class. Common  to all general addresses is  the value for
*            the  address itself  (a hex  number). On  the  other hand
*            everything  which  is  related  to  the  access  mode  is
*            technology  dependent and  therefore  implemented in  the
*            derived classes.
*
*       @see PCIHardwareAddress, PCIHardwareAddress
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/

class GeneralHardwareAddress {
public:
  /**
   * The Constructor of the general hardware address. 
   * @param address in each technology (PCI or VME) is the offset to 
   *        some baseaddress.
   */
  GeneralHardwareAddress ( uint64_t address,
                           uint64_t dataWidth );

  virtual ~GeneralHardwareAddress();  

  /**
   * Return the address offset for the HardwareAddress.
   */  
  uint64_t getAddress() const;

  /**
   * This function is given for enhanced efficency/
   * It is possible to retrieve the MapId and the address
   * separately and then add the baseaddress to the obtained address
   * outside of this class. But this involves two function calls
   * which increases substentially the overhead of the HAL to a 
   * read/write operation. 
   */
  uint64_t getComputedAddress( const std::vector<uint64_t>& baseaddresses ) const ;
  // for backward compatibility:
  uint32_t getComputedAddress( const std::vector<uint32_t>& baseaddresses ) const ;
  /**
   * Return the address modifier for VME modules.
   * This function throws an Exception for PCI modules
   * since in PCI there are no address modifiers.
   */  
  virtual uint32_t getAddressModifier() const
    throw( IllegalOperationException ) = 0;

  /**
   * Return the data width in bytes.
   */      
  uint64_t getDataWidth() const;

  /**
   * Return the AddressSpace.
   */
  enum AddressSpace getAddressSpace() const;
  /**
   * Return the identifier of the addressMap.
   */

  uint64_t getMapId() const; 
  /**
   * Test on the address space.
   */
  bool isConfigSpace() const;

  /**
   * Test on the address space.
   */
  bool isMemorySpace()const;

  /**
   * Test on the address space.
   * For VME Hardware Addresses this function throws an
   * exception since there is no IO-space in VME.
   */
  virtual bool isIOSpace() const 
    throw( IllegalOperationException ) = 0;

  /**
   * The implementation depends on the technology.
   */
  virtual void print( std::ostream& os = std::cout ) const = 0;

  /**
   * This method is needed to make deep copies of AddressTables.
   */
  virtual GeneralHardwareAddress* clone() const = 0;

protected:

  uint64_t address_;
  uint64_t dataWidth_;
  uint64_t addressModifier_;

  /**
   * for VME this is the functionId (0...7) and for PCI this is the BAR (0...5).
   */
  uint64_t mapId_;

  /**
   * This is configuration space or memory space. In case
   * of PCI it also can be IO-Space.
   */
  enum AddressSpace addressSpace_;
};

} /* namespace HAL */

#endif /* __GeneralHardwareAddress */
