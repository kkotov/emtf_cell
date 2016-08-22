#ifndef __MTF7_COMMON_HPP__
#define __MTF7_COMMON_HPP__

#include "swatch/core/Command.hpp"
#include "emtf/ts/cell/Mtf7Processor.hpp"
#include <string>


namespace emtf {


namespace verify {
    uint32_t CheckWrittenValue(Mtf7Processor &processor, std::string reg, const uint32_t value, swatch::core::Command::State &currentStatus);
    uint64_t CheckWrittenValue(Mtf7Processor &processor, std::string reg, const uint64_t value, swatch::core::Command::State &currentStatus);
    bool CheckWrittenValue(Mtf7Processor &processor, std::string reg, uint32_t length, const char *buffer, swatch::core::Command::State &currentStatus);
} // namespace verify


namespace config {
    const std::string log4cplusGeneralLogger();

    const std::string log4cplusLinkLogger();

    const std::string log4cplusPropertyFile();

    const std::string pcLutsPath();
} // namespace config


} // namespace emtf

#endif /* __MTF7_COMMON_HPP__ */

