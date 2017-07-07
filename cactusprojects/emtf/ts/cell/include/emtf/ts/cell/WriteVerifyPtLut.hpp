#ifndef __WRITE_VERIFY_PTLUT__
#define __WRITE_VERIFY_PTLUT__

#include "swatch/action/Command.hpp"
#include <string>
#include <vector>
#include <map>
#include <boost/format.hpp>
#include <boost/thread/mutex.hpp>

namespace emtf {


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


class VerifyWritePtLut : public swatch::action::Command
{
public:
    VerifyWritePtLut(const std::string& aId, swatch::action::ActionableObject& aActionable);
    ~VerifyWritePtLut(){}

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);

private:
    // hide the copy constructor and the assignment operator because the class allocates memory
    VerifyWritePtLut(const VerifyWritePtLut &);
    VerifyWritePtLut operator=(const VerifyWritePtLut &);

    EmtfProcessor &processor;
};


class VerifyPtLut : public swatch::action::Command
{
public:
    VerifyPtLut(const std::string& aId, swatch::action::ActionableObject& aActionable);
    ~VerifyPtLut(){}

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);

private:
    // hide the copy constructor and the assignment operator because the class allocates memory
    VerifyPtLut(const VerifyWritePtLut &);
    VerifyPtLut operator=(const VerifyWritePtLut &);

    EmtfProcessor &processor;
};


class WritePtLut : public swatch::action::Command
{
public:
    WritePtLut(const std::string& aId, swatch::action::ActionableObject& aActionable);
    ~WritePtLut(){}

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);

private:
    std::string writeDelaysName, readDelaysName;

    // hide the copy constructor and the assignment operator because the class allocates memory
    WritePtLut(const VerifyWritePtLut &);
    WritePtLut operator=(const VerifyWritePtLut &);

    EmtfProcessor &processor;
};


class VerifyPtLutVersion : public swatch::action::Command
{
public:
    VerifyPtLutVersion(const std::string& aId, swatch::action::ActionableObject& aActionable);
    ~VerifyPtLutVersion(){}

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);

private:
    // hide the copy constructor and the assignment operator because the class allocates memory
    VerifyPtLutVersion(const VerifyPtLutVersion &);
    VerifyPtLutVersion operator=(const VerifyPtLutVersion &);
};


} // namespace

#endif /* __WRITE_VERIFY_PTLUT__ */

