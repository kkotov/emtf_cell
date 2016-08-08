#ifndef __PCIAddressTable
#define __PCIAddressTable

#include <vector>
#include <string>
#include <iostream>

#include "hal/IllegalOperationException.hh"
#include "hal/AddressTable.hh"
#include "hal/AddressTableReader.hh"
#include "hal/PCIHardwareAddress.hh"
#include "hal/AddressSpace.hh"
#include "hal/AddressOutOfLimitsException.hh"


namespace HAL {

/**
*
*
*     @short The Address Table for PCI Devices.
*            
*            This  class  is  the  first  non abstract  class  in  the 
*            hierachy  of  AddressTables.   It  implements  the  print 
*            function. In  the constructor it fills  the internal data 
*            structures.
*            
*            Addresses for the PCIAddressTable contain 
*            @li   accessmode  specifier:  (either   configuration  or 
*            memory; io is not yet implemented)
*            @li  bar identifier to  specifiy the  PCI-Baseaddress for 
*            the item.
*            @li address offset with respect to the Baseaddress.
*            
*            The  specific input to  the PCIAddressTable  is discussed 
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

class PCIAddressTable : public AddressTable {

public :

  /**
   * The constructure fills the internal data strucutures of the 
   * PCIAddressTable. 
   * @param name is an arbitrary name for the table.
   * @param tableReader is used to fill the data structures.
   */
  PCIAddressTable( std::string name, AddressTableReader& tableReader );

  virtual ~PCIAddressTable();

  /**
   * Retrieves the highest and lowest configuration space address offset and
   * the highest and lowest address  offset with respect to each BAR.
   * This function is useful for PCIBusAdapters which map the Addressspace
   * of the hardware device into memory. This service allows the Adapter to
   * determine the needed memory space needed to be mapped. An exmaple
   * of a busadapter which uses this functionality is the PCIDummyBusAdapter.
   * The results of the function are passed by the references given as
   * function arguments.
   */
  void getAddressBoundaries( uint32_t& minConfigAddress,
			     uint32_t& maxConfigAddress,
			     std::vector<uint32_t>& minAddresses,
			     std::vector<uint32_t>& maxAddresses ) const;
  
  void getAddressBoundaries( uint64_t& minConfigAddress,
			     uint64_t& maxConfigAddress,
			     std::vector<uint64_t>& minAddresses,
			     std::vector<uint64_t>& maxAddresses ) const;

  /**
   * Implementation of the AddressTableInterface.
   */  
  void checkAddressLimits( std::string item, uint64_t offset ) const
    throw( NoSuchItemException,
	   AddressOutOfLimitsException );

  /**
   * Implementation of the AddressTableInterface.
   */
  void checkAddressLimits( const GeneralHardwareAddress& hardwareAddress,
			   uint64_t offset ) const
    throw( AddressOutOfLimitsException );  
  
  /**
   * Prints the AddressTable.
   */
  void print( std::ostream& os = std::cout ) const;

private :
  void determineAddressBoundaries();
  std::vector< uint64_t > maxAddresses_;
  std::vector< uint64_t > minAddresses_;
  uint64_t minConfigAddress_;
  uint64_t maxConfigAddress_;
};

} /* namespace HAL */

#endif /* __PCIAddressTable */
