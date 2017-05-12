#ifndef __EMTF_CSC_PORT_HPP__
#define __EMTF_CSC_PORT_HPP__

#include "emtf/ts/cell/EmtfPortTemplate.hpp"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <string>

namespace emtf {

class EmtfCscInputPort : public EmtfInputPortTemplate
{
public:
    EmtfCscInputPort(const std::string& aID, const uint32_t portId, EmtfProcessor &parent);
    virtual ~EmtfCscInputPort();

    virtual void retrieveMetricValues();
    virtual uint64_t readLinkRealId();
    virtual uint64_t readLinkExpectedId();

    void logLinkStatus(bool forceLog=false);

private:
    const int64_t afDeltaMin;
    const int64_t afDeltaMax;

    bool readMetricIsLocked();
    bool readMetricIsAligned();
    uint32_t readMetricCRCErrors();

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

