#ifndef __MTF7_PORT_HPP__
#define __MTF7_PORT_HPP__

#include "swatch/processor/Port.hpp"
#include "emtf/pciExprLinuxBusAdapter/PCIExprDevice.hh"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

namespace emtf {

class Mtf7InputPort : public swatch::processor::InputPort
{
public:
    Mtf7InputPort(const std::string& aID, const uint32_t portId, const std::string processorName, HAL::PCIExprDevice& driver);
    ~Mtf7InputPort();

    void retrieveMetricValues();

private:
    const std::string id;
    const uint64_t afDelayReference;

    HAL::PCIExprDevice & driver_;

    uint32_t sector, endcap;

    swatch::core::Metric<std::string>& link_id_mismatch;

    bool readMetricIsLocked();
    bool readMetricIsAligned();
    uint32_t readMetricCRCErrors();
    std::string readLinkID(void);

    log4cplus::Logger linkLogger;

    void logLinkStatus();
    bool lockedOld;
    bool alignedOld;
    bool crcOld;
    bool idOld;

    std::string boolToString(bool b)
    {
        return b ? "ok" : "NOK";
    }
};


class Mtf7OutputPort : public swatch::processor::OutputPort
{
public:
    Mtf7OutputPort(const std::string& aID, const uint32_t portId, HAL::PCIExprDevice& driver);
    ~Mtf7OutputPort();

    void retrieveMetricValues();

private:
    HAL::PCIExprDevice& driver_;

    bool readMetricIsOperating();
};

} // namespace

#endif /* __MTF7_PORT_HPP__ */

