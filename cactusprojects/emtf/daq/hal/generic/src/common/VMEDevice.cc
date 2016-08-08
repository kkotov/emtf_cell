#include "hal/VMEDevice.hh"

#include <string>
#include <sstream>
#include <iomanip>

HAL::VMEDevice::VMEDevice( HAL::VMEAddressTable & vmeAddressTable,
			   HAL::VMEBusAdapterInterface & vmeBusAdapter,
			   uint32_t baseaddress,
			   bool doSwapping )
  throw( HAL::BusAdapterException,
	 HAL::UnsupportedException )
  : HAL::HardwareDevice( vmeAddressTable ),
    vmeBusAdapter(vmeBusAdapter),
    baseaddresses_( NUMBER_OF_VME64XFUNCTIONS ),
    doSwapping_( doSwapping ) {
  
  // default : put the baseaddress into the first function baseaddress
  // this might be overwritten by the busadapter in case it is doing some
  // memory mapping.
  baseaddresses_[0] = baseaddress;
  vmeBusAdapter.openDevice(vmeAddressTable,
                           baseaddress,
                           &deviceIdentifierPtr,
                           &(baseaddresses_[0]),
			   doSwapping_);
}

// the constructore for VME64x modules
HAL::VMEDevice::VMEDevice( HAL::VMEAddressTable & vmeAddressTable,
			   HAL::VMEBusAdapterInterface & vmeBusAdapter,
			   std::vector<uint32_t>& baseaddresses,
			   bool doSwapping )
  throw( HAL::BusAdapterException,
	 HAL::IllegalValueException,
	 HAL::UnsupportedException )
  : HAL::HardwareDevice( vmeAddressTable ),
    vmeBusAdapter(vmeBusAdapter),
    baseaddresses_( baseaddresses ),
    doSwapping_( doSwapping ) {

  // check if we got a correct baseaddress vector, otherwise things might
  // go horribly wrong in the openDevice call which assumes that we have 
  // really space for 8 baseaddresses in the vector!  
  if ( baseaddresses_.size() != NUMBER_OF_VME64XFUNCTIONS ) {
    std::stringstream text;
    text << "baseaddress-vector does not have exactly size " << NUMBER_OF_VME64XFUNCTIONS << "!"
	 << "\n    (HAL::VMEDevice::VMEDevice)" << std::ends;
    throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }

  vmeBusAdapter.openDevice(vmeAddressTable,
                           baseaddresses,
                           &deviceIdentifierPtr,
                           &(baseaddresses_),
			   doSwapping_);
}

HAL::VMEDevice::~VMEDevice() { 
  vmeBusAdapter.closeDevice( deviceIdentifierPtr );
}

void HAL::VMEDevice::hardwareRead( const HAL::GeneralHardwareAddress& vmeAddress, 
                              uint32_t* result,
                              uint32_t offset ) const
  throw( HAL::BusAdapterException ) {

  vmeBusAdapter.read( deviceIdentifierPtr,
                      vmeAddress.getComputedAddress( baseaddresses_ ) + offset,
                      vmeAddress.getAddressModifier(),
                      vmeAddress.getDataWidth(),
                      result );
}

void HAL::VMEDevice::hardwareWrite( const HAL::GeneralHardwareAddress& vmeAddress, 
                               uint32_t data,
                               uint32_t offset ) const
  throw( HAL::BusAdapterException ) {
  
  vmeBusAdapter.write( deviceIdentifierPtr,
                       vmeAddress.getComputedAddress( baseaddresses_ ) + offset,
                       vmeAddress.getAddressModifier(),
                       vmeAddress.getDataWidth(),
                       data);
}

void HAL::VMEDevice::hardwareReadBlock( const HAL::GeneralHardwareAddress& vmeAddress, 
                                   uint32_t length,
                                   char *buffer,
                                   HalAddressIncrement addressBehaviour,
                                   uint32_t offset) const
  throw ( HAL::IllegalValueException, 
          HAL::BusAdapterException,
          HAL::UnsupportedException ) {

  uint32_t dataWidth = vmeAddress.getDataWidth();
  if ( length%dataWidth != 0 ) {
    std::string text = "the length is not a multiple of the dataWidth!\n     (HAL::VMEDevice::readBlock)";
    throw( HAL::IllegalValueException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }
  uint32_t startAddress = vmeAddress.getComputedAddress( baseaddresses_ ) + offset;
  if ( (startAddress) % dataWidth != 0 ) {
    std::stringstream text ;
    text << "the startaddress must be aligned for dataWidth " 
         << std::dec << dataWidth << ".\n"
         << "     But the start address is " 
         << std::hex << std::setw(8) << std::setfill('0') << startAddress 
         << "\n      (this is not necessarily an address which you can easily derive because\n"
         << "      it depends on the BusAdapter implementation)\n"
         << "      (HAL::VMEDevice::readBlock)" << std::ends;
    throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  vmeBusAdapter.readBlock( deviceIdentifierPtr, 
                           startAddress,
                           length,
                           vmeAddress.getAddressModifier(),
                           dataWidth,
                           buffer,
                           addressBehaviour);
}

void HAL::VMEDevice::hardwareWriteBlock( const HAL::GeneralHardwareAddress& vmeAddress, 
                                    uint32_t length,
                                    char *buffer,
                                    HalAddressIncrement addressBehaviour,
                                    uint32_t offset) const
  throw ( HAL::IllegalValueException, 
          HAL::BusAdapterException,
          HAL::UnsupportedException ) {

  uint32_t dataWidth = vmeAddress.getDataWidth();
  if ( (length) % dataWidth != 0 ) {
    std::string text = "the length is not a multiple of the dataWidth!\n     (HAL::VMEDevice::writeBlock)";
    throw( HAL::IllegalValueException( text, __FILE__, __LINE__, __FUNCTION__ ) );
  }
  uint32_t startAddress = vmeAddress.getComputedAddress( baseaddresses_ ) + offset;
  if ( (startAddress) % dataWidth != 0 ) {
    std::stringstream text;
    text << "the startaddress must be aligned for dataWidth " 
         << std::dec << dataWidth << ".\n"
         << "     But the start address is " 
         << std::hex << std::setw(8) << std::setfill('0') << startAddress 
         << "\n      (this is not necessarily an address which you can easily derive because\n"
         << "      it depends on the BusAdapter implementation)\n"
         << "      (HAL::VMEDevice::writeBlock)" << std::ends;
    throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  vmeBusAdapter.writeBlock( deviceIdentifierPtr, 
                            startAddress,
                            length,
                            vmeAddress.getAddressModifier(),
                            dataWidth,
                            buffer,
                            addressBehaviour);
}
