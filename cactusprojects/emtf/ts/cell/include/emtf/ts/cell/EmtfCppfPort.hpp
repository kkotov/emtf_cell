#ifndef __EMTF_CPPF_PORT_HPP__
#define __EMTF_CPPF_PORT_HPP__

#include "emtf/ts/cell/EmtfPortTemplate.hpp"

namespace emtf {

class EmtfCppfInputPort : public EmtfInputPortTemplate
{
public:
    EmtfCppfInputPort(const std::string& aID, const uint32_t portId, EmtfProcessor &parent);
    virtual ~EmtfCppfInputPort();

    virtual void retrieveMetricValues();
    virtual uint64_t readLinkRealId();
    virtual uint64_t readLinkExpectedId();
};

} // namespace

#endif /* __EMTF_CPPF_PORT_HPP__ */

