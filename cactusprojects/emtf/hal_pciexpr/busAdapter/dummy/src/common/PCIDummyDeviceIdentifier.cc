#include <sstream>
#include "hal/PCIDummyDeviceIdentifier.hh"

HAL::PCIDummyDeviceIdentifier::PCIDummyDeviceIdentifier( uint32_t deviceNumber,
							 std::vector<char *>&memorySpacePtr,
							 char *configSpacePtr,
							 std::vector<uint32_t> baseaddresses,
							 std::vector<uint32_t> mapsizes,
							 bool swapFlag) 
  : deviceNumber( deviceNumber ),
    memorySpacePtr_( memorySpacePtr ),
    configSpacePtr_( configSpacePtr ),
    baseaddresses_(baseaddresses),
    mapsizes_(mapsizes),
    swapFlag_( swapFlag ) {
}

HAL::PCIDummyDeviceIdentifier::~PCIDummyDeviceIdentifier() {

  // in principle we do not need the if since it is legal to delete a null 
  // pointer; but it is more instructive to code it like this:

  for ( int ic=0; ic<6; ic++ ) {
    if ( memorySpacePtr_[ic] != (char*)0 ) 
      delete memorySpacePtr_[ic];
  }
  if (configSpacePtr_ != (char*)0) {
    delete configSpacePtr_;
  }
}

char* HAL::PCIDummyDeviceIdentifier::getConfigSpacePtr() const {
  return configSpacePtr_;
}

char* HAL::PCIDummyDeviceIdentifier::remap( uint32_t address ) const
  throw( BusAdapterException ) {

  int ibar;
  bool found = false;

  for( ibar = 0; ibar < 6; ibar++ ) {

    if ( (address >= baseaddresses_[ibar] ) && ( address < (baseaddresses_[ibar] + mapsizes_[ibar]) ) ) {
      found = true;
      break;
    }
  }

  if ( ! found ) {
    std::stringstream text;
    text << "Could not find a mapped bar for the requested address. Software bug in HAL.\n" << std::ends;
    throw( BusAdapterException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }

  char* result = ((address - baseaddresses_[ibar])) + memorySpacePtr_[ibar];
  return result;
}

std::string HAL::PCIDummyDeviceIdentifier::printString() const {
  char buf[256];
  sprintf( buf, "PCIDummyDeviceIdentifier : %u ", deviceNumber );

  return std::string(buf) ;
}

bool HAL::PCIDummyDeviceIdentifier::doSwap() const {
  return swapFlag_;
}


uint32_t HAL::PCIDummyDeviceIdentifier::getPCIBusAddress( uint32_t bar ) const 
  throw( ) {
  return (uint32_t) baseaddresses_[ bar ];
};
