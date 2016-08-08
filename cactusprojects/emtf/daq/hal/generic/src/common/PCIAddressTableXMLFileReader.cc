#include "hal/PCIAddressTableXMLFileReader.hh"
#include <sstream>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/sax/SAXException.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/sax/HandlerBase.hpp>

using namespace XERCES_CPP_NAMESPACE;

HAL::PCIAddressTableXMLFileReader::PCIAddressTableXMLFileReader( std::string fileName )
  throw ( HAL::XMLProcessingException ) {
  // Initialize the XML4C system
  try {
    XMLPlatformUtils::Initialize();
  } catch (const XMLException& toCatch) {
    throw( HAL::XMLProcessingException( "Error during Xerces-initialization", __FILE__, __LINE__, __FUNCTION__));
  }
  // Instantiate the DOM parser.
  XercesDOMParser* parser = new XercesDOMParser();
  parser->setValidationScheme(XercesDOMParser::Val_Always);
  parser->setDoNamespaces(true);
  parser->setDoSchema(true);
  parser->setValidationSchemaFullChecking(false); // this is default
  parser->setCreateEntityReferenceNodes(true);  // this is default
  parser->setIncludeIgnorableWhitespace (false);


  HAL::XMLParserErrorHandler halHandler( fileName );
  parser->setErrorHandler( &halHandler );

  try {
    parser->parse(fileName.c_str());

  } catch (const XMLException& toCatch) {
    const XMLCh* didi(toCatch.getMessage());
    char* dada = XMLString::transcode(didi);
    std::string errorstr(dada);
    XMLString::release( &dada );
    std::string text = errorstr;
    XMLPlatformUtils::Terminate();
    throw(HAL::XMLProcessingException( text, __FILE__, __LINE__, __FUNCTION__ ));

  } catch (const DOMException& toCatch) {
    const XMLCh* dada = toCatch.msg;
    char* strChar = XMLString::transcode( dada );
    std::string errorstr( strChar );
    XMLString::release( &strChar );
    std::string text = errorstr;
    XMLPlatformUtils::Terminate();
    throw( HAL::XMLProcessingException( text, __FILE__, __LINE__, __FUNCTION__ ));

  } catch ( const SAXParseException& exception ) {
    // This exceptions is thrown in the errorHandler. We treat it a little
    // bit more in detail in order to give the user info where to find the
    // error in the xml-document.
    std::stringstream text;
    text << "Error during parsing file " << fileName
         << "\n     at line " << exception.getLineNumber()
         << " :\n";
    const XMLCh* dodo(exception.getMessage());
    char* xmlChar = XMLString::transcode(dodo);
    std::string errorstr(xmlChar);
    text << "     " << errorstr
         << std::ends;
    XMLString::release( &xmlChar );
    throw(HAL::XMLProcessingException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));

  } catch ( const SAXException& toCatch ) {
    const XMLCh* dodo = toCatch.getMessage();
    char* dada=XMLString::transcode(dodo);
    std::string errorstr(dada);
    XMLString::release( &dada );
    std::string text = errorstr;
    XMLPlatformUtils::Terminate();
    throw(HAL::XMLProcessingException( text, __FILE__, __LINE__, __FUNCTION__ ));

  } catch ( const HAL::XMLProcessingException& ex ) {
   XMLPlatformUtils::Terminate();
    throw(ex);

  } catch (...) {
    std::string text = "\nUnexpected exception during parsing: " + fileName  + "\n";
    XMLPlatformUtils::Terminate();
    throw( HAL::XMLProcessingException( text, __FILE__, __LINE__, __FUNCTION__ ));
  }
  DOMDocument* doc = parser->getDocument();

  HAL::DOMToPCIMapConverter::convert( doc, itemPointerList );

  delete( parser );

  XMLPlatformUtils::Terminate();
}
