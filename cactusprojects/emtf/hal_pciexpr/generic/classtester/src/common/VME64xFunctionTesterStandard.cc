#include "VME64xFunctionTesterStandard.hh"

std::string HAL::VME64xFunctionTesterStandard::getName() const {
  return("VME64xFunctionTesterStandard");
}

std::string HAL::VME64xFunctionTesterStandard::getDescription() const {
  std::string text = "Tests the class for with the constructor for the standard VME modules";
  return text;
}

bool HAL::VME64xFunctionTesterStandard::execute() {
  bool result = true;
  HAL::VME64xFunction function = HAL::VME64xFunction( 7,0x001000,3 );
  if ( function.getNumberOfMappedWindows() != 1 ) result = false;
  if ( ! function.isImplemented() ) result = false;
  if ( function.getAddressRank() != 3 ) result = false;

  std::list< HAL::VME64xMappedWindow* > windowList;
  windowList = function.getMappedWindowPtrList();
  if ( windowList.size() != 1 ) result = false;

  HAL::VME64xMappedWindow* window = windowList.front();

  if ( window->getDataAccessWidth()!=0 ) result = false;
  if ( window->getAddressRank() != 3 ) result = false;
  if ( window->getAM() != 0  ) result = false;
  if ( window->getMappedWindowId() != 0 ) result = false;
  if ( window->getBaseaddress() != 0x001000 ) result = false;
  if ( ! window->isConfigured() ) result = false;
  if ( ! window->isImplemented() ) result = false;
  if ( window->hasAnotherWindow() ) result = false;

  try {
    window->setADER( 0x100, 0x39 );
    result = false;
  } catch( HAL::IllegalOperationException& e ) {
    std::cout << e.what();
  } catch ( HAL::HardwareAccessException& e ) {
    std::cout << e.what();
    result = false;
  }
  return result;
}
