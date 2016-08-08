#ifndef __XMLAddressTableTester
#define __XMLAddressTableTester

#include "VMEAddressTableXMLFileReader.hh"
#include "VMEAddressTable.hh"
#include "PCIAddressTableXMLFileReader.hh"
#include "PCIAddressTable.hh"
#include "TesterInterface.hh"

#define VMEXMLADDRESSTABLE_1 "VMEXMLTable-1.xml"
#define VME64xXMLADDRESSTABLE_1 "VME64xXMLTable-1.xml"
#define PCIXMLADDRESSTABLE_1 "PCIXMLTable-1.xml"


namespace HAL {

class XMLAddressTableTester:public HAL::TesterInterface {
public:
  bool execute();
  std::string getName() const {
    return std::string("XMLAddressTableTester");
  }
  std::string getDescription() const;
};

} /* namespace HAL */

#endif /* __XMLAddressTableTester */

