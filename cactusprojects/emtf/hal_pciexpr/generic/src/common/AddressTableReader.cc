#include "hal/AddressTableReader.hh"

HAL::AddressTableReader::AddressTableReader()
  : firstAccess( true ) {
}

HAL::AddressTableReader::~AddressTableReader() {
  // must delete everything which has not been yet "read out"
  // via the next method:
  if ( firstAccess ) it = itemPointerList.begin();
  while ( it != itemPointerList.end() ) {
    delete (*it);
    it++;
  }
}

bool HAL::AddressTableReader::next( HAL::AddressTableItem** nextItem ) {
  if ( firstAccess )  {
    it = itemPointerList.begin();;
    firstAccess = false;
  }
  if ( it != itemPointerList.end() ) {
    *nextItem = *it;
    it++;
    return true;
  } else {
    return false;
  }
}
