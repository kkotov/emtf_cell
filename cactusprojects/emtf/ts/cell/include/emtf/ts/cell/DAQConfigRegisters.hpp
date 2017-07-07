#ifndef __DAQ_CONFIG_REGISTERS_HPP__
#define __DAQ_CONFIG_REGISTERS_HPP__

#include "swatch/action/Command.hpp"

namespace emtf {

class DAQConfigRegisters : public swatch::action::Command
{
public:
    DAQConfigRegisters(const std::string& aId, swatch::action::ActionableObject& aActionable);

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);
};

} // namespace

#endif /* __DAQ_CONFIG_REGISTERS_HPP__ */

