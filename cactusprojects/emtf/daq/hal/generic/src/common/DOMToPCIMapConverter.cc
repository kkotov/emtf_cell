#include "hal/DOMToPCIMapConverter.hh"
#include <sstream>
#include <iomanip>

using namespace XERCES_CPP_NAMESPACE;

void HAL::DOMToPCIMapConverter::convert( DOMDocument* doc, 
					 std::list<HAL::AddressTableItem*>& itemPointerList )
  throw ( HAL::XMLProcessingException ) {

  DOMNodeList* l = doc->getElementsByTagName(XMLString::transcode("CARD_TYPE"));	
  if (l->getLength() != 1) {
    std::string text = "There is not exactly one CARD_TYPE node in the DOM\n     (HAL::DOMToPCIMapConverter::convert)";
    throw( HAL::XMLProcessingException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }
  DOMNodeList* itemList = doc->getElementsByTagName(XMLString::transcode("PCI_ADDRESS"));
  DOMNode* item;
  for (uint32_t i=0; i<itemList->getLength(); i++) {
    item = itemList->item(i);
    processNode(item, itemPointerList);

  }
}

void HAL::DOMToPCIMapConverter::processNode(DOMNode* item, 
					    std::list<HAL::AddressTableItem*>& itemPointerList) 
  throw (HAL::XMLProcessingException) {
  uint32_t address, mask, bar;
  std::string description(""), access, key, space;

  // Frank wants the keys as attributes::
  DOMNamedNodeMap* attributes = item->getAttributes();
  const XMLCh* valPtr = attributes->getNamedItem(XMLString::transcode("ITEM_NAME"))->getNodeValue();
  char* xmlChar = XMLString::transcode(valPtr);
  key = std::string(xmlChar );
  XMLString::release( &xmlChar );

  // all other column as subelements:
  findNamedChild( item, "ADDRESS", address);
  findNamedChild( item, "MASK", mask);
  findNamedChild( item, "BAR", bar);

  findNamedChild( item, "SPACE", space);
  findNamedChild( item, "DESCRIPTION", description);
  findNamedChild( item, "READ_OR_WRITE", access);
  
  AddressSpace pciSpace;
  if ( strcasecmp( space.c_str(), "CONFIGURATION") == 0 ) {
    pciSpace = AddressSpace(CONFIGURATION);
  } else if ( strcasecmp( space.c_str(), "MEMORY") == 0 ) {
    pciSpace = AddressSpace(MEMORY);
  } else {
    std::string text="The pci space " + space +
      "is not implemented. \n    (HAL::DOMToPCIMapConverter::processNode)";
    throw (HAL::XMLProcessingException( text, __FILE__, __LINE__, __FUNCTION__ ));
  }
  
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
      std::string text = access + " is not an allowed access modifier\n     (HAL::DOMToPCIMapConverter::processNode)";
      throw( HAL::XMLProcessingException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }
  // load the list :
  HAL::PCIHardwareAddress* addressPointer = new HAL::PCIHardwareAddress(address, pciSpace, bar );
  HAL::AddressTableItem* itemPointer = 
    new HAL::AddressTableItem(key, description, *addressPointer, 
			 mask, isWritable, isReadable);
  itemPointerList.push_back(itemPointer);
}

void HAL::DOMToPCIMapConverter::getString( const DOMNode* node, std::string& text ) {
  text = "";
  if ( node->hasChildNodes() ) {
    const XMLCh *xmlChar =  node->getFirstChild()->getNodeValue();
    char *textChar = XMLString::transcode(xmlChar);
    text = std::string(textChar);
    XMLString::release( &textChar );
  }
}

void HAL::DOMToPCIMapConverter::getUnsignedLong( const DOMNode* node, uint32_t& number ) {
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
  
bool HAL::DOMToPCIMapConverter::findNamedChild( const DOMNode* node, std::string nodeName, uint32_t& value) {
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
  

bool HAL::DOMToPCIMapConverter::findNamedChild( const DOMNode* node, std::string nodeName, std::string& value) {
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
