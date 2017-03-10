#include "emtf/ts/cell/Mtf7Processor.hpp"
#include "emtf/ts/cell/Mtf7ConfigCommands.hpp"
#include "xdata/Boolean.h"
#include "emtf/ts/cell/Mtf7Common.hpp"
#include <fstream>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace swatch;
using namespace core;


namespace emtf {

AlignmentFifoDelay::AlignmentFifoDelay(const std::string& aId, swatch::core::ActionableObject& aActionable) :
    swatch::core::Command(aId, aActionable, xdata::Integer(0))
{
    registerParameter("enable_auto_delays", xdata::Boolean(true));

    Mtf7Processor &processor = getActionable<Mtf7Processor>();

    fillRegisterNames();
    loadFixedAlignmentFifoDelays(processor.endcap(), processor.sector());

    // throw in case of error
}

swatch::core::Command::State AlignmentFifoDelay::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("Enable the automatic alignment of the fifo delays");
    Mtf7Processor &processor = getActionable<Mtf7Processor>();

    // TODO: test if the auto_delays is set correctly
    const xdata::Boolean auto_delays(params.get<xdata::Boolean>("enable_auto_af_delays").value_);
    cout << "THE VALUE OF AUTO_DELAYS IS: " << auto_delays << endl;

    processor.write64("af_en", auto_delays); // enable or disable the auto af delays

    Command::State commandStatus = ActionSnapshot::kDone;

    if(!auto_delays) // in case of auto delays disabled set fixed values for alignment filo delays
    {
        bool res = writeFixedAlignmentFifoDelays();

        if(!res)
        {
            commandStatus = ActionSnapshot::kError;
        }
    }

    setProgress(1.);

    return commandStatus;
}

void AlignmentFifoDelay::fillRegisterNames()
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

bool AlignmentFifoDelay::loadFixedAlignmentFifoDelays(int endcap, int sector)
{
    bool result = true;

    const string fileName = config::alignmentFifoDelayFile(endcap, sector);
    fstream file(fileName, ios::in);
    if(file.is_open())
    {
        string line;

        while(getline(file, line))
        {
            const uint64_t value = boost::lexical_cast<uint64_t>(line);

            registerValues.push_back(value);
        }

        file.close();
    }
    else
    {
        result = false;
    }

    return result;
}

bool AlignmentFifoDelay::writeFixedAlignmentFifoDelays()
{
    bool result = true;

    if((registerNames.size() == config::cscLinksCount()) && (registerNames.size() == registerValues.size()))
    {
        for(unsigned i=0; i<registerNames.size(); ++i)
        {
            // write to the board
        }
    }
    else
    {
        result = false;
    }

    return result;
}

} // namespace

