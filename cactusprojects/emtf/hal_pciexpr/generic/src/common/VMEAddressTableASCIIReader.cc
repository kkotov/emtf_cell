#include "hal/VMEAddressTableASCIIReader.hh"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <strings.h>

HAL::VMEAddressTableASCIIReader::VMEAddressTableASCIIReader( std::string fileName ) 
  throw (HAL::NoSuchFileException) 
  : HAL::AddressTableReader() {
  // some preparation to read the file:
  const int MaxLength=1024;
  char buffer[MaxLength];
  std::ifstream FileStream( fileName.c_str() );
  if ( ! FileStream ) {
    std::string text = "Cannot open file : " + fileName + "    (HAL::VMEAddressTableASCIIReader::VMEAddressTableASCIIReader)";
    throw (HAL::NoSuchFileException( text, __FILE__, __LINE__, __FUNCTION__ ));
  }
  // prepare the loop over all lines of the ascii file:
  std::string key, description, addressSpaceStr;
  uint32_t address, addressModifier, dataWidth, mask, isReadable, isWritable, windowId;
  HAL::AddressSpace addressSpace = MEMORY;
  char charbuf[MaxLength];
  bool first = true;
  bool isVme64x = false;
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

    // first item: find out if we deal with vme64x or standard vme
    if ( first ) {
      first = false;
      std::string nextWord;
      Line >> nextWord;
      if ( nextWord == "memory" ||
	   nextWord == "configuration" ||
	   nextWord == "io" ) {
	isVme64x = true;
	addressSpaceStr = nextWord;
	addressSpace = convertAddressSpaceStr( addressSpaceStr );
     } else {
	isVme64x = false;
	std::istringstream tmp( nextWord );
	tmp >> std::hex >> addressModifier;
      }
    } else {
      if ( isVme64x ) {
	Line >> addressSpaceStr;
	addressSpace = convertAddressSpaceStr( addressSpaceStr );
      } else {
	Line >> std::hex >> addressModifier;
      }
    }
    
    if ( isVme64x ) {
      if ( addressSpace == MEMORY ) {
	Line >> std::dec >> windowId;
	dataWidth = 0;
      } else if ( addressSpace == CONFIGURATION ) {
	Line >> std::dec >> dataWidth;
      }
    } else {
      Line >> std::dec >> dataWidth;
    }

    Line >> std::hex >> address;
    Line >> std::hex >> mask;
    Line >> isReadable;
    Line >> isWritable;
    Line >> std::ws;
    Line.getline(charbuf, MaxLength);
    description = std::string(charbuf);
    // it will be destroyed in the desructor of the AddresstableItem

    // load the list :
    HAL::GeneralHardwareAddress* addressPointer;
    if ( isVme64x ) {
      addressPointer = new HAL::VME64xHardwareAddress(address, addressSpace, windowId, dataWidth);
    } else {
      addressPointer = new HAL::VMEHardwareAddress(address, 
					      addressModifier, 
					      dataWidth);
    }

    HAL::AddressTableItem* itemPointer = 
      new HAL::AddressTableItem(key, description, *addressPointer, 
                           mask, isWritable, isReadable);
    itemPointerList.push_back(itemPointer);
  }
  FileStream.close();
}

HAL::AddressSpace
HAL::VMEAddressTableASCIIReader::convertAddressSpaceStr( std::string addressSpaceStr ) 
throw( HAL::IllegalValueException ) {

  AddressSpace addressSpace;

  if ( strcasecmp( addressSpaceStr.c_str(), "memory" ) == 0 )  {
    addressSpace = MEMORY;
  } else if ( strcasecmp( addressSpaceStr.c_str(), "configuration") == 0 ) {
    addressSpace = CONFIGURATION;
  } else {
    std::string text = "No such address-space in VME64x : " + addressSpaceStr + 
      "\npossible spaces are 'memory' and 'configuration'." +
      "\n    (HAL::VME64xAddressTableASCIIReader::HAL::VME64xAddressTableASCIIReader)";
    throw (HAL::IllegalValueException( text, __FILE__, __LINE__, __FUNCTION__ )); 
  }
  return  addressSpace;
}
