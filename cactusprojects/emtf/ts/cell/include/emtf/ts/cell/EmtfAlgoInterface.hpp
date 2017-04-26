#ifndef __EMTF_ALGOINTERFACE_HPP__
#define __EMTF_ALGOINTERFACE_HPP__

#include "swatch/processor/AlgoInterface.hpp"
#include "emtf/pciExprLinuxBusAdapter/PCIExprDevice.hh"

namespace emtf {

class Mtf7AlgoInterface : public swatch::processor::AlgoInterface {
public:
    Mtf7AlgoInterface(HAL::PCIExprDevice& driver);
    ~Mtf7AlgoInterface();

    void retrieveMetricValues();

private:
    HAL::PCIExprDevice& driver_;
};

} // namespace

#endif /* __EMTF_ALGOINTERFACE_HPP__ */

