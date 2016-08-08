#include "hal/XMLParserErrorHandler.hh"

HAL::XMLParserErrorHandler::XMLParserErrorHandler( std::string xmlFile ) {
  this->xmlFile = xmlFile;
}

void HAL::XMLParserErrorHandler::error( const XERCES_CPP_NAMESPACE::SAXParseException &exception ) 
  throw (XERCES_CPP_NAMESPACE::SAXParseException) {
  throw ( exception );
}

void HAL::XMLParserErrorHandler::fatalError( const XERCES_CPP_NAMESPACE::SAXParseException &exception ) 
  throw (XERCES_CPP_NAMESPACE::SAXParseException) {
  throw ( exception );
}

void HAL::XMLParserErrorHandler::warning( const XERCES_CPP_NAMESPACE::SAXParseException &exception ) 
  throw (XERCES_CPP_NAMESPACE::SAXParseException) {
  throw ( exception );
}
