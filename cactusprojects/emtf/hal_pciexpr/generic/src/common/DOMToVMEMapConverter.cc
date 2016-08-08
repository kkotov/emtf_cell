#include "hal/DOMToVMEMapConverter.hh"
#include <sstream>
#include <iomanip>

using namespace XERCES_CPP_NAMESPACE;

void HAL::DOMToVMEMapConverter::convert( DOMDocument* doc, 
					 std::list<HAL::AddressTableItem*>& itemPointerList )
  throw ( HAL::XMLProcessingException ) {

  DOMNodeList* l = doc->getElementsByTagName(XMLString::transcode("CARD_TYPE"));	
  if ( l->getLength() != 1 ) {
    std::string text = "There is not exactly one CARD_TYPE node in the DOM\n     (HAL::DOMToVMEMapConverter::convert)";
    throw( HAL::XMLProcessingException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }
  DOMNodeList* itemList = doc->getElementsByTagName( XMLString::transcode("VME_ADDRESS") );
  if ( itemList->getLength() == 0 ) {
    itemList = doc->getElementsByTagName( XMLString::transcode("VME64X_ADDRESS") );
  }
  DOMNode* item;
  for (uint32_t i=0; i<itemList->getLength(); i++) {
    item = itemList->item(i);
    processNode(item, itemPointerList);

  }
}

void HAL::DOMToVMEMapConverter::processNode(DOMNode* item, 
					    std::list<HAL::AddressTableItem*>& itemPointerList) 
  throw (HAL::XMLProcessingException) {
  uint32_t address, mask, am, width, windowId;
  std::string description(""), access, key, spaceStr;
  enum AddressSpace space;
  // Frank wants the keys as attributes::
  DOMNamedNodeMap* attributes = item->getAttributes();
  const XMLCh* valPtr = attributes->getNamedItem(XMLString::transcode("ITEM_NAME"))->getNodeValue();
  char* xmlChar = XMLString::transcode(valPtr);
  key = std::string( xmlChar );
  XMLString::release( &xmlChar );

  // all other column as subelements:
  findNamedChild( item, "ADDRESS", address);
  findNamedChild( item, "MASK", mask);

  // separate VME from VME64x
  // VME64x: configuration space items do not need the MAP entry (it is ignored)
  //           but they myst have a width item.
  //         memory items must have a map item. The width item is not needed (ignored)
  //         there is no AM item since the AM is determined by the host.
  // VME   : there are no space and map entries
  //         there must be AM and width entries
  bool isVme64x;
  if ( findNamedChild( item, "SPACE", spaceStr ) ) {
    isVme64x = true;
    if ( strcasecmp( spaceStr.c_str(), "MEMORY") == 0 ) {
       findNamedChild( item, "MAP", windowId);
       space = HAL::MEMORY;
       width = 4; // not used
    } else if ( strcasecmp( spaceStr.c_str(), "CONFIGURATION" ) == 0 ) {
      space = HAL::CONFIGURATION;
      findNamedChild( item, "WIDTH", width);
      windowId = 0;
    } else {
      std::string text = "Illegal Address Space: " + spaceStr + 
        "\n     (HAL::DOMToVMEMapConverter::processNode)";
      throw( HAL::XMLProcessingException( text, __FILE__, __LINE__, __FUNCTION__ ) );
    }
    am = 0;
  } else {
    isVme64x = false;
    windowId = 0;
    space = HAL::MEMORY;
    findNamedChild( item, "ADDRESS_MODIFIER", am);
    findNamedChild( item, "WIDTH", width);
  }
  
  findNamedChild( item, "DESCRIPTION", description);
  findNamedChild( item, "READ_OR_WRITE", access);
 
  bool isReadable, isWritable;
  if (access == "r") {
    isReadable = true;
    isWritable = false;
  } else if ( access == "w" ) {
    isReadable = false;
    isWritable = true;
  } else if ( access == "rw" ) {
    isReadable = true;
    isWritable = true;
  } else {
      std::string text = access + " is not an allowed access modifier\n     (HAL::DOMToVMEMapConverter::processNode)";
      throw( HAL::XMLProcessingException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }
  // load the list :
  HAL::GeneralHardwareAddress* addressPointer;
  if ( isVme64x ) {
    addressPointer = new HAL::VME64xHardwareAddress(address, space, windowId, width);
  } else {
    addressPointer = new HAL::VMEHardwareAddress(address, am, width);
  }
  HAL::AddressTableItem* itemPointer = 
    new HAL::AddressTableItem(key, description, *addressPointer, 
			 mask, isWritable, isReadable);
  itemPointerList.push_back(itemPointer);  
}


void HAL::DOMToVMEMapConverter::getString( const DOMNode* node, std::string& text ) {
  text = "";
  if ( node->hasChildNodes() ) {
    const XMLCh *xmlChar =  node->getFirstChild()->getNodeValue();
    char *textChar = XMLString::transcode(xmlChar);
    text = std::string(textChar);
    XMLString::release( &textChar );
  }
}

void HAL::DOMToVMEMapConverter::getUnsignedLong( const DOMNode* node, uint32_t& number ) {
  number = 0;
  if ( node->hasChildNodes() ) {
    const XMLCh* xmlChar = node->getFirstChild()->getNodeValue();
    char *textChar = XMLString::transcode(xmlChar);
    std::istringstream text(textChar);
    XMLString::release( &textChar );
    std::string str;
    text >> std::ws >> str;
    std::istringstream textCopy(str);
    if ( str[0] == '0' && ( str[1] == 'x' || str[1] == 'X' )) {
      textCopy.get();
      textCopy.get();
      textCopy >> std::hex >> number;
    } else {
      textCopy >> number;
    }
  }
}

bool HAL::DOMToVMEMapConverter::findNamedChild( const DOMNode* node, std::string nodeName, uint32_t& value) {
  bool result = false;
  DOMNodeList* children = node->getChildNodes();
  for ( uint32_t i=0; ( i<children->getLength()) && (result == false); i++ ) {
    XMLCh* compXmlCh = XMLString::transcode(nodeName.c_str());
    if ( XMLString::equals( children->item(i)->getNodeName(), compXmlCh ) ) {
      result = true;
      getUnsignedLong( children->item(i), value );
    }
    XMLString::release( &compXmlCh );
  }
  return result;
}
  
bool HAL::DOMToVMEMapConverter::findNamedChild( const DOMNode* node, std::string nodeName, std::string& value) {
  bool result = false;
  DOMNodeList* children = node->getChildNodes();
  for ( uint32_t i=0; ( i<children->getLength()) && (result == false); i++ ) {
    XMLCh* compXmlCh = XMLString::transcode(nodeName.c_str());
    if ( XMLString::equals( children->item(i)->getNodeName(),compXmlCh ) ) {
      result = true;
      getString( children->item(i), value );
    }
    XMLString::release( &compXmlCh );
  }
  return result;
}
