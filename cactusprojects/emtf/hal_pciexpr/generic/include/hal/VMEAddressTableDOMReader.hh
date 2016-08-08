#ifndef __VMEAddressTableDOMReader
#define __VMEAddressTableDOMReader

#include <xercesc/dom/DOMDocument.hpp>
#include "hal/AddressTableReader.hh"
#include "hal/XMLProcessingException.hh"
#include "hal/DOMToVMEMapConverter.hh"

namespace HAL {

/**
*
*
*     @short A class to read DOM representation of a VMEAddressTable.
*
*            
*            http://xdaq.web.cern.ch/xdaq/xsd/HardwareAddressTable-{version}.xsd .
*            
*       @see AddressTable, AddressTableInterface, VMEAddressTable
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class VMEAddressTableDOMReader : public AddressTableReader {
public:
  VMEAddressTableDOMReader( XERCES_CPP_NAMESPACE::DOMDocument* doc )
    throw( XMLProcessingException );
};

} /* namespace HAL */

#endif /* __VMEAddressTableDOMReader */
