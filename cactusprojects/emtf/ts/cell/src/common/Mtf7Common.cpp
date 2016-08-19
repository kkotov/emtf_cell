#include "emtf/ts/cell/Mtf7Processor.hpp"
#include "emtf/ts/cell/Mtf7Common.hpp"

using namespace std;
using namespace swatch;
using namespace core;


namespace emtf {

uint32_t CheckWrittenValue(Mtf7Processor &processor, string reg, const uint32_t value, Command::State &currentStatus)
{
    uint32_t valueRead;
    processor.read(reg, valueRead);

    if(value != valueRead)
    {
        currentStatus = ActionSnapshot::kError;
    }

    return valueRead;
}

uint64_t CheckWrittenValue(Mtf7Processor &processor, string reg, const uint64_t value, Command::State &currentStatus)
{
    uint64_t valueRead;
    processor.read64(reg, valueRead);

    if(value != valueRead)
    {
        currentStatus = ActionSnapshot::kError;
    }

    return valueRead;
}

bool CheckWrittenValue(Mtf7Processor &processor, string reg, uint32_t length, const char *buffer, Command::State &currentStatus)
{
    char bufferRead[length];
    processor.readBlock(reg, length, bufferRead);

    bool success = (0 == memcmp(buffer, bufferRead, length));

    if(!success)
    {
        currentStatus = ActionSnapshot::kError;
    }

    return success;
}


namespace config {

    const string log4cplusPropertyFile()
    {
        return getenv("EMTF_LOGGER_PROPERTY_FILE") ? getenv("EMTF_LOGGER_PROPERTY_FILE") : "/home/emtfts/emtf_cell_config/log4cplus.properties";
    }

} // namespace config


} // namespace

