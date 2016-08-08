#ifndef __VMEConfigurationSpaceAddressReaderTester
#define __VMEConfigurationSpaceAddressReaderTester

#include "TesterInterface.hh"
#include "VMEAddressTable.hh"
#include "VMEConfigurationSpaceAddressReader.hh"


namespace HAL {

class VMEConfigurationSpaceAddressReaderTester : public HAL::TesterInterface {
public:
  bool execute();
  std::string getName() const;
  std::string getDescription() const;
};

} /* namespace HAL */

#endif /* __VMEConfigurationSpaceAddressReaderTester */
