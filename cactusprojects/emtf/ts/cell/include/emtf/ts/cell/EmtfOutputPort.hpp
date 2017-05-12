#ifndef __EMTF_OUTPUT_PORT_HPP__
#define __EMTF_OUTPUT_PORT_HPP__

#include "swatch/processor/Port.hpp"
#include "emtf/ts/cell/EmtfProcessor.hpp"

namespace emtf {

class EmtfOutputPort : public swatch::processor::OutputPort
{
public:
    EmtfOutputPort(const std::string& aID, const uint32_t portId, EmtfProcessor &parent);
    ~EmtfOutputPort();

    void retrieveMetricValues();

private:
    EmtfProcessor &parentProcessor;

    bool readMetricIsOperating();
};

} // namespace

#endif /* __EMTF_OUTPUT_PORT_HPP__ */

