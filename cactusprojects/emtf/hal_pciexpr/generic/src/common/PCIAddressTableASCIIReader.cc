#include "hal/PCIAddressTableASCIIReader.hh"

#include <sstream>
#include <fstream>
#include <iomanip>

HAL::PCIAddressTableASCIIReader::PCIAddressTableASCIIReader( std::string fileName )
  throw (HAL::NoSuchFileException, 
	 HAL::IllegalValueException) 
  : HAL::AddressTableReader() {

  // some preparation to read the file:
  const int MaxLength=1024;
  char buffer[MaxLength];
  std::ifstream FileStream( fileName.c_str() );
  if ( ! FileStream ) {
    std::string text = "Cannot open file : " + fileName + 
      " (HAL::PCIAddressTableASCIIReader::PCIAddressTableASCIIReader)";
    throw (HAL::NoSuchFileException( text, __FILE__, __LINE__, __FUNCTION__ ));
  }

  // prepare the loop over all lines of the ascii file:
  std::string key, pciSpaceString, description;
  uint32_t address, barId, mask, isReadable, isWritable;
  enum AddressSpace pciSpace;
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
    Line >> pciSpaceString;
    if (pciSpaceString == "memory") {
      pciSpace = (AddressSpace)MEMORY;
      Line >> std::hex >> barId;
    } else if ( pciSpaceString == "configuration" ) {
      pciSpace = (AddressSpace)CONFIGURATION;
      barId = 999;
    } else {
      std::string text =  "Illegal Access Mode : " + pciSpaceString
        + "\n    Must be \"configure\" or \"memory\"\n    (HAL::PCIAddressTableASCIIReader::PCIAddressTableASCIIReader)";
      throw( HAL::IllegalValueException( text, __FILE__, __LINE__, __FUNCTION__ ) );
    }
    Line >> std::hex >> address;
    Line >> std::hex >> mask;
    Line >> isReadable;
    Line >> isWritable;
    Line >> std::ws;
    Line.getline(charbuf, MaxLength);
    description = std::string(charbuf);
    
    HAL::PCIHardwareAddress* addressPointer = new HAL::PCIHardwareAddress(address, pciSpace, barId);
    HAL::AddressTableItem* itemPointer = 
	  new HAL::AddressTableItem(key, description, *addressPointer,
				    mask, isWritable, isReadable);
    itemPointerList.push_back(itemPointer);
  }
  FileStream.close();
}
