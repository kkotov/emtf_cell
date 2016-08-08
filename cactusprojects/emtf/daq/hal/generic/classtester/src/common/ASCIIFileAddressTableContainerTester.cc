#include "ASCIIFileAddressTableContainerTester.hh"
#include <sstream>

HAL::ASCIIFileAddressTableContainerTester::ASCIIFileAddressTableContainerTester( std::string moduleMapperFile,
									    std::string addressTableContainerFile )
  throw( HAL::IllegalValueException,
	 HAL::NoSuchFileException,
	 HAL::XMLProcessingException,
	 HAL::HardwareAccessException ) 
  : moduleMapper_(moduleMapperFile),
    addressTableContainer_(moduleMapper_,
			   addressTableContainerFile) {
}


std::string HAL::ASCIIFileAddressTableContainerTester::getDescription() const {
  std::stringstream text;
  text << "Tests the HAL::ASCIIFileAddressTableContainer.\n"
       << "You need to check the output manually!"
       << std::ends;
  return text.str();
}

bool HAL::ASCIIFileAddressTableContainerTester::execute() {
  try {
    HAL::VMEAddressTable tab1 = addressTableContainer_.getVMETableFromSerialNumber( "daqvme011" );
    tab1.print( cout );
    HAL::VMEAddressTable tab2 = addressTableContainer_.getVMETableFromSerialNumber( "daqvme021" );
    tab2.print( cout );
    HAL::VMEAddressTable tab3 = addressTableContainer_.getVMETableFromSerialNumber( "daqvme111" );
    tab3.print( cout );
    HAL::VMEAddressTable tab4 = addressTableContainer_.getVMETableFromSerialNumber( "daqvme121" );
    tab4.print( cout );
  } catch (HAL::HardwareAccessException& e) {
    std::cout << "!!! ERROR !!! UNEXPECTED EXCETPION !!!" << std::endl;
    std::cout << e.what() << std::endl;
    return false;
  }
  // all ok if arrived here
  return true;
}
