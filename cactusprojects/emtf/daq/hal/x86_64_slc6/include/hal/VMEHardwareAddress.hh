#ifndef __VMEHardwareAddress
#define __VMEHardwareAddress

#include <iostream>
#include <string>

#include "hal/GeneralHardwareAddress.hh"
#include "hal/IllegalValueException.hh"

namespace HAL {

/**
*
*
*     @short Specifies address and access mode for VME items.
*            
*            A VME access  to an item of a  VME-module is specified by
*            the the offset to the  baseaddress, the width of the data
*            to  be  transfered  and   the  access  context  which  is
*            specified   in  the   addressModifier.   Note   that  the
*            baseaddress itself  is not contained in  this class since
*            it is a property of  the VME-module and NOT a property of
*            the item  to be  accessed.  (Similarly in  the PCIAddress
*            the  BAR-Id is  contained but  NOT the  value of  the Bar
*            itself.)
*
*       @see GeneralHardwareAddress, PCIHardwareAddress
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class VMEHardwareAddress : public GeneralHardwareAddress {
public:
  /**
   * @param dataWidth is the data width in bytes.
   */
  VMEHardwareAddress( uint32_t address, 
                      uint32_t addressModifier,
                      uint32_t dataWidth )
    throw( IllegalValueException );

  virtual uint32_t getAddressModifier() const
    throw();

  virtual bool isIOSpace() const
    throw (IllegalOperationException);

  virtual void print( std::ostream& os = std::cout ) const;

  virtual GeneralHardwareAddress* clone() const;

private:
  /**
   * A helper for the print function;
   */
  std::string getAddressSpaceString() const;

  uint32_t addressModifier_;

};

} /* namespace HAL */

#endif /* __VMEHardwareAddress */
