#include "hal/CommandSequenceASCIIReader.hh"
#include <stdint.h>
#include <fstream>
#include <list>
#include <sstream>

HAL::CommandSequenceASCIIReader::CommandSequenceASCIIReader( std::string fileName )
  throw (HAL::NoSuchFileException)
  : fileName(fileName) {
 rescan();
}

HAL::CommandSequenceASCIIReader::~CommandSequenceASCIIReader( ) {
}

void HAL::CommandSequenceASCIIReader::rescan() 
  throw (HAL::NoSuchFileException) {
  // some preparation to read the file:
  const int MaxLength=1024;
  char buffer[MaxLength];
  std::ifstream FileStream( fileName.c_str() );
  std::list<std::string> commandStringList;

  commandList.clear();

  if ( ! FileStream ) {
    std::string text = "Cannot open file : " + fileName + 
      "\n    (HAL::CommandSequenceASCIIReader::CommandSequenceASCIIReader)";
    throw (HAL::NoSuchFileException( text, __FILE__, __LINE__, __FUNCTION__ ));
  }

  // loop over all lines of the file:
  
  while ( ! FileStream.eof() ) {
    FileStream.getline(buffer, MaxLength);
    if (buffer[0] == '#') continue;
    if ((uint32_t)buffer[0] == 0xd) continue; // in case of some windows edited ascii files....
    
    std::istringstream Line(buffer);
    std::string word;
    commandStringList.clear();
    // this construction should allow empty lines...
    word = "";
    Line >> word;
    if ( word != "" ) {
      commandStringList.push_back( word );
      while ( ! Line.eof() ) {
		word = "";
		Line >> word ;
		if ( word != "" ) {
		  commandStringList.push_back( word );
		}
      }
      commandList.push_back( commandStringList );
    }
  }
  FileStream.close();
  firstAccess = true;
}

