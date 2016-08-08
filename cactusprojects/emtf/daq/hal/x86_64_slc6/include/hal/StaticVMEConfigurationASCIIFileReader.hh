#ifndef __StaticVMEConfigurationASCIIFileReader
#define __StaticVMEConfigurationASCIIFileReader

#include <string>

#include "hal/NoSuchFileException.hh"
#include "hal/StaticVMEConfigurationReader.hh"

namespace HAL {

class  StaticVMEConfigurationASCIIFileReader : public StaticVMEConfigurationReader {
public:
  StaticVMEConfigurationASCIIFileReader(std::string fileName) 
    throw (NoSuchFileException);
};

} /* namespace HAL */

#endif
