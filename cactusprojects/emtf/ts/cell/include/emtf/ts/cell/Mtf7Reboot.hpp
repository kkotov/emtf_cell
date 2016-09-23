#ifndef __Mtf7_REBOOT_HPP__
#define __Mtf7_REBOOT_HPP__

#include <vector>
#include "swatch/core/Command.hpp"

namespace emtf {


class Reboot: public swatch::core::Command // Configure button in the Alex's expert tool
{
private:

public:
    Reboot(const std::string& aId, swatch::core::ActionableObject& aActionable);

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);
};


} // namespace

#endif /* __Mtf7_RESETS_HPP__ */

