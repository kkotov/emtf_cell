#include "hal/ConfigurationSpaceFileReader.hh"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

HAL::ConfigurationSpaceFileReader::~ConfigurationSpaceFileReader() {
  if ( ! gaveDataAway_ ) {
    delete dataMapPtr_;
  }
}

HAL::ConfigurationSpaceFileReader::ConfigurationSpaceFileReader( std::string fileName ) 
  throw (NoSuchFileException,
         IllegalValueException)  {
  //  cout << "about to read " << fileName << std::endl;
  // some preparation to read the file:
  gaveDataAway_ = false;
  dataMapPtr_ = new std::tr1::unordered_map<uint32_t, uint32_t>;
  const int MaxLength=1024;
  char buffer[MaxLength];
  std::ifstream FileStream( fileName.c_str() );
  if ( ! FileStream ) {
    std::string text = "Cannot open file : " + fileName + "    (HAL::ConfigurationSpaceFileReader::ConfigurationSpaceFileReader)";
    throw (NoSuchFileException( text, __FILE__, __LINE__, __FUNCTION__ ));
  }

  // prepare the loop over all lines of the ascii file:
  std::string dummy;
  uint32_t address, data;
  // loop over all lines of the file:
  while ( ! FileStream.eof() ) {
    FileStream.getline(buffer, MaxLength);
    if ( FileStream.eof() ) break;
    // skip all white space at beginning of buffer:
    int ic = 0;
    while ( buffer[ic] == ' ' ||
            buffer[ic] == '\t' ) {
      ic++;
    }
    if (buffer[ic] == '#') continue; // a comment
    if (buffer[ic] == '\n') continue; // should never happen since \n is not extracted with getline
    if (buffer[ic] == 0x00) continue;

    std::istringstream Line(buffer);    
    Line >> std::hex >> address ;
    Line >> dummy;
    
    if ( address%4 != 0 ) {
      std::stringstream text;
      text << "Addresses must be 32 bit aligned! (found " << std::hex << address 
           << ")\n    (HAL::ConfigurationSpaceFileReader::ConfigurationSpaceFileReader)" << std::endl;
      throw( IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
    }
    
    while ( Line >> std::hex >> data ) {
      (*dataMapPtr_)[address] = data;
      address += 4;
    }
  }
  FileStream.close();
}

std::tr1::unordered_map< uint32_t, uint32_t>* HAL::ConfigurationSpaceFileReader::getDataMapPtr() {
  gaveDataAway_ = true;
  return dataMapPtr_; 
}
