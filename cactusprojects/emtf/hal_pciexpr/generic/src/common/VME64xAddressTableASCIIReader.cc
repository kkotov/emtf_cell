#include "hal/VME64xAddressTableASCIIReader.hh"
#include <fstream>
#include <iomanip>
#include <sstream>

HAL::VME64xAddressTableASCIIReader::VME64xAddressTableASCIIReader( std::string fileName ) 
  throw (HAL::NoSuchFileException,
	 HAL::IllegalValueException) 
  : HAL::AddressTableReader() {

  // some preparation to read the file:
  const int MaxLength=1024;
  char buffer[MaxLength];
  std::ifstream FileStream( fileName.c_str() );
  if ( ! FileStream ) {
    std::string text = "Cannot open file : " + fileName + "    (HAL::VME64xAddressTableASCIIReader::VME64xAddressTableASCIIReader)";
    throw (HAL::NoSuchFileException( text, __FILE__, __LINE__, __FUNCTION__ ));
  }
  // prepare the loop over all lines of the ascii file:
  std::string key, addressSpaceStr, description;
  AddressSpace addressSpace;
  uint32_t address, window_width, mask, isReadable, isWritable;
  char charbuf[MaxLength];
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
    if ((uint32_t)buffer[ic] == 0xd) continue; // in case of some windows edited ascii files....
    if (buffer[ic] == 0x00) continue;
    
    std::istringstream Line(buffer);    
    Line >> key ;
    Line >> addressSpaceStr;
    Line >> std::dec >> window_width,
    Line >> std::hex >> address;
    Line >> std::hex >> mask;
    Line >> isReadable;
    Line >> isWritable;
    Line >> std::ws;
    Line.getline(charbuf, MaxLength);
    description = std::string(charbuf);
    if ( strcasecmp( addressSpaceStr.c_str(), "memory" ) == 0 )  {
      addressSpace = MEMORY;
    } else if ( strcasecmp( addressSpaceStr.c_str(), "configuration") == 0 ) {
      addressSpace = CONFIGURATION;
    } else {
      std::string text = "No such address-space in VME64x : " + addressSpaceStr + 
	"\npossible spaces are 'memory' and 'configuration'." +
	"\n    (HAL::VME64xAddressTableASCIIReader::VME64xAddressTableASCIIReader)";
      throw (HAL::IllegalValueException( text, __FILE__, __LINE__, __FUNCTION__ )); 
    }
    // it will be destroyed in the desructor of the AddresstableItem
    HAL::VME64xHardwareAddress* addressPointer = new HAL::VME64xHardwareAddress(address, addressSpace, 
										window_width, window_width);
    HAL::AddressTableItem* itemPointer = 
      new HAL::AddressTableItem(key, description, *addressPointer, 
                           mask, isWritable, isReadable);
    itemPointerList.push_back(itemPointer);
  }
  FileStream.close();
}
