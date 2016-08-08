#ifndef __PCIHardwareAddress
#define __PCIHardwareAddress

#include <string>
#include <iostream>

#include "hal/GeneralHardwareAddress.hh"
#include "hal/IllegalValueException.hh"

namespace HAL {

/**
*
*
*     @short Specifies address and access mode for PCI items.
*            
*            A PCI access is  specified by the baseaddress identidfier
*            (  a number  between  0 and  5  which selects one out  of
*            BAR0...BAR5),  the  offset to  this  baseaddress and  the
*            access  context  (memory  access or  configuration  space
*            access. IO accesses are not yet supported by this library
*            but on demand they can be easily implemented.)
*
*       @see GeneralHardwareAddress
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class PCIHardwareAddress : public GeneralHardwareAddress {
public:
  PCIHardwareAddress( uint64_t address, 
                      enum AddressSpace addressSpace,
                      uint64_t barId = 0 )
    throw (IllegalValueException);

  virtual bool isIOSpace() const throw();
  virtual uint32_t getAddressModifier() const // seems to be a placeholder anyway
    throw( IllegalOperationException );
  virtual void print( std::ostream& os = std::cout ) const;

  virtual GeneralHardwareAddress* clone() const;

private:
  /**
   * A helper for the print function;
   */
  std::string getAddressSpaceString() const;
};

} /* namespace HAL */

#endif /* __PCIHardwareAddress */
