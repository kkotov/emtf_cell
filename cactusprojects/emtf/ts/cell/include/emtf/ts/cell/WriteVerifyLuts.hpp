#ifndef __WRITE_VERIFY_LUTS__
#define __WRITE_VERIFY_LUTS__

#include "swatch/core/Command.hpp"
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
    // hide the copy constructor and the assignment operator because the class allocates memory
    WritePcLuts(const WritePcLuts &);
    WritePcLuts operator=(const WritePcLuts &);

    void write(EmtfProcessor &processor, std::map<std::string, std::vector<unsigned long long>> &pairs);

    EmtfProcessor &processor;

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
    // hide the copy constructor and the assignment operator because the class allocates memory
    VerifyPcLuts(const VerifyPcLuts &);
    VerifyPcLuts operator=(const VerifyPcLuts &);

    void verify(EmtfProcessor &processor, std::map<std::string, std::vector<unsigned long long>> &pairs, Command::State &status);

    EmtfProcessor &processor;

    PcLuts *pcLuts;
};


class VerifyPcLutsVersion : public swatch::core::Command
{
public:
    VerifyPcLutsVersion(const std::string& aId, swatch::core::ActionableObject& aActionable);
    ~VerifyPcLutsVersion();

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);
};

// Singleton holding the gigantic pT LUT
class PtLut
{
public:

    static const uint32_t* getAddress(void) throw (std::runtime_error) ;
    static const uint32_t* getData   (void) throw (std::runtime_error) ;
    static       uint32_t  size      (void) throw ();

private:
    static boost::mutex mtx;

    PtLut(void){}
    PtLut(const PtLut &){}
    PtLut& operator=(const PtLut &){ return *this; }

    static void readLUT(void) throw ( std::bad_alloc, std::ios_base::failure, std::runtime_error ) ;

    static uint32_t *data_buf;
    static uint32_t *addr_buf;
};


class VerifyWritePtLut : public swatch::core::Command
{
public:
    VerifyWritePtLut(const std::string& aId, swatch::core::ActionableObject& aActionable);
    ~VerifyWritePtLut(){}

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);

private:
    // hide the copy constructor and the assignment operator because the class allocates memory
    VerifyWritePtLut(const VerifyWritePtLut &);
    VerifyWritePtLut operator=(const VerifyWritePtLut &);

    EmtfProcessor &processor;
};

class VerifyPtLut : public swatch::core::Command
{
public:
    VerifyPtLut(const std::string& aId, swatch::core::ActionableObject& aActionable);
    ~VerifyPtLut(){}

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);

private:
    // hide the copy constructor and the assignment operator because the class allocates memory
    VerifyPtLut(const VerifyWritePtLut &);
    VerifyPtLut operator=(const VerifyWritePtLut &);

    EmtfProcessor &processor;
};

class WritePtLut : public swatch::core::Command
{
public:
    WritePtLut(const std::string& aId, swatch::core::ActionableObject& aActionable);
    ~WritePtLut(){}

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);

private:
    // hide the copy constructor and the assignment operator because the class allocates memory
    WritePtLut(const VerifyWritePtLut &);
    WritePtLut operator=(const VerifyWritePtLut &);

    EmtfProcessor &processor;
};

class VerifyPtLutVersion : public swatch::core::Command
{
public:
    VerifyPtLutVersion(const std::string& aId, swatch::core::ActionableObject& aActionable);
    ~VerifyPtLutVersion(){}

    virtual swatch::core::Command::State code(const swatch::core::XParameterSet& params);

private:
    // hide the copy constructor and the assignment operator because the class allocates memory
    VerifyPtLutVersion(const VerifyPtLutVersion &);
    VerifyPtLutVersion operator=(const VerifyPtLutVersion &);
};


} // namespace

#endif /* __WRITE_VERIFY_LUTS__ */

