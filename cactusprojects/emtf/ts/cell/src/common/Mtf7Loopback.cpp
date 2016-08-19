#include "xdata/Integer.h"
#include "emtf/ts/cell/Mtf7Processor.hpp"
#include "xdata/UnsignedInteger32.h"
#include "xdata/String.h"
#include "boost/lexical_cast.hpp"
#include <iomanip>
#include <cstdlib>
#include "emtf/ts/cell/Mtf7Loopback.hpp"

using namespace std;
using namespace swatch;
using namespace core;


namespace emtf {

Mtf7Loopback::Mtf7Loopback(const std::string& aId, ActionableObject& aActionable) :
    swatch::core::Command(aId, aActionable, xdata::UnsignedInteger32(0u))
{
    xdata::String options("inject_64, inject_32, mpc_spy, pt_mem_data, memory");

    registerParameter("Number of iterations", xdata::UnsignedInteger32(100u));
    registerParameter("Options", options);
}

Command::State Mtf7Loopback::code(const XParameterSet& params)
{
    setStatusMsg("Loopback test started.");

    // const uint32_t iterations(params.get<xdata::UnsignedInteger32>("Number of iterations").value_);
    const string options(params.get<xdata::String>("Options").value_);

    Mtf7Processor &processor = getActionable<Mtf7Processor>();

    // for test memory loopback
    uint32_t lbMemSize = 0x10000; // 64 kiB
    uint32_t DATA_SIZE = 0x1000;
    // uint32_t N_WRITES = (lbMemSize / DATA_SIZE); // how many transfers for entire buffer

    // loopback module addresses
    // uint32_t MEM_BASE;
    // uint64_t REG_BASE;
    // uint32_t check_mask = 0xffffffffULL;
    // uint32_t invert_mask = 0;
    // uint32_t latency;
    // uint32_t pattern;

    if(0 == options.compare("inject_64")) {
        lbMemSize = 0x20000; // 128 kiB
    }
    else if(0 == options.compare("inject_32")) {
    }
    else if(0 == options.compare("mpc_spy")) {
    }
    else if(0 == options.compare("pt_mem_data")) {
    }
    else if(0 == options.compare("memory")) {
        // read "bc0 period" and "resync count"
        uint32_t value = 0;
        processor.read("test_reg2", value);

        const uint32_t bc0Period = (value & 0xfff);
        const uint32_t resyncCount = (value >> 12) & 0xfffff;

        cout << "The 'bc0 period' is: " << bc0Period << " (0x" << hex << bc0Period << ")" << endl;
        cout << "The 'resync count' is: " << resyncCount << " (0x" << hex << resyncCount << ")" << endl;

        // configure the size, start address, transfer size ...
    }

    union {
        uint64_t value;
        char     data[];
    } testW, testR;

    testW.value = 641;
    processor.writeBlock("test_mem", sizeof(testW), testW.data);
    // processor->writeBlock("corelogic_control", sizeof(testW), testW.data);

    testR.value = 124;
    processor.readBlock("test_mem", sizeof(testR), testR.data);
    // processor->readBlock("corelogic_control", sizeof(testR), testR.data);

    cout << "TestW is: " << testW.value << endl;
    cout << "TestR is: " << testR.value << endl;

    // processor->writeBlock("test_mem", sizeof(wb), (char *)wb);
    // processor->readBlock("test_mem", sizeof(rb), (char *)rb);

    // for(int i=0; i<4; i++)
    // {
    //     if(wb[i] != rb[i])
    //     {
    //         cout << "kolepeee   " << rb[i] << "   ";
    //     }
    // }
    // cout << endl;

/*
    uint32_t write_buf[2][lbMemSize/4];
    uint32_t read_buf[lbMemSize/4];

    memset(write_buf, 0x0, lbMemSize);
    memset(read_buf, 0x0, lbMemSize);

    srandom(time(NULL));

    for (size_t i=0; i<DATA_SIZE/4; i++)
    {
        write_buf[0][i] = random();
        write_buf[1][i] = random();
    }

    int mismatch_cnt = 0;
    srand(765);
    for (int n = 0; n < iterations; n++)
    {
        for (size_t i=0; i<lbMemSize/4; i++)
        {
            write_buf[n&1][i] = rand(); // pattern;
        }

        // processor->writeBlock("test_mem", lbMemSize, reinterpret_cast<char *>(write_buf));
        for (uint32_t j = 0; j < N_WRITES; j++)
        {
            // mwrite(fd, &(write_buf[n&1][j * DATA_SIZE/4]), DATA_SIZE, MEM_BASE + j * DATA_SIZE);
        }

        // processor->readBlock("test_mem", lbMemSize, reinterpret_cast<char *>(read_buf));
        for (uint32_t j = 0; j < N_WRITES; j++)
        {
            // off_t pos = (REG_BASE << 32) + MEM_BASE + j * DATA_SIZE;
            // mread(fd, &(read_buf[j * DATA_SIZE/4]), DATA_SIZE, pos);
        }

        uint32_t xorr;
        for (size_t i = 0; i < lbMemSize/4; i++)
        {
            read_buf[i] ^= invert_mask;

            xorr = (write_buf[n&1][i] ^ read_buf[i]) & check_mask;
            if (xorr != 0)
            {
                mismatch_cnt++;
            }
        }
    }

    cout << "The number of errors is: " << mismatch_cnt << endl;
*/

    // setProgress(0.5); // set the progress of the command in the web interface

    // verify the written value
    // Command::State commandStatus = kDone;
    // uint32_t valueRead = verify::CheckWrittenValue(processor, "test_reg1", value, commandStatus);

    // setResult(xdata::UnsignedInteger32(valueRead)); // set the return value of the command

    setProgress(1.);

    // return commandStatus;
    return ActionSnapshot::kDone;
}

} // namespace

