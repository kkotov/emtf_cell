#include <sstream>
#include <fstream>
#include <iomanip>

#include "hal/ASCIIFileModuleMapper.hh"

HAL::ASCIIFileModuleMapper::ASCIIFileModuleMapper( std::string mapFile )
  throw( HAL::NoSuchFileException ) {
  // some preparation to read the file:
  const int MaxLength=1024;
  char buffer[MaxLength];
  std::ifstream FileStream( mapFile.c_str() );
  if ( ! FileStream ) {
    std::string text = "Cannot open file : " + mapFile + "    (HAL::ASCIIFileModuleMapper::ASCIIFileModuleMapper)";
    throw (HAL::NoSuchFileException( text, __FILE__, __LINE__, __FUNCTION__ ));
  }
  // prepare the loop over all lines of the ascii file:
  std::string serialNumber, typeId;
  uint32_t baseaddress;
  // loop over all lines of the file:
  while ( ! FileStream.eof() ) {
    FileStream.getline(buffer, MaxLength);
    if ( FileStream.eof() ) break;
    //skip all white space at beginning of buffer:
    int ic = 0;
    while ( buffer[ic] == ' ' ||
            buffer[ic] == '\t' ) {
      ic++;
    }
    if (buffer[ic] == '*') continue;
    if (buffer[ic] == '\n') continue; // should never happen since \n is not extracted with getline
    if (buffer[ic] == 0x00) continue;
    
    std::istringstream Line(buffer);    
    Line >> serialNumber ;
    Line >> std::ws >> typeId;
    Line >> std::ws >> std::hex >> baseaddress;
    typeIdMap_[serialNumber] = typeId;
    baseaddressMap_[serialNumber] = baseaddress;
  }
  FileStream.close();
}

std::string HAL::ASCIIFileModuleMapper::getTypeId( std::string serialNumber ) 
  throw( HAL::IllegalValueException ) {
  if ( typeIdMap_.find( serialNumber ) == typeIdMap_.end() ) {
    std::stringstream text;
    text << "There is no typeId mapped for the serialNumber " 
	 << serialNumber 
	 << "\n    (HAL::ASCIIFileModuleMapper::getTypeId)" << std::ends;
    throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  } 
  return typeIdMap_[serialNumber];
}


uint32_t HAL::ASCIIFileModuleMapper::getBaseaddress( std::string serialNumber ) 
  throw( HAL::IllegalValueException ) {
  if ( baseaddressMap_.find( serialNumber ) == baseaddressMap_.end()) {
    std::stringstream text;
    text << "There is no baseaddress mapped for the serialNumber " 
	 << serialNumber 
	 << "\n    (HAL::ASCIIFileModuleMapper::getBaseaddress)" << std::ends;
    throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  } 
  return baseaddressMap_[serialNumber];

}
