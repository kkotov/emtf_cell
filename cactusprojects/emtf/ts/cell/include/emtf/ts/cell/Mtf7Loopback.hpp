#ifndef __MTF7_LOOPBACK_HPP__
#define __MTF7_LOOPBACK_HPP__

#include "swatch/core/Command.hpp"
#include "swatch/core/XParameterSet.hpp"

namespace emtf {

class Mtf7Loopback : public swatch::core::Command
{
public:
    Mtf7Loopback(const std::string& aId, swatch::core::ActionableObject& aActionable);

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);
};

} // namespace

#endif /* __MTF7_LOOPBACK_HPP__ */

