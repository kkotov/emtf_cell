#ifndef __VME64xMappedWindowTesterStandard
#define __VME64xMappedWindowTesterStandard

#include "TesterInterface.hh"
#include "VME64xMappedWindow.hh"


namespace HAL {

class VME64xMappedWindowTesterStandard : public HAL::TesterInterface {
public:
  bool execute();
  std::string getName() const;
  std::string getDescription() const;


};

} /* namespace HAL */

#endif /* __VME64xMappedWindowTesterStandard */
