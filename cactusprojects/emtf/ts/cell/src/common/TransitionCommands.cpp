#include "swatch/processor/PortCollection.hpp"
#include "emtf/ts/cell/EmtfProcessor.hpp"
#include "emtf/ts/cell/EmtfPort.hpp"
#include "emtf/ts/cell/TransitionCommands.hpp"
#include <vector>
#include "emtf/ts/cell/Common.hpp"
#include <boost/thread/barrier.hpp>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

namespace emtf {

using namespace swatch::core;
using namespace swatch::processor;
using namespace log4cplus;


// The whole boost::barrier will be removed once we migrate to the monitoring database and stopped using log files for the status of the input links.
// The number of processors the EMTF has is 12. This is very unlikely to change for the future and this is why the value is hard-coded.
static boost::barrier runStartBarrier(12);


OnStart::OnStart(const std::string& aId, swatch::core::ActionableObject& aActionable) :
    swatch::core::Command(aId, aActionable, xdata::Integer(0))
{
}

Command::State OnStart::code(const swatch::core::XParameterSet& params)
{
    if(runStartBarrier.wait())
    {
        log4cplus::Logger linkLogger(Logger::getInstance(config::log4cplusLinkLogger()));
        LOG4CPLUS_TRACE(linkLogger, LOG4CPLUS_TEXT("###   Beginning of run XXX XXX   ###"));
    }

    setStatusMsg("Executed at the transition from 'Aligned' to 'Running'");
    EmtfProcessor &processor = getActionable<EmtfProcessor>();

    for(auto it=processor.getInputPorts().getPorts().begin(); it!=processor.getInputPorts().getPorts().end(); ++it)
    {
        dynamic_cast<EmtfInputPort *>(*it)->logLinkStatus(true);
    }

    setProgress(1.);

    return ActionSnapshot::kDone;
}

}

