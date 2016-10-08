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

    void write(Mtf7Processor &processor, std::map<std::string, std::vector<unsigned long long>> &pairs);

    Mtf7Processor &processor;

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

    void verify(Mtf7Processor &processor, std::map<std::string, std::vector<unsigned long long>> &pairs, Command::State &status);

    Mtf7Processor &processor;

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
#include <boost/thread/mutex.hpp>
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

    void init(void);
    void write_mrs(uint32_t cs, uint32_t code );
    void setWriteDelays(void);
    void setReadDelays (void);
    void log(const char *prefix, uint64_t val, const char *suffix="");
    void log(const char *message);

    bool verify(void); // returns true if error occurs
    void write (void);

    Mtf7Processor &processor;
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

#endif /* __Mtf7_WRITE_VERIFY_LUTS__ */

