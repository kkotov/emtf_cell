#ifndef __PCIAddressTableXMLFileReader
#define __PCIAddressTableXMLFileReader

#include <string>

#include "hal/AddressTableReader.hh"
#include "hal/HardwareAccessException.hh"
#include "hal/XMLProcessingException.hh"
#include "hal/DOMToPCIMapConverter.hh"
#include "hal/XMLParserErrorHandler.hh"

namespace HAL {

/**
*
*
*     @short A class to read a file with an XML representation
*            of a PCIAddressTable.
*            
*            The  XML representation of  the AddressTable  must follow 
*            the schema defined in 
*            
*            http://xdaq.web.cern.ch/xdaq/xsd/HardwareAddressTable-{version}.xsd .
*            
*            Note that the description if  given for an item must be a
*            one line item. (Subsequent lines are ignored)
*
*       @see AddressTable, AddressTableInterface, PCIAddressTable
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class PCIAddressTableXMLFileReader : public AddressTableReader {
public:
  PCIAddressTableXMLFileReader( std::string fileName )
    throw( XMLProcessingException );
};

} /* namespace HAL */

#endif /* __PCIAddressTableXMLFileReader */
