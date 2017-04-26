#ifndef _EMTF_TTC_INTERFACE_HPP_
#define _EMTF_TTC_INTERFACE_HPP_

#include "swatch/processor/TTCInterface.hpp"
#include "emtf/pciExprLinuxBusAdapter/PCIExprDevice.hh"

namespace emtf {

class Mtf7TTCInterface : public swatch::processor::TTCInterface {
public:
    Mtf7TTCInterface(HAL::PCIExprDevice& driver);
    ~Mtf7TTCInterface();

    void retrieveMetricValues();

private:
    HAL::PCIExprDevice& board;

    uint32_t readBunchCounter();
    uint32_t readL1ACounter();
    uint32_t readOrbitCounter();

    bool readIsClock40Locked();

    bool readHasClock40Stopped();

    bool readIsBC0Locked();

    uint32_t readSingleBitErrors();

    uint32_t readDoubleBitErrors();
};

} // namespace

#endif // _EMTF_TTC_INTERFACE_HPP_

