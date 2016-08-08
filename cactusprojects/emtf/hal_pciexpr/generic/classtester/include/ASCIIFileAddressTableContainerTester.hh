#ifndef __ASCIIFileAddressTableContainerTester
#define __ASCIIFileAddressTableContainerTester

#include <iostream>
#include "ASCIIFileAddressTableContainer.hh"
#include "TesterInterface.hh"
#include "ASCIIFileModuleMapper.hh"
#include "IllegalValueException.hh"
#include "NoSuchFileException.hh"
#include "XMLProcessingException.hh"
#include "VMEAddressTable.hh"

using std::cout;

#define VMEXMLADDRESSTABLE_1 "VMEXMLTable-1.xml"
#define VME64xXMLADDRESSTABLE_1 "VME64xXMLTable-1.xml"
#define PCIXMLADDRESSTABLE_1 "PCIXMLTable-1.xml"


namespace HAL {

class ASCIIFileAddressTableContainerTester:public HAL::TesterInterface {
public:

  ASCIIFileAddressTableContainerTester( std::string moduleMapperFile,
					std::string addressTableContainerFilebool )
    throw( IllegalValueException,
	   NoSuchFileException,
	   XMLProcessingException,
	   HardwareAccessException );
    
  virtual ~ASCIIFileAddressTableContainerTester() {};

  bool execute();
  std::string getName() const {
    return std::string("ASCIIFileAddressTableContainerTester");
  }
  std::string getDescription() const;

private:
  ASCIIFileModuleMapper moduleMapper_;
  ASCIIFileAddressTableContainer addressTableContainer_;
};
} /* namespace HAL */

#endif /* __ASCIIFileAddressTableContainerTester */

