#ifndef __VMEAddressTableTester
#define __VMEAddressTableTester

#include "VMEAddressTableASCIIReader.hh"
#include "VMEAddressTable.hh"
#include "TesterInterface.hh"

#define VMEADDRESSTABLE "VMEAsciiAddressTable.dat"


namespace HAL {

class VMEAddressTableTester:public HAL::TesterInterface {
public:
  bool execute();
  std::string getName() const {
    return std::string("VMEAddressTableTester");
  }
  std::string getDescription() const;
};

} /* namespace HAL */

#endif /* __VMEAddressTableTester */






