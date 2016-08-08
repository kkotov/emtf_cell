#include "hal/VMEAddressTableDOMReader.hh"

HAL::VMEAddressTableDOMReader::VMEAddressTableDOMReader( XERCES_CPP_NAMESPACE::DOMDocument* doc )
  throw ( HAL::XMLProcessingException ) {

  HAL::DOMToVMEMapConverter::convert( doc, itemPointerList );
}
