#include "VME64xMappedWindowTesterStandard.hh"

std::string HAL::VME64xMappedWindowTesterStandard::getName() const {
  return("VME64xMappedWindowTesterStandard");
}

std::string HAL::VME64xMappedWindowTesterStandard::getDescription() const {
  std::string text = "Tests the class for with the constructor for the standard VME modules";
  return text;
}

bool HAL::VME64xMappedWindowTesterStandard::execute() {
  bool result = true;
  HAL::VME64xMappedWindow window = HAL::VME64xMappedWindow( 1,0x001000,3 );
  if ( window.getDataAccessWidth()!=0 ) result = false;
  if ( window.getAddressRank() != 3 ) result = false;
  if ( window.getAM() != 0  ) result = false;
  if ( window.getMappedWindowId() != 0 ) result = false;
  if ( window.getBaseaddress() != 0x001000 ) result = false;
  if ( ! window.isConfigured() ) result = false;
  if ( ! window.isImplemented() ) result = false;
  if ( window.hasAnotherWindow() ) result = false;

  try {
    window.setADER( 0x100, 0x39 );
    result = false;
  } catch( HAL::IllegalOperationException& e ) {
    std::cout << e.what();
  } catch ( HAL::HardwareAccessException& e ) {
    std::cout << e.what();
    result = false;
  }
  return result;
}
