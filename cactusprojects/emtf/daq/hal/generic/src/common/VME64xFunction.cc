#include "hal/VME64xFunction.hh"
#include <string>
#include <sstream>

HAL::VME64xFunction::VME64xFunction( uint32_t slotId,
				     uint32_t baseAddress,
				     uint32_t addressRank ) 
: slotId_(slotId) {
  functionId_ = 0;
  vmeConfigurationSpaceHandlerRef_ = (HAL::VMEConfigurationSpaceHandler*)0;
  HAL::VME64xMappedWindow* vmeWindow;
  vmeWindow = new HAL::VME64xMappedWindow( slotId_,
					   baseAddress,
					   addressRank);
  mappedWindowPtrList_.push_back( vmeWindow );
}

HAL::VME64xFunction::VME64xFunction( uint32_t slotId,
				     uint32_t functionId,
				     HAL::VMEConfigurationSpaceHandler* vmeAccess ) 
  throw( HAL::UnsupportedException, 
         HAL::IllegalValueException )
  : slotId_(slotId),
	functionId_(functionId),
	vmeConfigurationSpaceHandlerRef_(vmeAccess) {
  
  uint32_t mappedWindowId = functionId;
  // see if function is implemented: This is done by trying to make the first mapped Window
  HAL::VME64xMappedWindow *nextWindow = 
    new HAL::VME64xMappedWindow( slotId, functionId, mappedWindowId, vmeConfigurationSpaceHandlerRef_);
  if ( ! nextWindow->isImplemented() ) {
    return;
  }
  mappedWindowPtrList_.push_back( nextWindow );
  uint32_t oldAddressRank;
  while ( nextWindow->hasAnotherWindow() ) {
    oldAddressRank = nextWindow->getAddressRank();
    mappedWindowId++;
    nextWindow = 
      new HAL::VME64xMappedWindow( slotId, functionId, mappedWindowId, vmeConfigurationSpaceHandlerRef_);
    if ( ! nextWindow->isImplemented() ) {
      // error in configuration space
      std::stringstream text;
      text << "Error in configuration space of module in slot" << slotId
           << ". \nWindow " << mappedWindowId << "is not implemented but previous window "
           << " demands another window for the same function\n"
           << "    (HAL::VME64xFunction::VME64xFunction)" << std::ends;
      throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
    }
    if ( oldAddressRank != nextWindow->getAddressRank() ) {
      // error: all mapped windows should demand the same address space in this software
      std::stringstream text;
      text << "Error in configuration space of module in slot" << slotId
           << ".\nFunction " << functionId_ << "has more than one mapped Window, "
           << "but they have different sized decoders (ADEM bits),\n"
           << "which is not supported by this software" << std::ends;
      throw( HAL::UnsupportedException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
    }
    mappedWindowPtrList_.push_back( nextWindow );
  }
}

HAL::VME64xFunction::~VME64xFunction() {
  std::list< HAL::VME64xMappedWindow * >::iterator it;
  for ( it = mappedWindowPtrList_.begin(); it != mappedWindowPtrList_.end(); it++ ) {
    delete *it;
  }
}

uint32_t HAL::VME64xFunction::getNumberOfMappedWindows() const {
  return mappedWindowPtrList_.size();
}

bool HAL::VME64xFunction::isImplemented() const {
  return (mappedWindowPtrList_.size() > 0);
}

uint32_t HAL::VME64xFunction::getAddressRank() const {
  // The constructor made sure that all windows have the same Address rank.
  // Therefore we can just take the first element of the list to determine
  // the rank.
  if ( mappedWindowPtrList_.size() > 0 ) {
    return mappedWindowPtrList_.front()->getAddressRank();
  } else {
    return 0;
  }
}

//uint32_t HAL::VME64xFunction::getFunctionId() const {
//  return functionId_;
//}

std::list< HAL::VME64xMappedWindow* > HAL::VME64xFunction::getMappedWindowPtrList() const {
  return mappedWindowPtrList_;
}
