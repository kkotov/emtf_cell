#ifndef __EMTF_OUTPUT_PORT_HPP__
#define __EMTF_OUTPUT_PORT_HPP__

#include "swatch/processor/Port.hpp"
#include "emtf/pciExprLinuxBusAdapter/PCIExprDevice.hh"

namespace emtf {

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

#endif /* __EMTF_OUTPUT_PORT_HPP__ */

