#include "swatch/core/Factory.hpp"
#include "swatch/core/SystemStateMachine.hpp"
#include "swatch/dtm/DaqTTCManager.hpp"
#include "emtf/ts/cell/Mtf7System.hpp"
#include "emtf/ts/cell/Mtf7Common.hpp"
#include "swatch/processor/Processor.hpp"
#include "swatch/dtm/DaqTTCManager.hpp"
#include "swatch/core/MetricConditions.hpp"
#include <string>

using namespace std;
using namespace swatch::core;

#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>

namespace emtf {

using namespace std;


SWATCH_REGISTER_CLASS(emtf::Mtf7System);

Mtf7System::Mtf7System(const swatch::core::AbstractStub& aStub) :
    swatch::system::System(aStub),
    brokenLinks(registerMetric<uint16_t>("Number of input links in error",
                                         GreaterThanCondition<uint16_t>(config::brokenLinksErrorSystem()),
                                         RangeCondition<uint16_t>(config::brokenLinksWarningSystem(),
                                                                        config::brokenLinksErrorSystem())))
{
    // system run control state machine
    typedef swatch::processor::RunControlFSM ProcFSM_t;
    typedef swatch::dtm::RunControlFSM DaqTTCFSM_t;

    swatch::system::RunControlFSM& fsm = getRunControlFSM();


    fsm.coldReset.add(getDaqTTCs(), DaqTTCFSM_t::kStateInitial, DaqTTCFSM_t::kTrColdReset).add(getProcessors(), ProcFSM_t::kStateInitial, ProcFSM_t::kTrColdReset);

    fsm.setup.add(getDaqTTCs(), DaqTTCFSM_t::kStateInitial, DaqTTCFSM_t::kTrClockSetup).add(getProcessors(), ProcFSM_t::kStateInitial, ProcFSM_t::kTrSetup).add(getDaqTTCs(), DaqTTCFSM_t::kStateClockOK, DaqTTCFSM_t::kTrCfgDaq);

    fsm.configure.add(getProcessors(), ProcFSM_t::kStateSync, ProcFSM_t::kTrConfigure);

    fsm.align.add(getProcessors(), ProcFSM_t::kStateConfigured, ProcFSM_t::kTrAlign);

    fsm.start.add(getProcessors(), ProcFSM_t::kStateAligned, ProcFSM_t::kTrStart).add(getDaqTTCs(), DaqTTCFSM_t::kStateConfigured, DaqTTCFSM_t::kTrStart);

    fsm.pause.add(getDaqTTCs(), DaqTTCFSM_t::kStateRunning, DaqTTCFSM_t::kTrPause);

    fsm.resume.add(getDaqTTCs(), DaqTTCFSM_t::kStatePaused, DaqTTCFSM_t::kTrResume);

    fsm.stopFromAligned.add(getProcessors(), ProcFSM_t::kStateAligned, ProcFSM_t::kTrStop);

    fsm.stopFromRunning.add(getDaqTTCs(), DaqTTCFSM_t::kStateRunning, DaqTTCFSM_t::kTrStop).add(getProcessors(), ProcFSM_t::kStateRunning, ProcFSM_t::kTrStop);

    fsm.stopFromPaused.add(getDaqTTCs(), DaqTTCFSM_t::kStatePaused, DaqTTCFSM_t::kTrStop).add(getProcessors(), ProcFSM_t::kStateRunning, ProcFSM_t::kTrStop);


    const string emtfLog4cplusPropertyFile(config::log4cplusPropertyFile());

    log4cplus::Logger generalLogger(log4cplus::Logger::getInstance(config::log4cplusGeneralLogger()));
    LOG4CPLUS_INFO(generalLogger, LOG4CPLUS_TEXT("Emtf log4cplus property file: " + emtfLog4cplusPropertyFile));

    // append a specific emtf logging configuration to the existing log4cplus
    log4cplus::PropertyConfigurator::doConfigure(emtfLog4cplusPropertyFile);
}

Mtf7System::~Mtf7System()
{
}

uint16_t Mtf7System::countBrokenLinks(void)
{
    uint16_t counter = 0;

    for(auto it=getProcessors().begin(); it!=getProcessors().end(); ++it)
    {
        counter += (dynamic_cast<Mtf7Processor *>(*it))->countBrokenLinks();
    }

    return counter;
}

void Mtf7System::retrieveMetricValues()
{
    setMetricValue<uint16_t>(brokenLinks, countBrokenLinks());
}

} // namespace

