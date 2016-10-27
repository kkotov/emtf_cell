#ifndef __MTF7_PROCESSOR_HPP__
#define __MTF7_PROCESSOR_HPP__

#include "swatch/processor/Processor.hpp"
#include "hal/PCIAddressTableASCIIReader.hh"
#include "hal/PCIAddressTable.hh"
#include "emtf/pciExprLinuxBusAdapter/PCIExprAddressTable.hh"
#include "emtf/pciExprLinuxBusAdapter/PCIExprLinuxBusAdapter.hh"
#include "emtf/pciExprLinuxBusAdapter/PCIExprDevice.hh"
#include "boost/lexical_cast.hpp"
#include <string>
#include <vector>
#include <utility>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

namespace emtf {

class Mtf7Processor : public swatch::processor::Processor
{
public:
    Mtf7Processor(const swatch::core::AbstractStub& aStub);
    ~Mtf7Processor();

    void retrieveMetricValues();

    void read(std::string reg, uint32_t &value)
    {
        driver->read(reg, &value);
    }

    void write(std::string reg, uint32_t value)
    {
        driver->write(reg, value);
    }

    void read64(std::string reg, uint64_t &value)
    {
        driver->read64(reg, &value);
    }

    void write64(std::string reg, uint64_t value)
    {
        driver->write64(reg, value);
    }

    void readBlock(std::string reg, uint32_t length, char *buffer)
    {
        driver->readBlock(reg, length, buffer);
    }

    void writeBlock(std::string reg, uint32_t length, const char *buffer)
    {
        driver->writeBlock(reg, length, const_cast<char *>(buffer));
    }

    void readBlock64(std::string reg, uint32_t length, char *buffer, uint64_t offset=0)
    {
        driver->readBlock64(reg, length, buffer, HAL::HAL_DO_INCREMENT, offset);
    }

    void writeBlock64(std::string reg, uint32_t length, char *buffer, uint64_t offset=0)
    {
        driver->writeBlock64(reg, length, buffer, HAL::HAL_NO_VERIFY, HAL::HAL_DO_INCREMENT, offset);
    }

    void read64reg(std::string reg, uint64_t &value)
    {
        union {
            uint64_t val;
            char data[];
        } rvalue;

        readBlock(reg, sizeof(rvalue), rvalue.data);

        value = rvalue.val;
    }

    void write64reg(std::string reg, uint64_t value)
    {
        union {
            uint64_t val;
            char data[];
        } wvalue;

        wvalue.val = value;

        writeBlock(reg, sizeof(wvalue), wvalue.data);
    }

    int deviceIndex()
    {
        return boost::lexical_cast<int, std::string>(getStub().uri);
    }

    int endcap()
    {
        return (deviceIndex() / 6) + 1;
    }

    int sector()
    {
        const int sectorIndex[] = {1,6,3,4,2,5};
        return sectorIndex[deviceIndex() % 6];
    }

    std::string readControlFirmwareVersion(uint32_t *controlFirmwareVersion = NULL);
    std::string readCoreFirmwareVersion(uint32_t *coreFirmwareVersion = NULL);

private:
    uint64_t readFirmwareVersion();

    bool   readPLLstatus (void);
    int    readBC0counter(void);

    uint32_t readTrackRate(uint16_t track);

    uint32_t lctRate(std::string lctName);
    void generateLctPairs();

    swatch::core::Metric<bool>        & extPllLockStatus;
    swatch::core::Metric<int>         & bc0PeriodCounter;
    swatch::core::Metric<uint32_t>    & outputTrack0Rate;
    swatch::core::Metric<uint32_t>    & outputTrack1Rate;
    swatch::core::Metric<uint32_t>    & outputTrack2Rate;
    std::vector<std::pair<swatch::core::Metric<uint32_t> *,
                          std::string>> lctRates;
    swatch::core::Metric<std::string> & controlFirmwareVersion;
    swatch::core::Metric<std::string> & coreFirmwareVersion;

    HAL::PCIExprLinuxBusAdapter     busAdapter;
    HAL::PCIAddressTableASCIIReader * addressTableReader;
    HAL::PCIAddressTable            * addressTable;
    HAL::PCIExprDevice              * driver;

    log4cplus::Logger generalLogger;
    log4cplus::Logger rateLogger;
};

} // namespace

#endif /* __MTF7_PROCESSOR_HPP__ */

