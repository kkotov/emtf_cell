#ifndef __VMEConfigurationSpaceHandlerTester
#define __VMEConfigurationSpaceHandlerTester

#include "TesterInterface.hh"
#include "VMEAddressTable.hh"
#include "VMEConfigurationSpaceHandler.hh"
#include "VME64xDummyBusAdapter.hh"

#define CONFIGSPACEDEFINITIONFILE "configSpace.dat"


namespace HAL {

class VMEConfigurationSpaceHandlerTester : public HAL::TesterInterface {
public:
  bool execute();
  std::string getName() const;
  std::string getDescription() const;
};

} /* namespace HAL */

#endif /* __VMEConfigurationSpaceHandlerTester */
