#ifndef __DOMToPCIMapConverter
#define __DOMToPCIMapConverter

#include <string>
#include <list>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include "hal/AddressTableItem.hh"
#include "hal/XMLProcessingException.hh"
#include "hal/PCIHardwareAddress.hh"
#include "hal/AddressTableItem.hh"

namespace HAL {

/**
*
*
*     @short Converts a DOM-tree to a list of AddressTableItems.
*            
*            This  is  a  utility  which  does not  contain  data.  It 
*            contains one static public function. 
*
*       @see PCIAddressTableXMLFileReader
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/

class DOMToPCIMapConverter {
public:
  /**
   * Convert a DOM node to a list of AddressTableItems. 
   * This routine is used by the PCIAddressTableXMLFileReader. Since 
   * other readers might have to perform the same task, the service has 
   * been isolated into this static object.
   */
  static void convert( XERCES_CPP_NAMESPACE::DOMDocument* doc, 
                       std::list<AddressTableItem*>& itemPointerList )
    throw( XMLProcessingException );

private:
  static void processNode(XERCES_CPP_NAMESPACE::DOMNode* item, 
                          std::list<AddressTableItem*>& itemPointerList)
    throw( XMLProcessingException );

  static void getString( const XERCES_CPP_NAMESPACE::DOMNode* node, std::string& text);

  static void getUnsignedLong( const XERCES_CPP_NAMESPACE::DOMNode* node, uint32_t& number);

  static bool findNamedChild( const XERCES_CPP_NAMESPACE::DOMNode* node, std::string nodeName, uint32_t& value);

  static bool findNamedChild( const XERCES_CPP_NAMESPACE::DOMNode* node, std::string nodeName, std::string& value);
};

} /* namespace HAL */

#endif /* __DOMToPCIMapConverter */
