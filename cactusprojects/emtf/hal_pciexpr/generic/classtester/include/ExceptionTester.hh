#ifndef __ExceptionTester
#define __ExceptionTester

#include "IllegalOperationException.hh"
#include "TesterInterface.hh"
#include <sstream>

using std::cout;
using std::stringstream;
using std::string;
using std::ends;
using std::endl;

#define VMEADDRESSTABLE "VMEAsciiAddressTable.dat"


namespace HAL {

class ExceptionTester:public HAL::TesterInterface {
public:
  bool execute();
  string getName() const {
    return string("ExceptionTester");
  }
  string getDescription() const;
};

} /* namespace HAL */

#endif /* __ExceptionTester */






