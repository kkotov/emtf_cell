#ifndef __DAQ_CONFIG_REGISTERS_HPP__
#define __DAQ_CONFIG_REGISTERS_HPP__

#include "swatch/core/Command.hpp"

namespace emtf {

class DAQConfigRegisters : public swatch::core::Command
{
public:
    DAQConfigRegisters(const std::string& aId, swatch::core::ActionableObject& aActionable);

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);
};

} // namespace

#endif /* __DAQ_CONFIG_REGISTERS_HPP__ */

