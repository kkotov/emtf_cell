#include "emtf/ts/cell/AlignmentFifoDelayCommands.hpp"
#include "emtf/ts/cell/Mtf7Processor.hpp"
#include "emtf/ts/cell/Mtf7ConfigCommands.hpp"
#include "xdata/Boolean.h"

using namespace std;
using namespace swatch;
using namespace core;


namespace emtf {

AlignmentFifoDelay::AlignmentFifoDelay(const std::string& aId, swatch::core::ActionableObject& aActionable) :
    swatch::core::Command(aId, aActionable, xdata::Integer(0))
{
    registerParameter("enable_auto_delays", xdata::Boolean(true));
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
        bool res = setFixedAlignmentFifoDelays(processor.endcap(), processor.sector());

        if(!res)
        {
            commandStatus = ActionSnapshot::kError;
        }
    }

    setProgress(1.);

    return commandStatus;
}

bool AlignmentFifoDelay::setFixedAlignmentFifoDelays(int endcap, int sector)
{
    return true;
}

} // namespace

