#ifndef __PollItemCommandTester
#define __PollItemCommandTester

#include "TesterInterface.hh"
#include "VMEDummyBusAdapter.hh"
#include "VMEAddressTableASCIIReader.hh"
#include "VMEDevice.hh"
#include "CommandSequenceASCIIReader.hh"
#include "CommandSequence.hh"

#define ADDRESSTABLE  "pollItemTable.dat"
#define SEQUENCE "pollItemSequence.seq"



namespace HAL {

class PollItemCommandTester : public HAL::TesterInterface {

public:
  bool execute();
  std::string getName() const {
    return std::string("PollItemCommandTester");
  }
  std::string getDescription() const;
};

} /* namespace HAL */

#endif /* __PollItemCommandTester */
