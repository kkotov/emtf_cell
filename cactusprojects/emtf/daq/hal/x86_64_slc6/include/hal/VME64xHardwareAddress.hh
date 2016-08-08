#ifndef __VME64xHardwareAddress
#define __VME64xHardwareAddress

#include <string>
#include <iostream>

#include "hal/GeneralHardwareAddress.hh"
#include "hal/IllegalValueException.hh"
#include "hal/AddressSpace.hh"

namespace HAL {

/**
*
*
*     @short Specifies address and access mode for VME64x items.
*            
*            A  VME64x  access  to  an  item  of  a  VME64x-module  is
*            specified by  the the offset to the  baseaddress, and the
*            mapped WindowId.  Note that the baseaddress itself is not
*            contained in  this class  since it is  a property  of the
*            VME64x-function  and NOT  a property  of the  item  to be
*            accessed.   (Similarly   in  the  PCIHardwareAddress  the
*            BAR-Id is contained but NOT the value of the Bar itself.
*
*       @see GeneralHardwareAddress, PCIHardwareAddress
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class VME64xHardwareAddress : public GeneralHardwareAddress {
public:
  /**
   * @param mappedWindowId has a default value of 0 since in case
   *        the addressSpace is the configuration space, the 
   *        value of this parameter is irrelevant.
   * @param width is only used for configuration space items. It 
   *        has no meaning for memory space items.
   */
  VME64xHardwareAddress( uint32_t address, 
                         enum AddressSpace addressSpace,
                         uint32_t mappedWindowId,
			 uint32_t width )
    throw( IllegalValueException );

  virtual uint32_t getAddressModifier() const
    throw();
  virtual bool isIOSpace() const
    throw (IllegalOperationException);
  virtual void print( std::ostream& os = std::cout ) const;

  /**
   * Set the address modifier and the data access width.
   * This function is specific to VME64x, since these paramenters
   * are set during configuration of the VMECrate. For standard
   * VME modules the parameters are contained in the addressTable.
   */
  void setWindowConfiguration( uint32_t AM,
                               uint32_t dataWdith )
    throw( IllegalValueException );

  virtual GeneralHardwareAddress* clone() const;

private:
  /**
   * A helper for the print function;
   */
  std::string getAddressSpaceString() const;

  uint32_t addressModifier_;

};

} /* namespace HAL */

#endif /* __VME64xHardwareAddress */
