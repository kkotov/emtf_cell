#ifndef __AddressTableReader
#define __AddressTableReader

#include <list>
#include "hal/AddressTableItem.hh"

namespace HAL {

/**
*
*
*     @short An interface defining how to read AddressTableItems from 
*            an implementation of this class. 
*            
*            This interface is used by concrete implementations of the
*            AddressTable which in the constructor need to read from
*            an AddressTableReader all items of the  Table. The reader
*            has  been separarted from  the AddressTable  itself since
*            this allows to construct different AddressTableReaders for
*            different input methods which then can be plugged at will
*            into the implementation of the AddressTable itself.
*            
*            Example:  You  can  plug   into  the  constructor  of  a 
*            PCIAddressTable  an  AddressTableReader  which  reads  an 
*            ASCII   file   or   another one  which   reads   an   XML 
*            representation  of  the  table,  or  a  third  one  which 
*            accesses a database.
*
*            This class  is not ment  to be instantiated  directly. It 
*            merely defines the functionality  which is common to all
*            AddressTableReaders: That is the  way how to retrieve the
*            data from an AddressTableReader.
*           
*       @see AddressTable, PCIAddressTable, VMEAddressTable
*       @see VMEAddressTableASCIIReader, PCIAddressTableASCIIReader 
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/

class AddressTableReader {
public:
  /**
   * Clean up of non-used resources.
   * With the next() method the AddressTableReader gives the pointer to 
   * the AddressTableItems to the user. The items are then owned by the 
   * user and it is the responsibility of the user to delete the items 
   * if they are not anymore needed. (This is done in the destructor of 
   * the AddressTable.) If on the other hand no item or only a part of
   * the items have been read out with the next() routine, the remaining
   * items must be deleted in this destructor.
   */
  virtual ~AddressTableReader();
  /**
   * Retrieves the next item from the AddressTableReader. 
   * After excecution *nextItem contains a pointer to the next item 
   * of the AddressTable. 
   */
  virtual bool next( AddressTableItem** nextItem );
protected:
  /**
   * The constructor of this class is protected in order to avoid
   * direct instantiation.
   */
  AddressTableReader();
  std::list<AddressTableItem*> itemPointerList;
private:
  bool firstAccess;
  std::list<AddressTableItem*>::iterator it;  
};

} /* namespace HAL */

#endif /* __AddressTableReader */




