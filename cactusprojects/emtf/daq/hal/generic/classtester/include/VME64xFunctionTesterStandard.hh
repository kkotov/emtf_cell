#ifndef __VME64xFunctionTesterStandard
#define __VME64xFunctionTesterStandard

#include "TesterInterface.hh"
#include "VME64xFunction.hh"


namespace HAL {

class VME64xFunctionTesterStandard : public HAL::TesterInterface {
public:
  bool execute();
  std::string getName() const;
  std::string getDescription() const;


};

} /* namespace HAL */

#endif /* __VME64xFunctionTesterStandard */
