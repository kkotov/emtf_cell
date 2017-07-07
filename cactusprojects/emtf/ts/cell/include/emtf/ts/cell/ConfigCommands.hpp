#ifndef __CONFIG_COMMANDS_HPP__
#define __CONFIG_COMMANDS_HPP__

#include "swatch/action/Command.hpp"

namespace emtf {

class CheckFWVersion : public swatch::action::Command
{
public:
    CheckFWVersion(const std::string& aId, swatch::action::ActionableObject& aActionable);

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);
};

class AlgoConfig: public swatch::action::Command
{
public:
    AlgoConfig(const std::string& aId, swatch::action::ActionableObject& aActionable);

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);
};

class SetDoubleMuonTrg : public swatch::action::Command
{
public:
    SetDoubleMuonTrg(const std::string& aId, swatch::action::ActionableObject& aActionable);

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);
};

} // namespace

#endif /* __CONFIG_COMMANDS_HPP__ */

