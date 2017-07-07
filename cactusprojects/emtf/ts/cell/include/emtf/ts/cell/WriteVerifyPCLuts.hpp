#ifndef __WRITE_VERIFY_PCLUTS__
#define __WRITE_VERIFY_PCLUTS__

#include "swatch/action/Command.hpp"
#include <string>
#include <vector>
#include <map>
#include <boost/format.hpp>
#include <boost/thread/mutex.hpp>

namespace emtf {


class PcLuts
{
public:
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


class WritePcLuts : public swatch::action::Command
{
public:
    WritePcLuts(const std::string& aId, swatch::action::ActionableObject& aActionable);
    ~WritePcLuts()
    {
        delete pcLuts;
    }

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);

private:
    // hide the copy constructor and the assignment operator because the class allocates memory
    WritePcLuts(const WritePcLuts &);
    WritePcLuts operator=(const WritePcLuts &);

    void write(EmtfProcessor &processor, std::map<std::string, std::vector<unsigned long long>> &pairs);

    EmtfProcessor &processor;

    PcLuts *pcLuts;
};


class VerifyPcLuts : public swatch::action::Command
{
public:
    VerifyPcLuts(const std::string& aId, swatch::action::ActionableObject& aActionable);
    ~VerifyPcLuts()
    {
        delete pcLuts;
    }

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);

private:
    // hide the copy constructor and the assignment operator because the class allocates memory
    VerifyPcLuts(const VerifyPcLuts &);
    VerifyPcLuts operator=(const VerifyPcLuts &);

    void verify(EmtfProcessor &processor, std::map<std::string, std::vector<unsigned long long>> &pairs, Command::State &status);

    EmtfProcessor &processor;

    PcLuts *pcLuts;
};


class VerifyPcLutsVersion : public swatch::action::Command
{
public:
    VerifyPcLutsVersion(const std::string& aId, swatch::action::ActionableObject& aActionable);
    ~VerifyPcLutsVersion();

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);
};

} // namespace

#endif /* __WRITE_VERIFY_PCLUTS__ */

