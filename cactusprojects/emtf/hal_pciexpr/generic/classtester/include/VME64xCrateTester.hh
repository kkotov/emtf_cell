#ifndef __VME64xCrateTester
#define __VME64xCrateTester

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

class VME64xCrateTester:public HAL::TesterInterface {
public:
  VME64xCrateTester( string configSpaceDefinitionFile,
                     string moduleMapperFile,
                     string addressTableContainerFile )
    throw( IllegalValueException,
	   NoSuchFileException,
	   XMLProcessingException,
	   HardwareAccessException);
  virtual ~VME64xCrateTester() {};
  bool execute();
  string getName()const;
  string getDescription()const;
private:
  VME64xDummyBusAdapter busAdapter_;
  ASCIIFileModuleMapper moduleMapper_;
  ASCIIFileAddressTableContainer addressTableContainer_;
};

} /* namespace HAL */

#endif /* __VME64xCrateTester */

