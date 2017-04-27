#ifndef __EMTF_READOUTINTERFACE_HPP__
#define __EMTF_READOUTINTERFACE_HPP__

#include "swatch/processor/ReadoutInterface.hpp"
#include "swatch/core/TTSUtils.hpp"
#include "emtf/pciExprLinuxBusAdapter/PCIExprDevice.hh"

namespace emtf {

class EmtfReadoutInterface : public swatch::processor::ReadoutInterface {
public:
    EmtfReadoutInterface(HAL::PCIExprDevice& driver);
    ~EmtfReadoutInterface(){};

    void retrieveMetricValues();

private:
    HAL::PCIExprDevice& board;

    swatch::core::tts::State readMetricTTS();
    bool readMetricAMCCoreReady();
    uint32_t readMetricEventCounter();
};

} // namespace

#endif /* __EMTF_READOUTINTERFACE_HPP__ */

