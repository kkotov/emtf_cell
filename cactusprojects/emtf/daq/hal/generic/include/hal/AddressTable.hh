#ifndef __AddressTable
#define __AddressTable

#include <tr1/unordered_map>
#include <string>

#include "hal/AddressTableInterface.hh"
#include "hal/AddressTableItem.hh"
#include "hal/NoSuchItemException.hh"
#include "hal/IllegalOperationException.hh"
#include "hal/HalHash.hh"
#include "hal/MaskBoundaryException.hh"
#include "VMEConstants.h"

namespace HAL {

/**
*
*
*     @short Implementation of a AddressTableInterface.
*            
*            This    class    is     an    implementation    of    the
*            AddressTableInterface.   It  is NOT  thought  to be  used
*            directly,  but it  is  merely the  implementation of  the
*            functionality  defined by the  interface.  It  is missing
*            any functionality  to insert  items into the  itemlist of
*            the  map. This  is supposed  to  be done  by the  classes
*            inheritating  from the  AddressTable.   With this  design
*            general   functionality   is   cleanly   separated   from
*            application dependend features.
*            
*            Since this  class merely  implemets the methods  from the
*            AddressTableInterface there is  hardly  documentation for
*            the  functions  here. PLease  look  up the  documentation 
*            there. 
*            
*            Note that  the constructor of  the class is  protected in 
*            order to prevent direct instantiation of this class
*
*       @see AddressTableInterface, AddressTableItem
*            PCIAddressTable, VMEAddressTable
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/

class AddressTable : public AddressTableInterface {
public:
  /**
   * The copy constructor of the AddressTable.
   *
   * The copy constructor of the AddressTable has been implemented
   * for safety: The AddressTableContainer shares addresstables among
   * many objects by handling the references to the AddressTables in the
   * container. If by mistake a programmer assigns the reference to a
   * variable of type AddressTable (and not of type "reference to Adddresstable")
   * the copy constructor of the AddressTable is called. If this one does
   * not produce a deep copy of the AddressTableItems in the table
   * it happens that as soon as the first AddressTable is destroyed 
   * (goes out of scope) all AddressTableItems are destroyed although
   * they still should exist for use in the objects which share the 
   * same AddressTable.
   */
  AddressTable( const AddressTable& orgiginal );
  /**
   */
  virtual ~AddressTable();
  /**
   */
  std::string getName() const { return name; } ;
  /**
   */
  const GeneralHardwareAddress& getGeneralHardwareAddress( std::string item ) const
    throw( NoSuchItemException );
  /**
   */
  const GeneralHardwareAddress& getWritableHardwareAddress( std::string item ) const
    throw (NoSuchItemException, IllegalOperationException);
  /**
   */
  const GeneralHardwareAddress& getReadableHardwareAddress( std::string item ) const
    throw (NoSuchItemException, IllegalOperationException);
  /**
   */
  const AddressTableItem& checkItem(std::string item) const 
    throw (NoSuchItemException);
  /**
   */
  bool isReadable( std::string item ) const
    throw( NoSuchItemException );
  /**
   */
  bool isWritable( std::string item ) const
    throw( NoSuchItemException );
  /**
   */
  bool exists( std::string item ) const;
  /**
   */
  uint64_t getMask( std::string item ) const
    throw( NoSuchItemException );
  /**
   */
  std::string getDescription( std::string item ) const
    throw( NoSuchItemException );
  /**
   */
  bool isBit( std::string item ) const
    throw( NoSuchItemException );
  /**
   */
  std::tr1::unordered_map<std::string, AddressTableItem*, HalHash<std::string> >::const_iterator
  getItemListBegin() const; 

  /**
   */
  std::tr1::unordered_map<std::string, AddressTableItem*, HalHash<std::string> >::const_iterator
  getItemListEnd() const; 
  
  /**
   */
  uint64_t applyToMask( std::string item, 
                             uint64_t data, 
                             bool MaskBoundaryCheck = true ) const
    throw( NoSuchItemException, MaskBoundaryException );
  /**
   */
  uint64_t applyFromMask( std::string item, 
                               uint64_t data, 
                               bool MaskBoundaryCheck = false ) const
    throw( NoSuchItemException, MaskBoundaryException );

protected:
  /**
   * The protected constructor.
   * The   constructor  is  protected   since  nobody   should  ever
   * instantiate  an  AddressTable  directly.  Only  the  technology
   * specific Tables PCIAddressTable and VMEAddressTable are useful.
   */
  AddressTable( std::string name );

protected:
  std::tr1::unordered_map<std::string, AddressTableItem*, HalHash<std::string> > itemMap;
  std::string name;
};

} /* namespace HAL */

#endif /*__AddressTable */

