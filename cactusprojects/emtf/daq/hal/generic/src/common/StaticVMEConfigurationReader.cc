#include "hal/StaticVMEConfigurationReader.hh"

HAL::StaticVMEConfigurationReader::StaticVMEConfigurationReader() {
  firstAccess_ = true;
}


/**
 * Must delete all items which have not been retrieved yet.
 */
HAL::StaticVMEConfigurationReader::~StaticVMEConfigurationReader() {
  if ( firstAccess_ ) it = staticVMEItemList_.begin();
  while ( it != staticVMEItemList_.end() ) {
    delete (*it);
    it++;
  }
  
}

bool HAL::StaticVMEConfigurationReader::next( HAL::StaticVMEItem** nextItem ) {
  if ( firstAccess_ )  {
    it = staticVMEItemList_.begin();
    firstAccess_ = false;
  }
  if ( it != staticVMEItemList_.end() ) {
    *nextItem = *it;
    it++;
    return true;
  } else {
    return false;
  }
}

