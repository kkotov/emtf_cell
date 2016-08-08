#ifndef __PCIAddressTableDOMReader
#define __PCIAddressTableDOMReader

#include <dom/DOMDocument.hpp>
#include "hal/AddressTableReader.hh"
#include "hal/XMLProcessingException.hh"
#include "hal/DOMToPCIMapConverter.hh"

namespace HAL {

/**
*
*
*     @short A class to read DOM representation of a PCIAddressTable.
*
*            
*            http://xdaq.web.cern.ch/xdaq/xsd/HardwareAddressTable-{version}.xsd .
*            
*       @see AddressTable, AddressTableInterface, PCIAddressTable
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class PCIAddressTableDOMReader : public AddressTableReader {
public:
  PCIAddressTableDOMReader( DOMDocument* doc )
    throw( XMLProcessingException );
};

} /* namespace HAL */

#endif /* __PCIAddressTableDOMReader */
