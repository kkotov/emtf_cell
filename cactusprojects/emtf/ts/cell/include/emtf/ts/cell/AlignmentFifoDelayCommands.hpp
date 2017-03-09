#ifndef __ALIGNMENT_FIFO_DELAY_COMMANDS_HPP__
#define __ALIGNMENT_FIFO_DELAY_COMMANDS_HPP__

#include "swatch/core/Command.hpp"

namespace emtf {

class AlignmentFifoDelay : public swatch::core::Command
{
public:
    AlignmentFifoDelay(const std::string& aId, swatch::core::ActionableObject& aActionable);

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);

private:
    bool setFixedAlignmentFifoDelays(int endcap, int sector);
};

} // namespace

#endif /* __ALIGNMENT_FIFO_DELAY_COMMANDS_HPP__ */

