#ifndef __TRANSITION_COMMANDS_HPP__
#define __TRANSITION_COMMANDS_HPP__

#include "swatch/action/Command.hpp"

namespace emtf {

class OnStart : public swatch::action::Command
{
public:
    OnStart(const std::string& aId, swatch::action::ActionableObject& aActionable);

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);
};

class OnStop : public swatch::action::Command
{
public:
    OnStop(const std::string& aId, swatch::action::ActionableObject& aActionable);

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);
};

} // namespace emtf

#endif // __TRANSITION_COMMANDS_HPP__

