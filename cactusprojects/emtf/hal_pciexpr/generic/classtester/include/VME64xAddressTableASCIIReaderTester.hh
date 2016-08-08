#ifndef __VME64xAddressTableASCIIReaderTester
#define __VME64xAddressTableASCIIReaderTester

#include "VMEAddressTableASCIIReader.hh"
#include "VMEAddressTable.hh"
#include "TesterInterface.hh"

#define VME64xASCIIADDRESSTABLE "VME64xAsciiAddressTable.dat"


namespace HAL {

class VME64xAddressTableASCIIReaderTester:public HAL::TesterInterface {
public:
  bool execute();
  std::string getName() const {
    return std::string("VME64xAddressTableASCIIReaderTester");
  }
  std::string getDescription() const;
};

} /* namespace HAL */

#endif /* __VME64xAddressTableASCIIReaderTester */






