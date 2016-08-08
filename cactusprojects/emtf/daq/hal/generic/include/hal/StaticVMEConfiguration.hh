#ifndef __StaticVMEConfigurationInterface
#define __StaticVMEConfigurationInterface

#include "hal/StaticVMEConfigurationReader.hh"
#include <list>

namespace HAL {

class StaticVMEConfiguration {
public:
  StaticVMEConfiguration( StaticVMEConfigurationReader& reader );
  
  virtual ~StaticVMEConfiguration();

  std::list< StaticVMEItem* >::const_iterator getListBegin() const;
  std::list< StaticVMEItem* >::const_iterator getListEnd() const;

private:
  std::list< StaticVMEItem* > staticVMEItemList_;
};

} /* namespace HAL */

#endif /* __StaticVMEConfigurationInterface */
