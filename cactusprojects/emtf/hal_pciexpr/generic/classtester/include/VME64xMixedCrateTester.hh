#ifndef __VME64xMixedCrateTester
#define __VME64xMixedCrateTester

#include "TesterInterface.hh"
#include <string>
#include <iostream>
#include "VME64xDummyBusAdapter.hh"
#include "ASCIIFileAddressTableContainer.hh"
#include "ASCIIFileModuleMapper.hh"
#include "VME64xCrate.hh"
#include "IllegalValueException.hh"
#include "NoSuchFileException.hh"
#include "StaticVMEConfigurationASCIIFileReader.hh"

using std::string;
using std::cout;




namespace HAL {

class VME64xMixedCrateTester:public HAL::TesterInterface {
public:
  VME64xMixedCrateTester( string configSpaceDefinitionFile,
			  string moduleMapperFile,
			  string addressTableContainerFile,
			  string staticConfigurationFile)
    throw( IllegalValueException,
	   NoSuchFileException,
	   XMLProcessingException );
  virtual ~VME64xMixedCrateTester() {};
  bool execute();
  string getName()const;
  string getDescription()const;
private:
  VME64xDummyBusAdapter busAdapter_;
  ASCIIFileModuleMapper moduleMapper_;
  ASCIIFileAddressTableContainer addressTableContainer_;
  StaticVMEConfigurationASCIIFileReader staticConfigurationFilerReader_;
  StaticVMEConfiguration staticVMEConfiguration_;
};

} /* namespace HAL */

#endif /* __VME64xMixedCrateTester */

