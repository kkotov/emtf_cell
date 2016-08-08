#include "hal/AddressTable.hh"
#include <sstream>

HAL::AddressTable::AddressTable( std::string name )
  : name(name) {
}

HAL::AddressTable::AddressTable( const HAL::AddressTable& original ) {  
  std::tr1::unordered_map< std::string, HAL::AddressTableItem*, HAL::HalHash<std::string> >::const_iterator it;
  for ( it = original.itemMap.begin(); it != original.itemMap.end(); it++ ) {
    std::string key = (*it).first;
    HAL::AddressTableItem* itemPtr = (*it).second;
    HAL::AddressTableItem* clonePtr = new HAL::AddressTableItem( *itemPtr );
    this->itemMap[key] = clonePtr;
  }
}

HAL::AddressTable::~AddressTable() {
  std::tr1::unordered_map< std::string, HAL::AddressTableItem*, HAL::HalHash<std::string> >::iterator it;
  for ( it = itemMap.begin(); it != itemMap.end(); it++ ) {
    delete (*it).second;
  }
}

const HAL::GeneralHardwareAddress& 
HAL::AddressTable::getGeneralHardwareAddress( std::string item ) const
  throw( HAL::NoSuchItemException ){
  return checkItem( item ).getGeneralHardwareAddress();
}

const HAL::GeneralHardwareAddress& 
HAL::AddressTable::getWritableHardwareAddress( std::string item ) const
  throw (HAL::NoSuchItemException, HAL::IllegalOperationException) {
  const HAL::AddressTableItem& addressItem = 
    checkItem( item );
  if (! addressItem.isWritable() ) {
    std::stringstream text;
    text << "Try to write to non writable item!"
         << "\n    TableName : " << getName() 
         << "\n    Item : " << item
         << "\n    (HAL::HardwareDevice::getWritableHardwareAddress)" << std::ends;
    std::string textStr = text.str();
    throw (HAL::IllegalOperationException( textStr, __FILE__, __LINE__, __FUNCTION__ ));
  }
  return addressItem.getGeneralHardwareAddress();
}

const HAL::GeneralHardwareAddress& 
HAL::AddressTable::getReadableHardwareAddress( std::string item ) const
  throw (HAL::NoSuchItemException, HAL::IllegalOperationException) {
  const HAL::AddressTableItem& addressItem = 
    checkItem( item );
  if (! addressItem.isReadable() ) {
    std::stringstream text;
    text << "Try to read from non readable item!"
         << "\n    TableName : " << getName() 
         << "\n    Item : " << item
         << "\n    (HAL::HardwareDevice::getReadableHardwareAddress)" << std::ends;
    std::string textStr = text.str();
    throw (HAL::IllegalOperationException( textStr, __FILE__, __LINE__, __FUNCTION__ ));
  }
  return addressItem.getGeneralHardwareAddress();
}

std::string HAL::AddressTable::getDescription( std::string item ) const
  throw (HAL::NoSuchItemException) {
  return checkItem( item ).getDescription();
}

bool HAL::AddressTable::isReadable( std::string item ) const
  throw (HAL::NoSuchItemException) {
  return checkItem( item ).isReadable();
}

bool HAL::AddressTable::isWritable( std::string item ) const
  throw( HAL::NoSuchItemException ) {
  return checkItem( item ).isWritable();
}

uint32_t HAL::AddressTable::getMask( std::string item ) const 
  throw( HAL::NoSuchItemException ) {
  return checkItem( item ).getMask();
}

bool HAL::AddressTable::isBit( std::string item ) const
  throw( HAL::NoSuchItemException ) {
  return checkItem( item ).isBit();
}

bool HAL::AddressTable::exists( std::string item ) const {
  if ( itemMap.count(item) != 0 ) {
    return true;
  } else {
    return false;
  }
}

const HAL::AddressTableItem&  HAL::AddressTable::checkItem( std::string item ) const 
  throw( HAL::NoSuchItemException ) {
  std::tr1::unordered_map< std::string, HAL::AddressTableItem*, HAL::HalHash<std::string> >::const_iterator it;
  if ( (it = itemMap.find(item)) == itemMap.end() ) {
    std::string text = "Item \"" + item + "\" not found.\n    (HAL::AddressTable::checkItem)";
    throw (HAL::NoSuchItemException(text, __FILE__, __LINE__, __FUNCTION__ )) ;
  }
  return(*((*it).second) );
}

std::tr1::unordered_map<std::string, HAL::AddressTableItem*, HAL::HalHash<std::string> >::const_iterator
HAL::AddressTable::getItemListBegin() const {
  std::tr1::unordered_map< std::string, HAL::AddressTableItem*, HAL::HalHash<std::string> >::const_iterator it;
  it = itemMap.begin();
  return it;
}

std::tr1::unordered_map<std::string, HAL::AddressTableItem*, HAL::HalHash<std::string> >::const_iterator
HAL::AddressTable::getItemListEnd() const {
  std::tr1::unordered_map< std::string, HAL::AddressTableItem*, HAL::HalHash<std::string> >::const_iterator it;
  it = itemMap.end();
  return it;
}

uint32_t HAL::AddressTable::applyToMask( std::string item, 
					      uint32_t data,
					      bool MaskBoundaryCheck ) const 
  throw( HAL::NoSuchItemException, HAL::MaskBoundaryException ) {
  return checkItem( item ).applyToMask( data, MaskBoundaryCheck );
}

uint32_t HAL::AddressTable::applyFromMask( std::string item, 
						uint32_t data,
						bool MaskBoundaryCheck ) const 
  throw( HAL::NoSuchItemException, HAL::MaskBoundaryException ) {
  return checkItem( item ).applyFromMask( data, MaskBoundaryCheck );
}
