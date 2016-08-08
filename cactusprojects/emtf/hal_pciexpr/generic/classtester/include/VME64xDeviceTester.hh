#ifndef __VME64xDeviceTester
#define __VME64xDeviceTester

#include "TesterInterface.hh"
#include <string>
#include <iostream>
#include "VME64xDummyBusAdapter.hh"
#include "ASCIIFileAddressTableContainer.hh"
#include "ASCIIFileModuleMapper.hh"
#include "VME64xCrate.hh"
#include "VME64xDevice.hh"
#include "IllegalValueException.hh"
#include "NoSuchFileException.hh"
#include "XMLProcessingException.hh"
#include "ConfigCSRDumper.hh"
#include "ConfigROMDumper.hh"

using std::string;
using std::cout;


namespace HAL {

class VME64xDeviceTester:public HAL::TesterInterface {
public:
  VME64xDeviceTester( string configSpaceDefinitionFile,
		      string moduleMapperFile,
		      string addressTableContainerFile )
    throw( IllegalValueException,
	   NoSuchFileException,
	   XMLProcessingException,
	   HardwareAccessException);
  virtual ~VME64xDeviceTester() {};
  bool execute();
  string getName()const;
  string getDescription()const;
private:
  VME64xDummyBusAdapter busAdapter_;
  ASCIIFileModuleMapper moduleMapper_;
  ASCIIFileAddressTableContainer addressTableContainer_;
  VME64xCrate crate_;
};

} /* namespace HAL */

#endif /* __VME64xDeviceTester */

