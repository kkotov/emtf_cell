#include "VMEAddressTableTester.hh"
#include "VME64xAddressTableASCIIReaderTester.hh"
#include "XMLAddressTableTester.hh"
#include "PollItemCommandTester.hh"
#include "TestMaster.hh"
#include "VMEConfigurationSpaceHandlerTester.hh"
#include "VMEConfigurationSpaceAddressReaderTester.hh"
#include "VME64xMappedWindowTesterStandard.hh"
#include "VME64xFunctionTesterStandard.hh"
#include "VME64xCrateTester.hh"
#include "VME64xMixedCrateTester.hh"

#define MODULE_MAP_FILE "ModuleMapperFile.dat"
#define ADDERSSTABLE_CONTAINER_FILE "AddressTableMap.dat"
#define CONFIGSPACE_MAP "configSpace.dat"
#define STATIC_CONFIGURATION_FILE "StaticConfiguration.dat"

int main() {

  try {
    HAL::TestMaster tester;
    HAL::VME64xCrateTester vme64xCrateTester(CONFIGSPACE_MAP, 
					     MODULE_MAP_FILE, 
					     ADDERSSTABLE_CONTAINER_FILE);
    
    tester.registerTester( vme64xCrateTester );
        
    tester.doTest();
    std::cout << "\n\n\nLong test report:\n";
    tester.printLongReport();
    std::cout << "\n\n\nShort test report:\n";
    tester.printShortReport();
  } catch (HAL::HardwareAccessException& e) {
    std::cout << "UNEXPECTED Exeption : " << std::endl;
    std::cout << e.what() << std::endl;
  }
}
