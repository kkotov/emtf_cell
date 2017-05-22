#ifndef __ALIGNMENT_FIFO_DELAYS_HPP__
#define __ALIGNMENT_FIFO_DELAYS_HPP__

#include "swatch/action/Command.hpp"
#include <vector>
#include <string>

namespace emtf {

class AlignmentFifoDelays : public swatch::action::Command
{
public:
    AlignmentFifoDelays(const std::string& aId, swatch::action::ActionableObject& aActionable);

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);

private:
    void fillRegisterNames();
    bool loadFixedAlignmentFifoDelays();
    bool writeFixedAlignmentFifoDelays();
    bool verifyFixedAlignmentFifoDelays();

    std::vector<std::string> registerNames;
    std::vector<uint64_t>    registerValues;

    EmtfProcessor &processor;
};

} // namespace

#endif /* __ALIGNMENT_FIFO_DELAYS_HPP__ */

