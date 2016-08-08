#ifndef __VME64xDummyBusAdapterTester
#define __VME64xDummyBusAdapterTester

#include "TesterInterface.hh"
#include <string>
#include <iostream>
#include "VME64xDummyBusAdapter.hh"
#include "ASCIIFileAddressTableContainer.hh"
#include "ASCIIFileModuleMapper.hh"
#include "VME64xCrate.hh"
#include "IllegalValueException.hh"
#include "NoSuchFileException.hh"
#include "XMLProcessingException.hh"
#include "ConfigCSRDumper.hh"

using std::string;
using std::cout;


namespace HAL {

class VME64xDummyBusAdapterTester:public HAL::TesterInterface {
public:
  VME64xDummyBusAdapterTester( string configSpaceDefinitionFile,
			  string moduleMapperFile,
			  string addressTableContainerFile )
    throw( IllegalValueException,
	   NoSuchFileException,
	   XMLProcessingException,
	   HardwareAccessException);
  virtual ~VME64xDummyBusAdapterTester() {};
  bool execute();
  string getName()const;
  string getDescription()const;
private:
  VME64xDummyBusAdapter busAdapter_;
  ASCIIFileModuleMapper moduleMapper_;
  ASCIIFileAddressTableContainer addressTableContainer_;
};

} /* namespace HAL */

#endif /* __VME64xDummyBusAdapterTester */

