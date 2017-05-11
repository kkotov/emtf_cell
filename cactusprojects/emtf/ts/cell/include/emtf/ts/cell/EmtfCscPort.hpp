#ifndef __EMTF_CSC_PORT_HPP__
#define __EMTF_CSC_PORT_HPP__

#include "emtf/ts/cell/EmtfPortTemplate.hpp"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

namespace emtf {

class EmtfCscInputPort : public EmtfInputPortTemplate
{
public:
    EmtfCscInputPort(const std::string& aID, const uint32_t portId, EmtfProcessor &parent);
    virtual ~EmtfCscInputPort();

    virtual void retrieveMetricValues();
    virtual uint64_t readLinkRealId();
    virtual uint64_t readLinkExpectedId();

public:
    void logLinkStatus(bool forceLog=false);
};

//{
//private:
//    const std::string id;
//    const uint64_t afDelayReference;
//    const int64_t afDeltaMin;
//    const int64_t afDeltaMax;
//
//    HAL::PCIExprDevice & driver_;
//
//    uint32_t sector, endcap;
//
//    swatch::core::Metric<std::string>& link_id_mismatch;
//
//    bool readMetricIsLocked();
//    bool readMetricIsAligned();
//    uint32_t readMetricCRCErrors();
//
//    log4cplus::Logger linkLogger;
//
//    bool lockedOld;
//    bool alignedOld;
//    bool crcOld;
//    bool idOld;
//};

} // namespace

#endif /* __EMTF_CSC_PORT_HPP__ */

