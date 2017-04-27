#include "emtf/ts/cell/EmtfProcessor.hpp"
#include "emtf/ts/cell/ConfigCommands.hpp"
#include "emtf/ts/cell/Common.hpp"
#include "emtf/ts/cell/AlignmentFifoDelays.hpp"
#include "xdata/Boolean.h"
#include <fstream>
#include <string>

using namespace std;
using namespace swatch;
using namespace core;


namespace emtf {

AlignmentFifoDelays::AlignmentFifoDelays(const std::string& aId, swatch::core::ActionableObject& aActionable) :
    swatch::core::Command(aId, aActionable, xdata::Integer(0)),
    processor(getActionable<Mtf7Processor>())
{
    registerParameter("enable_auto_af_delays", xdata::Boolean(true));

    fillRegisterNames();
    loadFixedAlignmentFifoDelays();

    // throw in case of error
}

swatch::core::Command::State AlignmentFifoDelays::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("Enable automatic AF delays");

    const uint64_t auto_delays(params.get<xdata::Boolean>("enable_auto_af_delays").value_);

    processor.write64("af_en", auto_delays); // enable or disable the auto af delays

    Command::State commandStatus = ActionSnapshot::kDone;

    if(!auto_delays) // in case of auto delays disabled set fixed values for alignment filo delays
    {
        bool res = writeFixedAlignmentFifoDelays(); // write the values
        res &= verifyFixedAlignmentFifoDelays(); // verify the values

        if(!res)
        {
            commandStatus = ActionSnapshot::kError;
        }
    }

    setProgress(1.);

    return commandStatus;
}

void AlignmentFifoDelays::fillRegisterNames()
{
    registerNames.push_back("af_delay_me1a_02");
    registerNames.push_back("af_delay_me1a_03");
    registerNames.push_back("af_delay_me1a_04");
    registerNames.push_back("af_delay_me1a_05");
    registerNames.push_back("af_delay_me1a_06");
    registerNames.push_back("af_delay_me1a_07");
    registerNames.push_back("af_delay_me1a_08");
    registerNames.push_back("af_delay_me1a_09");
    registerNames.push_back("af_delay_me1b_02");
    registerNames.push_back("af_delay_me1b_03");
    registerNames.push_back("af_delay_me1b_04");
    registerNames.push_back("af_delay_me1b_05");
    registerNames.push_back("af_delay_me1b_06");
    registerNames.push_back("af_delay_me1b_07");
    registerNames.push_back("af_delay_me1b_08");
    registerNames.push_back("af_delay_me1b_09");
    registerNames.push_back("af_delay_me2_02");
    registerNames.push_back("af_delay_me2_03");
    registerNames.push_back("af_delay_me2_04");
    registerNames.push_back("af_delay_me2_05");
    registerNames.push_back("af_delay_me2_06");
    registerNames.push_back("af_delay_me2_07");
    registerNames.push_back("af_delay_me2_08");
    registerNames.push_back("af_delay_me2_09");
    registerNames.push_back("af_delay_me3_02");
    registerNames.push_back("af_delay_me3_03");
    registerNames.push_back("af_delay_me3_04");
    registerNames.push_back("af_delay_me3_05");
    registerNames.push_back("af_delay_me3_06");
    registerNames.push_back("af_delay_me3_07");
    registerNames.push_back("af_delay_me3_08");
    registerNames.push_back("af_delay_me3_09");
    registerNames.push_back("af_delay_me4_02");
    registerNames.push_back("af_delay_me4_03");
    registerNames.push_back("af_delay_me4_04");
    registerNames.push_back("af_delay_me4_05");
    registerNames.push_back("af_delay_me4_06");
    registerNames.push_back("af_delay_me4_07");
    registerNames.push_back("af_delay_me4_08");
    registerNames.push_back("af_delay_me4_09");
    registerNames.push_back("af_delay_me1n_03");
    registerNames.push_back("af_delay_me1n_06");
    registerNames.push_back("af_delay_me1n_09");
    registerNames.push_back("af_delay_me2n_03");
    registerNames.push_back("af_delay_me2n_09");
    registerNames.push_back("af_delay_me3n_03");
    registerNames.push_back("af_delay_me3n_09");
    registerNames.push_back("af_delay_me4n_03");
    registerNames.push_back("af_delay_me4n_09");
}

bool AlignmentFifoDelays::loadFixedAlignmentFifoDelays()
{
    bool result = true;

    const string fileName = config::alignmentFifoDelayFile(processor.endcap(), processor.sector());

    fstream file(fileName, ios::in);
    if(file.is_open())
    {
        string line;

        while(getline(file, line))
        {
            if(!line.empty()) // discard the empty lines
            {
                const uint64_t value = stoull(line, nullptr, 16);

                registerValues.push_back(value);
            }
        }

        file.close();
    }
    else
    {
        // TODO: log error or something
        result = false;
    }

    return result;
}

bool AlignmentFifoDelays::writeFixedAlignmentFifoDelays()
{
    bool result = true;

    if((registerNames.size() == config::cscLinksCount()) && (registerNames.size() == registerValues.size()))
    {
        for(unsigned i=0; i<registerNames.size(); ++i)
        {
            processor.write64(registerNames[i], registerValues[i]);
        }
    }
    else
    {
        result = false;
    }

    return result;
}

bool AlignmentFifoDelays::verifyFixedAlignmentFifoDelays()
{
    bool result = true;

    if((registerNames.size() == config::cscLinksCount()) && (registerNames.size() == registerValues.size()))
    {
        for(unsigned i=0; i<registerNames.size(); ++i)
        {
            uint64_t value;
            processor.read64(registerNames[i], value);

            if(value != registerValues[i])
            {
                result = false;
                break;
            }
        }
    }
    else
    {
        result = false;
    }

    return result;
}

} // namespace

