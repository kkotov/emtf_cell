#ifndef __EMTF_PORT_TEMPLATE_HPP__
#define __EMTF_PORT_TEMPLATE_HPP__

#include "swatch/processor/Port.hpp"
#include "emtf/ts/cell/EmtfProcessor.hpp"
#include "swatch/core/MetricConditions.hpp"
#include <string>

namespace emtf {

class EmtfInputPortTemplate : public swatch::processor::InputPort
{
public:
    EmtfInputPortTemplate(const std::string& aID, const uint32_t portId, EmtfProcessor &parent) :
        InputPort(aID),
        id(aID),
        mLinkIdMismatch(registerMetric<std::string>("linkIdMismatch", swatch::core::LessThanCondition<std::string>(""), swatch::core::NotEqualCondition<std::string>("none"))),
        parentProcessor(parent)
    {
    }

    virtual ~EmtfInputPortTemplate() {};

    virtual void retrieveMetricValues() = 0;

    virtual std::string getId(void) const { return id; }

protected:
    const std::string id;

    swatch::core::SimpleMetric<std::string> & mLinkIdMismatch;

    EmtfProcessor &parentProcessor;

    uint32_t endcap()
    {
        return parentProcessor.endcap();
    }

    uint32_t sector()
    {
        return parentProcessor.sector();
    }

    virtual uint64_t readLinkRealId() = 0;
    virtual uint64_t readLinkExpectedId() = 0;

    std::string compareLinkIds()
    {
        std::stringstream mismatches;

        if(readLinkRealId() != readLinkExpectedId())
        {
            mismatches << "read/expect: "<< std::hex << "0x" << readLinkRealId() << "/0x" << readLinkExpectedId() << std::dec;
        }

        if(0 == mismatches.str().length())
        {
            return std::string("none");
        }

        return mismatches.str();
    }
};

} // namespace

#endif /* __EMTF_PORT_TEMPLATE_HPP__ */

