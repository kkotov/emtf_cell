#ifndef __RESETS_HPP__
#define __RESETS_HPP__

#include <vector>
#include "swatch/core/Command.hpp"

namespace emtf {


class DAQModuleReset : public swatch::core::Command
{
private:
    const useconds_t sleepInterval;

public:
    DAQModuleReset(const std::string& aId, swatch::core::ActionableObject& aActionable);

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);
};


class ResetCoreLink: public swatch::core::Command
{
private:
    const useconds_t sleepInterval;

public:
    ResetCoreLink(const std::string& aId, swatch::core::ActionableObject& aActionable);

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);
};


class ResetGthTransceivers: public swatch::core::Command
{
private:
    const useconds_t sleepInterval;

public:
    ResetGthTransceivers(const std::string& aId, swatch::core::ActionableObject& aActionable);

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);
};


class ResetPtLut : public swatch::core::Command
{
private:
    const useconds_t sleepInterval;
    std::vector<uint32_t> clkWord;

public:
    ResetPtLut(const std::string& aId, swatch::core::ActionableObject& aActionable);

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);
};


class ResetRXBuffers : public swatch::core::Command
{
public:
    ResetRXBuffers(const std::string& aId, swatch::core::ActionableObject& aActionable);

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);
};


class SyncClockReset : public swatch::core::Command
{
private:
    const useconds_t sleepInterval;
    std::vector<uint32_t> syncClkWord;

public:
    SyncClockReset(const std::string& aId, swatch::core::ActionableObject& aActionable);

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);
};

} // namespace

#endif /* __RESETS_HPP__ */

