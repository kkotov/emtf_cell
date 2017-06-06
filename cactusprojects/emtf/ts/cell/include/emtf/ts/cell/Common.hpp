#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#include "swatch/action/Command.hpp"
#include "emtf/ts/cell/EmtfProcessor.hpp"
#include <string>


namespace emtf {


namespace verify {
    uint32_t CheckWrittenValue(EmtfProcessor &processor, std::string reg, const uint32_t value, swatch::action::Command::State &currentStatus);
    uint64_t CheckWrittenValue(EmtfProcessor &processor, std::string reg, const uint64_t value, swatch::action::Command::State &currentStatus);
    bool CheckWrittenValue(EmtfProcessor &processor, std::string reg, uint32_t length, const char *buffer, swatch::action::Command::State &currentStatus);
} // namespace verify


namespace config {
    const std::string log4cplusGeneralLogger();
    const std::string log4cplusLinkLogger();
    const std::string log4cplusRateLogger();
    const std::string log4cplusLctLogger();
    const std::string log4cplusPropertyFile();
    const std::string pcLutsPath();
    const std::string ptLutPath();
    const std::string rwDelaysPath();
    const uint16_t brokenLinksWarningProcessor();
    const uint16_t brokenLinksErrorProcessor();
    const uint16_t brokenLinksWarningSystem();
    const uint16_t brokenLinksErrorSystem();
    const std::string alignmentFifoDelayFile(int endcap, int sector);
    const uint32_t cscLinksCount();
} // namespace config


} // namespace emtf

#endif /* __COMMON_HPP__ */

