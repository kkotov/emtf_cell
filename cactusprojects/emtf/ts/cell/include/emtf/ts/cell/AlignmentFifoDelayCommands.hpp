#ifndef __ALIGNMENT_FIFO_DELAY_COMMANDS_HPP__
#define __ALIGNMENT_FIFO_DELAY_COMMANDS_HPP__

#include "swatch/core/Command.hpp"
#include <vector>
#include <string>

namespace emtf {

class AlignmentFifoDelay : public swatch::core::Command
{
public:
    AlignmentFifoDelay(const std::string& aId, swatch::core::ActionableObject& aActionable);

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);

private:
    void fillRegisterNames();
    bool loadFixedAlignmentFifoDelays(int endcap, int sector);
    bool writeFixedAlignmentFifoDelays();

    std::vector<std::string> registerNames;
    std::vector<uint64_t>    registerValues;
};

} // namespace

#endif /* __ALIGNMENT_FIFO_DELAY_COMMANDS_HPP__ */

