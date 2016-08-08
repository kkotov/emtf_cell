#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

int main(  int argi, char* argv[] ) {
  if ( argi != 3 ) {
    cout << "Takes a mif file containing the CR/CSR space of a VME64x module" << endl;
    cout << "and calculates the checksum to be put as the first byte in the CR space." << endl;
    cout << "The mif file must be in the same format as the files given to the" << endl;
    cout << "ConfigurationSpaceFileReader (see examples and documentation)." << endl;
    cout << "" << endl;
    cout << endl;
    cout << "usage : checksum {fileName} {endaddress (hex)}" << endl;
    cout << "" << endl;
    cout << "The endaddress has to be an address which is larger or equal than" << endl;
    cout << "the last byte of the CR space which is included in the checksum calculation." << endl;
    cout << "The user defined items (like for example the serial number) must be included" << endl;
    cout << "in the checksum calculation. The CSR space of cours MUST NOT be included" << endl;
    cout << "in the calculation." << endl;
    cout << "" << endl;
    exit(-1);
  }
  string fileName( argv[1] );
  istringstream endadrs( argv[2] );

  uint32_t endadr;
  endadrs >> hex >> endadr;
  cout << "about to read " << fileName << " until address " << hex << endadr << endl;
  
  // some preparation to read the file:

  const int MaxLength=1024;
  char buffer[MaxLength];
  ifstream FileStream( fileName.c_str() );
  if ( ! FileStream ) {
    string text = "Cannot open file : " + fileName ;
    cout << text << endl;
    return -1;
  }

  // prepare the loop over all lines of the ascii file:
  string dummy;
  uint32_t address, data;
  uint32_t checksum = 0;
  address = 0;
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
    if (buffer[ic] == '#') continue; // a comment
    if (buffer[ic] == '\n') continue; // should never happen since \n is not extracted with getline
    if (buffer[ic] == 0x00) continue;

    istringstream Line(buffer);    
    Line >> hex >> address ;
    if ( address > endadr ) break;
    Line >> dummy;
    
    
    if ( address%4 != 0 ) {
      stringstream text;
      text << "Addresses must be 32 bit aligned! (found " << hex << address 
           << ")" << endl;
      cout << text.str() << endl;
      return -1;
    }
    while ( Line >> hex >> data ) {
      if ( address > 0 ) {
        checksum =  checksum +
          ((0xff000000 & data) >> 24) + 
          ((0x00ff0000 & data) >> 16) +
          ((0x0000ff00 & data) >> 8) +
          (0x000000ff & data);
      }
      address += 4;
    }
  }
  //hex << checksum << endl;
  checksum = (~(0xff & checksum) + 1) & 0xff;
  cout << endl;
  cout << "checksum (HEX) : " <<  hex << checksum << endl;
  FileStream.close();
  cout << endl;
  return 0;
}
