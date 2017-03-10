#ifndef __ALIGNMENT_FIFO_DELAYS_HPP__
#define __ALIGNMENT_FIFO_DELAYS_HPP__

#include "swatch/core/Command.hpp"
#include <vector>
#include <string>

namespace emtf {

class AlignmentFifoDelays : public swatch::core::Command
{
public:
    AlignmentFifoDelays(const std::string& aId, swatch::core::ActionableObject& aActionable);

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);

private:
    void fillRegisterNames();
    bool loadFixedAlignmentFifoDelays();
    bool writeFixedAlignmentFifoDelays();
    bool verifyFixedAlignmentFifoDelays();

    std::vector<std::string> registerNames;
    std::vector<uint64_t>    registerValues;

    Mtf7Processor &processor;
};

} // namespace

#endif /* __ALIGNMENT_FIFO_DELAYHPP__ */

