#ifndef __TRANSITION_COMMANDS_HPP__
#define __TRANSITION_COMMANDS_HPP__

#include "swatch/core/Command.hpp"

namespace emtf {

class OnStart : public swatch::core::Command
{
public:
    OnStart(const std::string& aId, swatch::core::ActionableObject& aActionable);

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);
};

} // namespace emtf

#endif // __TRANSITION_COMMANDS_HPP__

