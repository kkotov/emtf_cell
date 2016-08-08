#ifndef __AddressTableItem
#define __AddressTableItem

#include <string>
#include <iostream>
#include "hal/GeneralHardwareAddress.hh"
#include "hal/IllegalOperationException.hh"
#include "hal/MaskBoundaryException.hh"

namespace HAL {

/**
*
*
*     @short Contains data of an AddressTable item.
*            
*            This class  is a container  for all data belonging  to an 
*            item.  It  is  technology  independent  since  everything 
*            which   is  technology   dependent  is   hidden   in  the 
*            GeneralHardwareAddress  (the  user  will  pass  either  a 
*            PCIHardwareAddress   or  a   VMEHardwareAddress   to  the 
*            constructor. See the documentation of those.) 
*            
*       @see AddressTableInterface, GeneralHardwareAddress
*       @see PCIHardwareAddress, VMEHardwareAddress
*   @author: Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/

class AddressTableItem {
public:
  /**
   * The constructor of the class is trivial and its arguments are 
   * mostly selfexplaining.
   * @param key is the name of the item. This string is used in the rest
   *        of the software to reference the item. This is more userfriendly 
   *        than trying to remeber the addresses.
   * @param description is a short one line description of the item
   *        which in a gui could appear as a tooltip.
   * @param address contains the address and the access modifier for the
   *        item. Here hardware and platform dependent stuff is stored.
   * @param mask determines where the bits associated to the item can 
   *        be found in the 32-bit data word. The relevant bits are 
   *        marked with a '1'.
   * @param writableFlag is set if the item can be written.
   * @param readableFlag is set if the item can be read.
   */
  AddressTableItem( std::string key,
                    std::string description,
                    GeneralHardwareAddress& address,
                    uint32_t mask,
                    bool writableFlag,
                    bool readableFlag);
  

  /**
   * Copy constructor to make deep copies of AddressTables.
   *
   * The AddressTableItem contains a reference to a GeneralHardwareAddress.
   * This is needed since the GeneralHardwareAddress is used polymorphic.
   * On the other hand this means the default copy constructor is not 
   * adequate for this class. (Remember: this class owns the GeneralHardwareAddress
   * and therfore deletes it in the destructor. Therfore it must be cloned
   * in the copy constructor.)
   */
  AddressTableItem( const AddressTableItem& );

  /**
   * Here the GeneralHardwareAddress is deleted. (It is on the heap.)
   */
  virtual ~AddressTableItem();

  uint32_t getMask() const;

  std::string getDescription() const;

  std::string getKey() const;

  /**
   * The returned reference is not constant since in the 
   * case of VME64x the configuration algorithm must configure
   * the HardwareAddress with the AM and the dataWidth chosen to 
   * access the items of the mapped function window. In the case
   * of standard VME and PCI there is no danger since the API of 
   * the HardwareAddresses for these two cases guarantees that 
   * no method can change the internal state. (All methdos are const.)
   */
  GeneralHardwareAddress& getGeneralHardwareAddress() const;

  bool isWritable() const;

  bool isReadable()const;

  /**
   * This function throws an exceptioni if this item is not writable.
   */
  void checkWritable() const 
    throw(IllegalOperationException);

  /**
   * This function throws an exceptioni if this item is not readable.
   */
  void checkReadable() const 
    throw(IllegalOperationException);

  /**
   * Check if an item is a single bit.
   * This is the only service of the class. 
   * @return true if the item is a single bit (i.e. in its mask only
   *         one bit is set.
   */
  bool isBit() const ;

  void checkBit() const 
    throw(IllegalOperationException);

  /**
   * Performs what you think it does...
   */
  virtual void print( std::ostream& os = std::cout ) const;

  /**
   * Retrieve  a value  from  a bitfield  and  convert it  to a  
   * number.   Every  item  is  associated  to some  bitfield  in  a
   * dataword.  This function  is shifting a data value  for a given
   * item, which is  assumed to be at the  position indicated by the
   * item-mask, in a way that  the least significant bit becomes bit
   * 0   (   and   the   data   value  of   the   bitfield   becomes
   * "human-readable").
   * @param data is the value for the item which should be shifted.
   * @param MaskBoundaryCheck is an optional parameter which allows 
   *        to switch off a check that only bits within the mask
   *        are set. If checking is switched on and the routine 
   *        finds data bits set outside of the mask it will throw 
   *        a MaskBoundaryException. The check is switched of 
   *        by defaults because it will be ofter the case that bits
   *        might be set outside the mask since they belong to 
   *        other items.
   */
  uint32_t applyFromMask( uint32_t data,
                               bool MaskBoundaryCheck = false ) const
    throw( MaskBoundaryException );

  /**
   * Shifts  data to the bitpositions  defined by  the  item's mask.
   * Every  item  is  usually  associated  to  some  bitfield  in  a
   * dataword.  This function  is shifting a value for  a given item
   * into the bitfield indicated by the mask of the item.
   * @param data is the value for the item which should be shifted.
   * @param MaskBoundaryCheck is an optional parameter which allows 
   *        to switch off a check that the data value fits into the
   *        bitfields of the mask. If checking is switched on and 
   *        the user provides a value which would set bits outside
   *        of the items mask, a MaskBoundaryException is thrown.
   *        By default checking is switched on since this usually
   *        means that the user of the method provided a wrong data
   *        value for the item. 
   */
  uint32_t applyToMask( uint32_t data,
                             bool MaskBoundaryCheck = true ) const
    throw( MaskBoundaryException) ;
protected:
  /**
   * Counts the number of bits set to '1'.
   */
  uint32_t countBits( uint32_t word ) const;
  
private:
  /**
   * forbid assignment
   */
  void operator=(const AddressTableItem& ai) {};
  GeneralHardwareAddress& hardwareAddress;
  std::string key;
  std::string description;
  uint32_t mask;
  bool writableFlag;
  bool readableFlag;
  bool bitFlag;
};
} /* namespace HAL */

#endif /*_AddressTableItem */

