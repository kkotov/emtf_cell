#include "hal/PCIAddressTableDOMReader.hh"

HAL::PCIAddressTableDOMReader::PCIAddressTableDOMReader( DOMDocument* doc )
  throw ( HAL::XMLProcessingException ) {

  HAL::DOMToPCIMapConverter::convert( doc, itemPointerList );
}
