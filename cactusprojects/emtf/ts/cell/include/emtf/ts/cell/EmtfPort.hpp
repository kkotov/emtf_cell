#ifndef __EMTF_PORT_HPP__
#define __EMTF_PORT_HPP__

#include "swatch/processor/Port.hpp"
#include "emtf/pciExprLinuxBusAdapter/PCIExprDevice.hh"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

namespace emtf {

class EmtfInputPort : public swatch::processor::InputPort
{
public:
    EmtfInputPort(const std::string& aID, const uint32_t portId, const std::string processorName, HAL::PCIExprDevice& driver);
    ~EmtfInputPort();

    void retrieveMetricValues();
    void logLinkStatus(bool forceLog=false);

private:
    const std::string id;
    const uint64_t afDelayReference;
    const int64_t afDeltaMin;
    const int64_t afDeltaMax;

    HAL::PCIExprDevice & driver_;

    uint32_t sector, endcap;

    swatch::core::Metric<std::string>& link_id_mismatch;

    bool readMetricIsLocked();
    bool readMetricIsAligned();
    uint32_t readMetricCRCErrors();
    std::string readLinkID(void);

    log4cplus::Logger linkLogger;

    bool lockedOld;
    bool alignedOld;
    bool crcOld;
    bool idOld;

    std::string boolToString(bool b)
    {
        return b ? "ok" : "NOK";
    }
};


class EmtfOutputPort : public swatch::processor::OutputPort
{
public:
    EmtfOutputPort(const std::string& aID, const uint32_t portId, HAL::PCIExprDevice& driver);
    ~EmtfOutputPort();

    void retrieveMetricValues();

private:
    HAL::PCIExprDevice& driver_;

    bool readMetricIsOperating();
};

} // namespace

#endif /* __EMTF_PORT_HPP__ */
