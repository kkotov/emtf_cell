#include "VME64xDeviceTester.hh"

HAL::VME64xDeviceTester::VME64xDeviceTester( string configSpaceDefinitionFile,
                                      string moduleMapperFile,
                                      string addressTableContainerFile )
  throw( HAL::IllegalValueException,
	 HAL::NoSuchFileException,
	 HAL::XMLProcessingException,
	 HAL::HardwareAccessException )
  : busAdapter_( configSpaceDefinitionFile, 
                 HAL::VME64xDummyBusAdapter::VERBOSE_OFF, 
                 HAL::VME64xDummyBusAdapter::MEMORY_MAP_ON ),
    moduleMapper_(moduleMapperFile),
    addressTableContainer_(moduleMapper_,
			   addressTableContainerFile),
    crate_(busAdapter_, addressTableContainer_, moduleMapper_)
{

}

bool HAL::VME64xDeviceTester::execute() {
  bool result = true;
  try {
    crate_.printAddressMap( std::cout );
    uint32_t slotId = 10;
    std::cout << "try to get the device in slot " <<std::dec << slotId <<  std::endl;
    HAL::VME64xDevice* devicePtr = dynamic_cast<HAL::VME64xDevice*> (crate_.getVMEDevice( slotId ));
    if ( devicePtr == NULL ) {
      std::cout << "all went wrong "<< std::endl;
//      throw( HAL::HardwareAccessException( "could not cast to VME64xDevice! ", __FILE__, __LINE__, __FUNCTION__ ));
      exit(-1);
   } 
    // dump the config space of slot 
    HAL::VMEConfigurationSpaceHandler vmeConfigHandler( busAdapter_ );

    std::cout << "dumping CR" << std::endl;
    HAL::ConfigROMDumper crDumper;
    crDumper.dumpROM( vmeConfigHandler, slotId );

    std::cout << "dumping CSR" << std::endl;
    HAL::ConfigCSRDumper csrDumper;
    csrDumper.dumpCSR( vmeConfigHandler, slotId );

    // get the serial number
    std::string serialNo = devicePtr->getSerialNumber();
    std::cout << "serialNo is " << serialNo << std::endl;

    // read an std config item
    uint32_t res;
    std::string item = "romLength";
    devicePtr->stdConfigRead(item , &res );
    std::cout << "result for item " << item << " is : " << std::hex << res << std::endl;

    item = "CRWidth";
    devicePtr->stdConfigRead(item , &res );
    std::cout << "result for item " << item << " is : " << std::hex << res << std::endl;

    item = "CSRWidth";
    devicePtr->stdConfigRead(item , &res );
    std::cout << "result for item " << item << " is : " << std::hex << res << std::endl;

    item = "CRAMWidth";
    devicePtr->stdConfigRead(item , &res );
    std::cout << "result for item " << item << " is : " << std::hex << res << std::endl;

    item = "checksum";
    devicePtr->stdConfigRead(item , &res );
    std::cout << "result for item " << item << " is : " << std::hex << res << std::endl;

    item = "romLength";
    devicePtr->stdConfigRead(item , &res );
    std::cout << "result for item " << item << " is : " << std::hex << res << std::endl;

    item = "vme64x-csr-confItem1";
    devicePtr->read(item , &res );
    std::cout << "result for item " << item << " is : " << std::hex << res << std::endl;

  } catch( HAL::HardwareAccessException& e) {
    std::cout << "unexpected exception: test failed:" << std::endl;
    std::cout << e.what();
    result = false;
  }
  return result;
}

string HAL::VME64xDeviceTester::getName() const {
  return "VME64xDeviceTester";
}

string HAL::VME64xDeviceTester::getDescription() const {
  return "Performs a test of the HAL::VME64xDevice";
}
