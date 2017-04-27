#ifndef __SPY_CONFIG_REGISTERS_HPP__
#define __SPY_CONFIG_REGISTERS_HPP__

#include "swatch/core/Command.hpp"

namespace emtf {

class SetDelaysAndTriggerSource : public swatch::core::Command
{
public:
    SetDelaysAndTriggerSource(const std::string& aId, swatch::core::ActionableObject& aActionable);

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);
};


class SetSingleHits : public swatch::core::Command
{
public:
    SetSingleHits(const std::string& aId, swatch::core::ActionableObject& aActionable);

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);
};


class DaqReportWoTrack : public swatch::core::Command
{
public:
    DaqReportWoTrack(const std::string& aId, swatch::core::ActionableObject& aActionable);

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);
};


} // namespace

#endif /* __SPY_CONFIG_REGISTERS_HPP__ */

