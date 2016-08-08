#ifndef __MTF7_SPY_CONFIG_REGISTERS_HPP__
#define __MTF7_SPY_CONFIG_REGISTERS_HPP__

#include "swatch/core/Command.hpp"

namespace emtf {

class Mtf7SetDelaysAndTriggerSource : public swatch::core::Command
{
public:
    Mtf7SetDelaysAndTriggerSource(const std::string& aId, swatch::core::ActionableObject& aActionable);

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);
};


class Mtf7SetSingleHits : public swatch::core::Command
{
public:
    Mtf7SetSingleHits(const std::string& aId, swatch::core::ActionableObject& aActionable);

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);
};


class Mtf7DaqReportWoTrack : public swatch::core::Command
{
public:
    Mtf7DaqReportWoTrack(const std::string& aId, swatch::core::ActionableObject& aActionable);

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);
};


} // namespace

#endif /* __MTF7_SPY_CONFIG_REGISTERS_HPP__ */

