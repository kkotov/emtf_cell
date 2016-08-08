#include "hal/StaticVMEConfiguration.hh"

HAL::StaticVMEConfiguration::StaticVMEConfiguration( HAL::StaticVMEConfigurationReader& reader ) {
  HAL::StaticVMEItem *nextItem;
  while ( reader.next( &nextItem ) ) {
    staticVMEItemList_.push_back( nextItem );
  }
}

HAL::StaticVMEConfiguration::~StaticVMEConfiguration( ) {
  std::list< HAL::StaticVMEItem* >::iterator it;
  for ( it = staticVMEItemList_.begin(); 
	it != staticVMEItemList_.end(); 
	it++ ) {
    delete *it;
  }
}

std::list< HAL::StaticVMEItem* >::const_iterator HAL::StaticVMEConfiguration::getListBegin() const {
  return staticVMEItemList_.begin();
}

std::list< HAL::StaticVMEItem* >::const_iterator HAL::StaticVMEConfiguration::getListEnd() const {
  return staticVMEItemList_.end();
}
