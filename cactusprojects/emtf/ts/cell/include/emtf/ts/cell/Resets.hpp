#ifndef __RESETS_HPP__
#define __RESETS_HPP__

#include <vector>
#include "swatch/action/Command.hpp"

namespace emtf {


class DAQModuleReset : public swatch::action::Command
{
private:
    const useconds_t sleepInterval;

public:
    DAQModuleReset(const std::string& aId, swatch::action::ActionableObject& aActionable);

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);
};


class ResetCoreLink: public swatch::action::Command
{
private:
    const useconds_t sleepInterval;

public:
    ResetCoreLink(const std::string& aId, swatch::action::ActionableObject& aActionable);

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);
};


class ResetGthTransceivers: public swatch::action::Command
{
private:
    const useconds_t sleepInterval;

public:
    ResetGthTransceivers(const std::string& aId, swatch::action::ActionableObject& aActionable);

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);
};


class ResetPtLut : public swatch::action::Command
{
private:
    const useconds_t sleepInterval;
    std::vector<uint32_t> clkWord;

public:
    ResetPtLut(const std::string& aId, swatch::action::ActionableObject& aActionable);

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);
};


class ResetRXBuffers : public swatch::action::Command
{
public:
    ResetRXBuffers(const std::string& aId, swatch::action::ActionableObject& aActionable);

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);
};


class SyncClockReset : public swatch::action::Command
{
private:
    const useconds_t sleepInterval;
    std::vector<uint32_t> syncClkWord;

public:
    SyncClockReset(const std::string& aId, swatch::action::ActionableObject& aActionable);

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);
};

} // namespace

#endif /* __RESETS_HPP__ */

