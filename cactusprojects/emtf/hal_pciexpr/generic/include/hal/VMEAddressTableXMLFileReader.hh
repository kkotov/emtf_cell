#ifndef __VMEAddressTableXMLFileReader
#define __VMEAddressTableXMLFileReader

#include <string>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include "hal/AddressTableReader.hh"
#include "hal/HardwareAccessException.hh"
#include "hal/XMLProcessingException.hh"
#include "hal/DOMToVMEMapConverter.hh"
#include "hal/XMLParserErrorHandler.hh"


namespace HAL {

/**
*
*
*     @short A class to read a file with an XML representation
*            of a VMEAddressTable.
*            
*            The  XML representation of  the AddressTable  must follow 
*            the schema defined in 
*            
*            http://xdaq.web.cern.ch/xdaq/xsd/HardwareAddressTable-{version}.xsd .
*            
*            Note that the description if  given for an item must be a
*            one line item. (Subsequent lines are ignored)
*
*       @see AddressTable, AddressTableInterface, VMEAddressTable
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class VMEAddressTableXMLFileReader : public AddressTableReader {
public:
  VMEAddressTableXMLFileReader( std::string fileName )
    throw( XMLProcessingException );
};

} /* namespace HAL */

#endif /* __VMEAddressTableXMLFileReader */
