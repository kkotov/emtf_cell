#ifndef __EMTF_CSC_PORT_HPP__
#define __EMTF_CSC_PORT_HPP__

#include "swatch/processor/Port.hpp"
#include "emtf/pciExprLinuxBusAdapter/PCIExprDevice.hh"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

namespace emtf {

class EmtfCscInputPort : public swatch::processor::InputPort
{
public:
    EmtfCscInputPort(const std::string& aID, const uint32_t portId, const std::string processorName, HAL::PCIExprDevice& driver);
    ~EmtfCscInputPort();

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

} // namespace

#endif /* __EMTF_CSC_PORT_HPP__ */

