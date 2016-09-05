#include "swatch/processor/PortCollection.hpp"
#include "emtf/ts/cell/Mtf7Processor.hpp"
#include "emtf/ts/cell/Mtf7Port.hpp"
#include "emtf/ts/cell/TransitionCommands.hpp"
#include <vector>

namespace emtf {

using namespace swatch::core;
using namespace swatch::processor;


OnStart::OnStart(const std::string& aId, swatch::core::ActionableObject& aActionable) :
    swatch::core::Command(aId, aActionable, xdata::Integer(0))
{
}

Command::State OnStart::code(const swatch::core::XParameterSet& params)
{
    setStatusMsg("Executed at the transition from 'Aligned' to 'Running'");
    Mtf7Processor &processor = getActionable<Mtf7Processor>();

    for(auto it=processor.getInputPorts().getPorts().begin(); it!=processor.getInputPorts().getPorts().end(); ++it)
    {
        dynamic_cast<Mtf7InputPort *>(*it)->logLinkStatus(true);
    }

    setProgress(1.);

    return ActionSnapshot::kDone;
}

}

