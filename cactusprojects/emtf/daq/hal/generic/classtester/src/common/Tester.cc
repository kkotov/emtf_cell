#include "ExceptionTester.hh"
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
#include "VME64xDeviceTester.hh"
#include "VME64xDummyBusAdapterTester.hh"
#include "VME64xMixedCrateTester.hh"
#include "ASCIIFileAddressTableContainerTester.hh"

#define MODULE_MAP_FILE "ModuleMapperFile.dat"
#define ADDRESSTABLE_CONTAINER_FILE "AddressTableMap.dat"
#define MIXED_ADDRESSTABLE_CONTAINER_FILE "MixedAddressTableMap.dat"
#define CONFIGSPACE_MAP "configSpace.dat"
#define ONE_MODULE_CONFIGSPACE_MAP "oneModuleConfigSpace.dat"
#define STATIC_CONFIGURATION_FILE "StaticConfiguration.dat"

int main() {

  try {
    HAL::TestMaster tester;
    
    HAL::ExceptionTester exceptionTester;
    tester.registerTester( exceptionTester );

    HAL::VMEAddressTableTester vmeAddressTableTester;
    tester.registerTester( vmeAddressTableTester );
 
    HAL::VME64xAddressTableASCIIReaderTester vme64xAddressTableASCIIReaderTester;
    tester.registerTester( vme64xAddressTableASCIIReaderTester );
    
    HAL::XMLAddressTableTester xmlAddressTableTester;
    tester.registerTester( xmlAddressTableTester );
    
    HAL::ASCIIFileAddressTableContainerTester asciiFileAddressTableContainerTester( MODULE_MAP_FILE, 
    									       MIXED_ADDRESSTABLE_CONTAINER_FILE);
    tester.registerTester( asciiFileAddressTableContainerTester );
    
    HAL::PollItemCommandTester pollItemCommandTester;
    tester.registerTester( pollItemCommandTester );
    
    HAL::VME64xMappedWindowTesterStandard vme64xMappedWindowTesterStandard;
    tester.registerTester( vme64xMappedWindowTesterStandard );
    
    HAL::VME64xFunctionTesterStandard vme64xFunctionTesterStandard;
    tester.registerTester( vme64xFunctionTesterStandard );
    
    HAL::VMEConfigurationSpaceHandlerTester vmeConfigurationSpaceHandlerTester;
    tester.registerTester( vmeConfigurationSpaceHandlerTester );
    
    HAL::VMEConfigurationSpaceAddressReaderTester vmeConfigurationSpaceAddressReaderTester;
    tester.registerTester( vmeConfigurationSpaceAddressReaderTester );
    
    HAL::VME64xCrateTester vme64xCrateTester(CONFIGSPACE_MAP, 
					     MODULE_MAP_FILE, 
					     ADDRESSTABLE_CONTAINER_FILE);
    
    tester.registerTester( vme64xCrateTester );
    
    HAL::VME64xMixedCrateTester vme64xMixedCrateTester(CONFIGSPACE_MAP, 
    						  MODULE_MAP_FILE, 
    						  ADDRESSTABLE_CONTAINER_FILE, 
    						  STATIC_CONFIGURATION_FILE);
    tester.registerTester( vme64xMixedCrateTester );
    
    HAL::VME64xDummyBusAdapterTester vme64xDummyBusAdapterTester(ONE_MODULE_CONFIGSPACE_MAP, 
    							    MODULE_MAP_FILE, 
    							    MIXED_ADDRESSTABLE_CONTAINER_FILE);
    tester.registerTester( vme64xDummyBusAdapterTester );
    
    HAL::VME64xDeviceTester  vme64xDeviceTester( CONFIGSPACE_MAP, 
    						 MODULE_MAP_FILE, 
    						 ADDRESSTABLE_CONTAINER_FILE);
    tester.registerTester( vme64xDeviceTester );

    tester.doTest();
    cout << "\n\n\nLong test report:\n";
    tester.printLongReport();
    cout << "\n\n\nShort test report:\n";
    tester.printShortReport();
    cout << "\n\n" << endl;
  } catch (HAL::HardwareAccessException& e) {
    cout << "UNEXPECTED Exeption : " << endl;
    cout << e.what() << endl;
  }
}
