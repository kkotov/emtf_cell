#ifndef __VMEAddressTable
#define __VMEAddressTable

#include <string>
#include <vector>
#include <iostream>

#include "hal/AddressTable.hh"
#include "hal/AddressTableItem.hh"
#include "hal/VMEHardwareAddress.hh"
#include "hal/AddressTableReader.hh"
#include "hal/AddressOutOfLimitsException.hh"

namespace HAL {

/**
*
*
*     @short The Address Table for VME Devices.
*            
*            This  class  is  the  first  non abstract  class  in  the 
*            hierachy  of  AddressTables.   It  implements  the  print 
*            function. In  the constructor it fills  the internal data 
*            structures.
*            
*            Addresses for the VMEAddressTable contain 
*            @li VME-Address-Modifier
*            @li data  width in bytes  (2 or 4 is  supported currently 
*            in HAL.
*            @li address offset with respect to the module-baseaddress.
*            
*            The  specific input to  the VMEAddressTable  is discussed 
*            in     the     relevant     implementations    of     the 
*            AddressTableReader. 
*
*       @see AddressTableReader
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class VMEAddressTable : public AddressTable {

public :
  /**
   * The constructure fills the internal data strucutures of the 
   * VMEAddressTable.
   * @param name is an arbitrary name for the table.
   * @param tableReader is used to fill the data structures.
   */
  VMEAddressTable( std::string name, AddressTableReader& tableReader );

  virtual ~VMEAddressTable();
  
  /**
   * Retrieves the highest address offset offset with respect to the
   * baseaddress.
   * This function is useful for VMEBusAdapters which map the Addressspace
   * of the hardware device into memory. This service allows the Adapter to
   * determine the needed memory space needed to be mapped. 
   * @return the maximal address offset wrt the basaeaddress.
   */
  void getAddressBoundaries( std::vector<uint32_t>& minAddresses,
			     std::vector<uint32_t>& maxAddresses ) const;

  /**
   * Implementation of the AddressTableInterface.
   */  
  void checkAddressLimits( std::string item, uint32_t offset ) const
    throw( NoSuchItemException,
	   AddressOutOfLimitsException );

  /**
   * Implementation of the AddressTableInterface.
   */
  void checkAddressLimits( const GeneralHardwareAddress& hardwareAddress,
			   uint32_t offset ) const
    throw( AddressOutOfLimitsException );
  
  /**
   * Prints the AddressTable.
   */
  void print( std::ostream& os = std::cout ) const;

  /**
   * For each baseaddress determines the address boundaries
   * This function is public since in the case of VME64x
   * address tables this function must be called after the
   * data width has been determined. (Which in turn is done 
   * only after the crate has been mapped by the host computer).
   */
  void determineAddressBoundaries();
private:
  std::vector< uint32_t > maxAddresses_;
  std::vector< uint32_t > minAddresses_;
};

} /* namespace HAL */

#endif /* __VMEAddressTable */
