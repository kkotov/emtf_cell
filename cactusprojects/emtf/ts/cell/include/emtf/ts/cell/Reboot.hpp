#ifndef __REBOOT_HPP__
#define __REBOOT_HPP__

#include <vector>
#include "swatch/action/Command.hpp"

namespace emtf {


class Reboot: public swatch::action::Command // Configure button in the Alex's expert tool
{
private:
    const useconds_t sleepInterval;

    void log(const char *prefix, uint32_t val, const char *suffix="");

public:
    Reboot(const std::string& aId, swatch::action::ActionableObject& aActionable);

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);
};


} // namespace

#endif /* __REBOOT_HPP__ */

