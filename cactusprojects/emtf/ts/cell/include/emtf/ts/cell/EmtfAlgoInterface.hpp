#ifndef __EMTF_ALGOINTERFACE_HPP__
#define __EMTF_ALGOINTERFACE_HPP__

#include "swatch/processor/AlgoInterface.hpp"
#include "emtf/pciExprLinuxBusAdapter/PCIExprDevice.hh"

namespace emtf {

class EmtfAlgoInterface : public swatch::processor::AlgoInterface {
public:
    EmtfAlgoInterface(HAL::PCIExprDevice& driver);
    ~EmtfAlgoInterface();

    void retrieveMetricValues();

private:
    HAL::PCIExprDevice& driver_;
};

} // namespace

#endif /* __EMTF_ALGOINTERFACE_HPP__ */

