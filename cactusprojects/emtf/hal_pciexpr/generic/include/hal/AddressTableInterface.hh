#ifndef __AddressTableInterface
#define __AddressTableInterface

#include <string>
#include <tr1/unordered_map>
#include <iostream>
#include "hal/GeneralHardwareAddress.hh"
#include "hal/AddressTableItem.hh"
#include "hal/NoSuchItemException.hh"
#include "hal/IllegalOperationException.hh"
#include "hal/MaskBoundaryException.hh"
#include "hal/AddressOutOfLimitsException.hh"
#include "hal/HalHash.hh"

namespace HAL {

/**
*
*
*     @short The user interface for the AddressTable.
*            
*            The  AddressTable  contains  the  information  about  all 
*            accessible  items of  a hardware  device.  This interface 
*            summarizes  all functions on  the AddressTable  which are 
*            technology   independent.  The   items   common  to   all 
*            AddressTables are :
*            
*            - The key (=name) of the item.
*            - Some  kind of Address.  The specific properties  of the 
*            Address depend  on the technology  (PCI or VME),  but all 
*            include an address which means  an offset to some kind of
*            baseaddress, and information on how to access the item.
*            - A mask  for the item. This mask  indicates the bitfield 
*            which refers to the item. It should be a continuous field
*            where all bits  set are next to each  other. (In practice 
*            this restriction should not be problematic.)
*            - A flag indicating if the item is readable.
*            - A flag indicating if the item is writable.
*            - A string to describe the meaning of the item.
*            
*            The   AddressTableInterface    describes   the   services 
*            provided  once  the   AddressTable  is  loaded  into  the 
*            internal  data-structures of  the HAL.  In order  to read 
*            the table  itself from some medium (file,  database, ...)  
*            look at the documentation of the AddressTableReader tree.
*
*       @see GeneralHardwareAddress, PCIHardwareAddress
*       @see VMEHardwareAddress, AddressTableReader
*            
*    @author Christoph Schwick
* $Revision: 1.3 $
*     $Date: 2009/04/29 10:23:07 $
*
*
**/

class AddressTableInterface {
public:

  virtual ~AddressTableInterface() {};

  /**
   * Retrieve the name of the AddressTable.
   */
  virtual std::string getName() const = 0;
  /**
   * Retrieve the GeneralHardwareAddress of a particular item.
   * If the item is not existing an "NoSuchItemException" is thrown.
   */
  virtual const GeneralHardwareAddress& getGeneralHardwareAddress( std::string item ) const
    throw( NoSuchItemException ) = 0;
  /**
   * Retrieve a writable General Address of a particular item.
   * If the item is not existing an "NoSuchItemException" is thrown.
   * If the item is not writable an "IllegalOperationException" is thrown.
   */
  virtual const GeneralHardwareAddress& getWritableHardwareAddress( std::string item ) const
    throw (NoSuchItemException, IllegalOperationException) = 0;
   /**
   * Retrieve a writable General Address of a particular item.
   * If the item is not existing an "NoSuchItemException" is thrown.
   * If the item is not readable an "IllegalOperationException" is thrown.
   */
  virtual const GeneralHardwareAddress& getReadableHardwareAddress( std::string item ) const
    throw (NoSuchItemException, IllegalOperationException) = 0;
  /**
   * Check if an item exists and return a reference to it.
   *
   * @exception NoSuchItemException is thrown if the item does not exist.
   */
  virtual const AddressTableItem&  checkItem( std::string item ) const
    throw (NoSuchItemException) = 0;
  /**
   * Test if an item in the map is readable.
   */
  virtual bool isReadable( std::string item ) const 
    throw( NoSuchItemException ) = 0;
  /**
   * Test if an item in the map is writable.
   */
  virtual bool isWritable( std::string item ) const
    throw( NoSuchItemException ) = 0;
 /**
   * Test if an item exists in the map.
   */
  virtual bool exists( std::string item ) const = 0;
  /**
   * Check if an item with an offset is in the limits of 
   * the address table.
   */
  virtual void checkAddressLimits( std::string item, 
				   uint32_t offset ) const 
    throw( NoSuchItemException,
	   AddressOutOfLimitsException ) = 0;
  /**
   * Check in a GeneralHardwareAddress is in the limits of the 
   * items listed in the address table. This function is used 
   * internally since it is more efficient than the equally named
   * function with the std::string in the signature. (The lookup of the 
   * item is saved.)
   */
  virtual void checkAddressLimits( const GeneralHardwareAddress& address,
				   uint32_t offset ) const
    throw( AddressOutOfLimitsException ) = 0;
  /**
   * Retrieve the Bitmap of the item.
   */
  virtual uint32_t getMask( std::string item ) const 
    throw( NoSuchItemException ) = 0;
  /**
   * Retrieve the description of the item.
   */
  virtual std::string getDescription( std::string item ) const
    throw( NoSuchItemException ) = 0;

  /**
   * Tests if an item is a flag.
   * If in the mask of an item  only one bit is set it is considered
   * a flag. This is tested here.
   */
  virtual bool isBit( std::string item ) const
    throw( NoSuchItemException ) = 0;

  /**
   * Provide an iterator for the map of AddressTableItems.
   * This method is needed for tools which need to perform checks
   * on all items in the table. For example a class which is 
   * configuring a VME64x module wants to check if the AMs specified
   * in the AddressTable are consistent with the AMCAP settings 
   * in the configuration ROM. The Iterator delivered is pointing to 
   * the first element of the iterator sequence.
   */
  virtual std::tr1::unordered_map<std::string, AddressTableItem*, HalHash<std::string> >::const_iterator
  getItemListBegin() const = 0; 

  /**
   * Provide an iterator pointing to the end of the AddressTableItems map.
   * This iterator is needed for loops over all items of the AddressTable.
   * @see getItemListBegin()
   */
  virtual std::tr1::unordered_map<std::string, AddressTableItem*, HalHash<std::string> >::const_iterator
  getItemListEnd() const = 0; 
  
  /**
   * This function does what it should...
   */
  virtual void print( std::ostream& os = std::cout ) const = 0;

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
   *        of the items mask, a warning is printed on the terminal.
   *        By default checking is switched on since this usually
   *        means that the user of the method provided a wrong data
   *        value for the item. 
   */
  virtual uint32_t applyToMask( std::string item, 
                                     uint32_t data, 
                                     bool MaskBoundaryCheck = true ) const
  throw( NoSuchItemException,
         MaskBoundaryException ) = 0;
/**
   * Retrieve  a value  from  a bitfield  and  convert it  to a  hex
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
   *        finds data bits set outside of the mask it will issue 
   *        a warning on the terminal. The check is switched of 
   *        by defaults because it will be ofter the case that bits
   *        might be set outside the mask since they belong to 
   *        other items.
   */
  virtual uint32_t applyFromMask( std::string item, 
                                       uint32_t data, 
                                       bool MaskBoundaryCheck = false ) const
    throw( NoSuchItemException,
           MaskBoundaryException ) = 0;


};
} /* namespace HAL */

#endif /*__AddressTableInterface */

