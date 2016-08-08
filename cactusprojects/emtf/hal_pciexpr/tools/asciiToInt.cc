#include <iostream>
#include <string>
#include <iomanip>

using std::cout;
using std::hex;
using std::setw;
using std::setfill;
using std::endl;
using std::string;

int main( int argi, char* argv[] ) {
  if (argi != 2 ) {
    cout << "Converts a string to a sequence of hex numbers." << endl;
    cout << "Can be used to convert serial numbers into a mif-file compatible format." << endl;
    cout << "usage : asciiToInt {\"string\"}" << endl;
    exit(-1);
  }
  string in(argv[1]);
  cout << in << endl;
  char * ptr = argv[1];
  int ic=0;
  while ( ptr[ic] != 0  ) {
    uint32_t  i = (int)ptr[ic];
    i <<= 24;
    cout << hex << setw(8) << setfill('0') << i << " ";
    ic++;
  }

  cout << endl;

}
