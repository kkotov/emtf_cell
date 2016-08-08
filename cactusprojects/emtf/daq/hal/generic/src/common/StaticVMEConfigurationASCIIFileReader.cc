#include "hal/StaticVMEConfigurationASCIIFileReader.hh"
#include <fstream>
#include <sstream>
#include <iomanip>

HAL::StaticVMEConfigurationASCIIFileReader::StaticVMEConfigurationASCIIFileReader( std::string fileName )
  throw (HAL::NoSuchFileException) 
  : HAL::StaticVMEConfigurationReader() {

  // some preparation to read the file:

  const int MaxLength=1024;
  char buffer[MaxLength];
  std::ifstream FileStream( fileName.c_str() );
  if ( ! FileStream ) {
    std::string text = "Cannot open file : " + fileName + 
      "    (HAL::StaticVMEConfigurationASCIIFileReader::StaticVMEConfigurationASCIIFileReader)";
    throw (HAL::NoSuchFileException( text, __FILE__, __LINE__, __FUNCTION__ ));
  }


  // prepare the loop over all lines of the ascii file:

  std::string serialNumber;
  uint32_t slotId;

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

    // skip comments or empty lines
    if (buffer[ic] == '*') continue;
    if (buffer[ic] == '\n') continue; // should never happen since \n is not extracted with getline
    if ((uint32_t)buffer[ic] == 0xd) continue; // in case of some windows edited ascii files....
    if (buffer[ic] == 0x00) continue;
    
    // scan a line
    std::istringstream Line(buffer);    
    Line >> serialNumber;
    Line >> std::dec >> slotId;

    // it will be destroyed in the desructor of the HAL::VME64xCrate
    HAL::StaticVMEItem* itemPointer = new HAL::StaticVMEItem( serialNumber, slotId ); 
    staticVMEItemList_.push_back(itemPointer);

  }

  FileStream.close();

}
