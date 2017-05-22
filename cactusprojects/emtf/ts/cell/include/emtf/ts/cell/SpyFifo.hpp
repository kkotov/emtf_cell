#ifndef __SPY_CONFIG_REGISTERS_HPP__
#define __SPY_CONFIG_REGISTERS_HPP__

#include "swatch/action/Command.hpp"

namespace emtf {

class SetDelaysAndTriggerSource : public swatch::action::Command
{
public:
    SetDelaysAndTriggerSource(const std::string& aId, swatch::action::ActionableObject& aActionable);

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);
};


class SetSingleHits : public swatch::action::Command
{
public:
    SetSingleHits(const std::string& aId, swatch::action::ActionableObject& aActionable);

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);
};


class DaqReportWoTrack : public swatch::action::Command
{
public:
    DaqReportWoTrack(const std::string& aId, swatch::action::ActionableObject& aActionable);

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);
};


} // namespace

#endif /* __SPY_CONFIG_REGISTERS_HPP__ */

