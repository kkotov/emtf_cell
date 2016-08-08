#include "hal/VMEAddressTableXMLFileReader.hh"
#include <sstream>

using namespace XERCES_CPP_NAMESPACE;

HAL::VMEAddressTableXMLFileReader::VMEAddressTableXMLFileReader( std::string fileName )
  throw ( HAL::XMLProcessingException ) {

  // Initialize the XML4C system
  try {
    XMLPlatformUtils::Initialize();
  } catch (const XMLException& toCatch) {
    throw( HAL::XMLProcessingException( "Error during Xerces-initialization\n     (HAL::VMEAddressTableXMLFileReader::VMEAddressTableXMLFileReader)", __FILE__, __LINE__, __FUNCTION__ ));
  }

  // Instantiate the DOM parser.

  XercesDOMParser* parser = new  XercesDOMParser;
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
    char * errptr = XMLString::transcode(toCatch.getMessage());
    std::string errorstr(errptr);
    XMLString::release( &errptr );
    std::string text = errorstr + "\n" +
      "     (HAL::VMEAddressTableXMLFileReader::VMEAddressTableXMLFileReader)";
    XMLPlatformUtils::Terminate();

    throw(HAL::XMLProcessingException( text, __FILE__, __LINE__, __FUNCTION__ ));

  } catch (const DOMException& toCatch) {
    char * errptr = XMLString::transcode(toCatch.msg);
    std::string errorstr(errptr);
    XMLString::release( &errptr );
    std::string text = errorstr + "\n    " +
      "    (HAL::VMEAddressTableXMLFileReader::VMEAddressTableXMLFileReader)";
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
    char * errptr = XMLString::transcode(exception.getMessage());
    std::string message( errptr );
    text << "     " << message
         << "\n     (HAL::VMEAddressTableXMLFileReader::VMEAddressTableXMLFileReader)"
         << std::ends;
    XMLString::release( &errptr );    
    XMLPlatformUtils::Terminate();
    throw(HAL::XMLProcessingException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));

  } catch ( const SAXException& toCatch ) {
    char * errptr = XMLString::transcode(toCatch.getMessage());
    std::string errorstr(errptr);
    std::string text = errorstr + "\n" +
      "     (HAL::VMEAddressTableXMLFileReader::VMEAddressTableXMLFileReader)";
    XMLString::release( &errptr );    
    XMLPlatformUtils::Terminate();
    throw(HAL::XMLProcessingException( text, __FILE__, __LINE__, __FUNCTION__ ));

  } catch (...) {
    std::string text = "\nUnexpected exception during parsing: " + fileName  + "\n";
    XMLPlatformUtils::Terminate();
    throw( HAL::XMLProcessingException( text, __FILE__, __LINE__, __FUNCTION__ ));
  }

  DOMDocument* doc = parser->getDocument();

  HAL::DOMToVMEMapConverter::convert( doc, itemPointerList );

  delete parser;

  XMLPlatformUtils::Terminate();
}
