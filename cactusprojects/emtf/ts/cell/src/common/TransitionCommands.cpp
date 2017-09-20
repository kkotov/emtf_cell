#include "swatch/processor/PortCollection.hpp"
#include "emtf/ts/cell/EmtfProcessor.hpp"
#include "emtf/ts/cell/EmtfCscPort.hpp"
#include "emtf/ts/cell/TransitionCommands.hpp"
#include <vector>
#include "emtf/ts/cell/Common.hpp"
#include <boost/thread/barrier.hpp>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

namespace emtf {

using namespace swatch::action;
using namespace swatch::processor;
using namespace std;
using namespace log4cplus;

OnStart::OnStart(const std::string& aId, swatch::action::ActionableObject& aActionable) :
    swatch::action::Command(aId, aActionable, xdata::Integer(0))
{
}

Command::State OnStart::code(const swatch::core::XParameterSet& params)
{
    // TODO: the run number XXX XXX needs to be taken from SWATCH
    const string runBeginMsg("###   Beginning of run XXX XXX   ###");

    log4cplus::Logger linkLogger(Logger::getInstance(config::log4cplusLinkLogger()));
    LOG4CPLUS_TRACE(linkLogger, LOG4CPLUS_TEXT(runBeginMsg));

    log4cplus::Logger rateLogger(Logger::getInstance(config::log4cplusRateLogger()));
    LOG4CPLUS_TRACE(rateLogger, LOG4CPLUS_TEXT(runBeginMsg));

    log4cplus::Logger lctLogger(Logger::getInstance(config::log4cplusLctLogger()));
    LOG4CPLUS_TRACE(lctLogger, LOG4CPLUS_TEXT(runBeginMsg));

    setStatusMsg("Starting link status logging");
    EmtfProcessor &processor = getActionable<EmtfProcessor>();

    for(auto it=processor.getInputPorts().getPorts().begin(); it!=processor.getInputPorts().getPorts().end(); ++it)
    {
        EmtfCscInputPort *port = dynamic_cast<EmtfCscInputPort *>(*it);

        if(port) // process only the CSC input ports
        {
            port->logLinkStatus(true);
        }
    }

    setProgress(1.);

    return Functionoid::kDone;
}

OnStop::OnStop(const std::string& aId, swatch::action::ActionableObject& aActionable) :
    swatch::action::Command(aId, aActionable, xdata::Integer(0))
{
}

Command::State OnStop::code(const swatch::core::XParameterSet& params)
{
    // TODO: the run number XXX XXX needs to be taken from SWATCH
    const string runBeginMsg("###   Ending run XXX XXX   ###");

    log4cplus::Logger linkLogger(Logger::getInstance(config::log4cplusLinkLogger()));
    LOG4CPLUS_TRACE(linkLogger, LOG4CPLUS_TEXT(runBeginMsg));

    log4cplus::Logger rateLogger(Logger::getInstance(config::log4cplusRateLogger()));
    LOG4CPLUS_TRACE(rateLogger, LOG4CPLUS_TEXT(runBeginMsg));

    log4cplus::Logger lctLogger(Logger::getInstance(config::log4cplusLctLogger()));
    LOG4CPLUS_TRACE(lctLogger, LOG4CPLUS_TEXT(runBeginMsg));

    return Functionoid::kDone;
}

}

