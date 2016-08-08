#ifndef __VMEAddressTableDynamicReader
#define __VMEAddressTableDynamicReader

#include <string>

#include "hal/AddressTableReader.hh"

namespace HAL {

/**
*
*
*     @short A reader which can be used to create VMEAddressTables
*            dynamically.
*            
*
*       @see 
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class VMEAddressTableDynamicReader : public AddressTableReader {

public:
 
  /**
   * The only method of the class creates an item for the reader
   * and stores them in a list (the list is defined in the 
   * baseclass). 
   * This method MUST be called ONLY BEFORE the reader is used
   * to build an AddressTable, of course.  
   */
  void createItem( std::string key, 
		   uint32_t width,
		   uint32_t address,
		   uint32_t addressModifier,
		   uint32_t mask,
		   uint32_t isReadable, 
		   uint32_t isWritable, 
		   std::string description);
};

} /* namespace HAL */

#endif /* __VMEAddressTableDynamicReader */
