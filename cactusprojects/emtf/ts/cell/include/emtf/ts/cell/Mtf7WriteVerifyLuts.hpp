#ifndef __Mtf7_WRITE_VERIFY_LUTS__
#define __Mtf7_WRITE_VERIFY_LUTS__

#include "swatch/core/Command.hpp"
#include <string>
#include <vector>
#include <map>
#include <boost/format.hpp>

namespace emtf {


class PcLuts
{
public:
    static const std::string prefix;

    PcLuts(int endcap, int sector);

    // return map of pairs where each pair consist of "register name" and a "vector of unsigned long long values"
    const std::map<std::string, std::vector<unsigned long long>> & getThLutPairs()
    {
        return thLutPairs;
    }

    // return map of pairs where each pair consist of "register name" and a "vector of unsigned long long values"
    const std::map<std::string, std::vector<unsigned long long>> & getCorrPairs()
    {
        return thCorrPairs;
    }

private:
    bool loadData(std::string regName, std::string fileName, std::map<std::string, std::vector<unsigned long long>> & thLutPairs);

    void generateThLutPairs(int endcap, int sector, std::map<std::string, std::vector<unsigned long long>> & thLutPairs);
    std::map<std::string, std::vector<unsigned long long>> thLutPairs;

    void generateThCorrPairs(int endcap, int sector, std::map<std::string, std::vector<unsigned long long>> & thCorrPairs);
    std::map<std::string, std::vector<unsigned long long>> thCorrPairs;
};


class WritePcLuts : public swatch::core::Command
{
public:
    WritePcLuts(const std::string& aId, swatch::core::ActionableObject& aActionable);
    ~WritePcLuts()
    {
        delete pcLuts;
    }

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);

private:
    void write(Mtf7Processor &processor, std::map<std::string, std::vector<unsigned long long>> &pairs);

    PcLuts *pcLuts;
};


class VerifyPcLuts : public swatch::core::Command
{
public:
    VerifyPcLuts(const std::string& aId, swatch::core::ActionableObject& aActionable);
    ~VerifyPcLuts()
    {
        delete pcLuts;
    }

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);

private:
    void verify(Mtf7Processor &processor, std::map<std::string, std::vector<unsigned long long>> &pairs, Command::State &status);

    PcLuts *pcLuts;
};


class VerifyPcLutsVersion : public swatch::core::Command
{
public:
    VerifyPcLutsVersion(const std::string& aId, swatch::core::ActionableObject& aActionable);
    ~VerifyPcLutsVersion();

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);
};

} // namespace

#endif /* __Mtf7_WRITE_VERIFY_LUTS__ */

