#ifndef __MTF7_DAQ_CONFIG_REGISTERS_HPP__
#define __MTF7_DAQ_CONFIG_REGISTERS_HPP__

#include "swatch/core/Command.hpp"

namespace emtf {

class Mtf7DAQConfigRegisters : public swatch::core::Command
{
public:
    Mtf7DAQConfigRegisters(const std::string& aId, swatch::core::ActionableObject& aActionable);

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);
};

} // namespace

#endif /* __MTF7_DAQ_CONFIG_REGISTERS_HPP__ */

