#include "hal/VMEDummyDeviceIdentifier.hh"
#include <sstream>

HAL::VMEDummyDeviceIdentifier::VMEDummyDeviceIdentifier( uint32_t deviceNumber,
							 std::vector<char *> memoryRegionPtr,
							 std::vector<uint32_t> baseaddresses,
							 std::vector<uint32_t> mapsizes,
							 bool doSwapping )
    
  : deviceNumber_(deviceNumber),
    memoryRegionPtr_(memoryRegionPtr),
    baseaddresses_( baseaddresses),
    mapsizes_( mapsizes ),
    doSwapping_( doSwapping ) {

  
}

HAL::VMEDummyDeviceIdentifier::~VMEDummyDeviceIdentifier() {
  // in principle we do not need the if since it is legal to delete a null 
  // pointer; but it is more instructive to code it like this:

  for ( int ic=0; ic<=NUMBER_OF_VME64XFUNCTIONS; ic++ ) {

    if ( memoryRegionPtr_[ic] != (char *) 0 ) {
      delete [] memoryRegionPtr_[ic];
      memoryRegionPtr_[ic] = (char *) 0;
    }

  }
}

// to be done here: find out in which ifunc we are
//                  subtract the baseaddress offset
//                  add the offset to the memory region in pc
//                  return the resulting pointer for read or write operation
char* HAL::VMEDummyDeviceIdentifier::remap( uint32_t address ) const 
  throw( BusAdapterException) {

  int ifunc;
  bool found = false;
  for ( ifunc=0; ifunc<NUMBER_OF_VME64XFUNCTIONS; ifunc++ ) {

    //std::cout << "ifunc " << ifunc 
    //	      << " address " << std::hex << address
    //	      << " baseadr " << baseaddresses_[ifunc]
    //	      << " mapsizes " << mapsizes_[ifunc]
    //	      << std::endl;

    if ( ( address >= baseaddresses_[ifunc] ) && ( address < (baseaddresses_[ifunc] + mapsizes_[ifunc]) ) ) {
      found = true;
      break;
    }
  }

  if ( ! found ) {    
    std::stringstream text;
    text << "Could not find a mapped ifunc for the requested address. Software bug in HAL.\n" << std::ends;
    throw( BusAdapterException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }

  char* result = ((address - baseaddresses_[ifunc])) + memoryRegionPtr_[ifunc];

  return result;

}


std::string HAL::VMEDummyDeviceIdentifier::printString() const {
  char buf[256];
  sprintf( buf, "VMEDummyDeviceIdentifier : %u ", deviceNumber_ ); 
  return std::string(buf) ;
}

bool HAL::VMEDummyDeviceIdentifier::doSwapping() const {
    return doSwapping_;
}
