#ifndef __XMLParserErrorHandler
#define __XMLParserErrorHandler

#include <string>

#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include "hal/XMLProcessingException.hh"

namespace HAL {

class XMLParserErrorHandler:public XERCES_CPP_NAMESPACE::HandlerBase {
public:
  XMLParserErrorHandler( std::string xmlFile );
  void error( const XERCES_CPP_NAMESPACE::SAXParseException &exception )
    throw(XERCES_CPP_NAMESPACE::SAXParseException);
  void fatalError( const XERCES_CPP_NAMESPACE::SAXParseException &exception )
    throw(XERCES_CPP_NAMESPACE::SAXParseException);
  void warning( const XERCES_CPP_NAMESPACE::SAXParseException &exception )
    throw(XERCES_CPP_NAMESPACE::SAXParseException);
private:
  std::string xmlFile;
};

} /* namespace HAL */

#endif /* __XMLParserErrorHandler */
