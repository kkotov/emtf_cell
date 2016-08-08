#include "VMEConfigurationSpaceHandlerTester.hh"
#include <iomanip>

std::string HAL::VMEConfigurationSpaceHandlerTester::getName() const {
  return "VMEConfigurationSpaceHandlerTester";
}

std::string HAL::VMEConfigurationSpaceHandlerTester::getDescription() const {
  return "Tests of the HAL::VMEConfigurationSpaceHandler";
}

bool HAL::VMEConfigurationSpaceHandlerTester::execute() {

  bool result = true;
  try {
    HAL::VME64xDummyBusAdapter busAdapter(CONFIGSPACEDEFINITIONFILE, HAL::VME64xDummyBusAdapter::VERBOSE_ON, HAL::VME64xDummyBusAdapter::MEMORY_MAP_OFF);
    HAL::VMEConfigurationSpaceHandler handler(busAdapter);
    uint32_t slotId;
    bool modulePresent;
    for ( slotId=0; slotId<MAX_NUMBER_OF_SLOTS; slotId++ ) {
      modulePresent = handler.containsVME64xModule( slotId );
      std::cout << "    Slot " << std::dec << std::setw(2) 
		<< slotId << " has VME64x module : " 
		<< modulePresent << std::endl;
      if ( modulePresent ) {
        std::string serial = handler.readSerialNumber( slotId );
        std::cout << "        Serial number is : " << serial << std::endl;
        for ( int funcId = 0; funcId < NUMBER_OF_VME64XFUNCTIONS; funcId ++ ) {
          if (  handler.functionIsImplemented( slotId, funcId ) ) { 
            std::cout << "        function : " << std::dec << funcId 
		      << " IS implemented" << std::endl;

          } else {
            std::cout << "        function : " << std::dec << funcId 
		      << " is NOT implemented" << std::endl;
          }
        }
        if ( (handler.checksumOk( slotId )) == false ) {
          std::cout << "        Checksum error! " << std::endl;
          result = false;
        } else {
          std::cout << "        Checksum is ok" << std::endl;
        }
      }
    }
  } catch( HAL::HardwareAccessException& e ) {
    std::cout << "Unexpected exception: " << std::endl;
    std::cout << e.what() << std::endl;
    result = false;
  }
  return result;
}
