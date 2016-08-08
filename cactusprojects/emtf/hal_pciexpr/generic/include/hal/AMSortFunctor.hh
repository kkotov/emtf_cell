#ifndef __AMSortFunctor
#define __AMSortFunctor

namespace HAL {

class AMSortFunctor {
public:
  AMSortFunctor() {
    for ( int i = 0; i < NUM_AMs; i++ ) {
      lookupTable[ i ] = 0;
    }

    // The lookup table with the priorities:
    // The am is the index into the table; the contents of the 
    // table is the priority. Higher values mean higher priority.
    // A40 is not supported.

    // A32 accesses (at the moment D64 (MBLT) not supported)
    lookupTable[ 0x08 ] = 0;
    lookupTable[ 0x0c ] = 0;
    lookupTable[ 0x0b ] = 64; 
    lookupTable[ 0x0f ] = 63;
    lookupTable[ 0x09 ] = 62;
    lookupTable[ 0x0d ] = 61;
    lookupTable[ 0x0a ] = 60;
    lookupTable[ 0x0e ] = 59;

    // A24 accesses (at the moment D64 (MBLT) not supported)
    lookupTable[ 0x38 ] = 0;
    lookupTable[ 0x3c ] = 0;
    lookupTable[ 0x3b ] = 58;
    lookupTable[ 0x3f ] = 57;
    lookupTable[ 0x39 ] = 56;
    lookupTable[ 0x3d ] = 55;
    lookupTable[ 0x3a ] = 54;
    lookupTable[ 0x3e ] = 53;

    // A16 accesses
    lookupTable[ 0x29 ] = 52;
    lookupTable[ 0x2d ] = 51;

  };
    
  bool operator() ( uint32_t AM1, uint32_t AM2 )  {
    if ( AM1 < 64 && AM2 < 64 ) {
      return lookupTable[ AM1 ] > lookupTable[ AM2 ]; // sort in descending order
    } else if ( AM1 >= 64 ) {
      return true;
    }
    return false; 
  }
private:
  static const int NUM_AMs = 64;
  uint32_t lookupTable[NUM_AMs];
};

} /* namespace HAL */

#endif /* __AMSortFunctor */
