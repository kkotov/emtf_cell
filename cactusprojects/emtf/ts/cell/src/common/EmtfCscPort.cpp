#include "emtf/ts/cell/EmtfCscPort.hpp"
#include "emtf/ts/cell/EmtfCscPortIds.hpp"
#include "emtf/ts/cell/Common.hpp"
#include "emtf/ts/cell/LinksAlignmentReferences.hpp"
#include <string>
#include <map>
#include <boost/lexical_cast.hpp>

namespace emtf {

using namespace std;
using namespace log4cplus;

EmtfCscInputPort::EmtfCscInputPort(const string & aID, const uint32_t portId, EmtfProcessor &parent) :
    EmtfInputPortTemplate(aID, portId, parent),
    afDeltaMin(-8),
    afDeltaMax(20),
    linkLogger(Logger::getInstance(config::log4cplusLinkLogger())),
    lockedOld(false),
    alignedOld(false),
    crcOld(false),
    idOld(false)
{
}

EmtfCscInputPort::~EmtfCscInputPort()
{
}

void EmtfCscInputPort::retrieveMetricValues()
{
    setMetricValue<bool>(mMetricIsLocked, readMetricIsLocked());
    setMetricValue<bool>(mMetricIsAligned, readMetricIsAligned());
    setMetricValue<uint32_t>(mMetricCRCErrors, readMetricCRCErrors());

    setMetricValue<string>(mLinkIdMismatch, compareLinkIds());

    logLinkStatus();
}

uint64_t EmtfCscInputPort::readLinkRealId()
{
    string regName = "link_id_" + id;

    uint64_t result = 0;

    parentProcessor.read64(regName, result);

    return result;
}

uint64_t EmtfCscInputPort::readLinkExpectedId()
{
    return EmtfCscInputPortIds::getId(parentProcessor.endcap(), parentProcessor.sector(), id);
}

bool EmtfCscInputPort::readMetricIsLocked()
{
    const string regName("bc0_err_" + id);

    uint64_t bc0Err;

    parentProcessor.read64(regName, bc0Err);

    return (0 == bc0Err);
}

bool EmtfCscInputPort::readMetricIsAligned()
{
    const string regName("af_delay_" + id);

    uint64_t afDelay;

    parentProcessor.read64(regName, afDelay);

    bool res = true;

    const int64_t delta = (afDelay - InputLinksAlignmentReferences::getReferenceValue(endcap(), sector(), id));

    if(   (0x40 <= afDelay)                       // afDelay must be smaller than 0x40
       || (delta < afDeltaMin)
       || (delta > afDeltaMax))
    {
        res = false;
    }

    return res;
}

uint32_t EmtfCscInputPort::readMetricCRCErrors()
{
    // TODO: implement the function when the firmware support for this metric is added

    return 0;
}

void EmtfCscInputPort::logLinkStatus(bool forceLog)
{
    const bool lockedOk = readMetricIsLocked();
    const bool alignedOk = readMetricIsAligned();
    const bool crcOk = (0 == readMetricCRCErrors());
    const bool idOk = (readLinkRealId() == readLinkExpectedId());

    if(    forceLog
        || (lockedOk  != lockedOld)
        || (alignedOk != alignedOld)
        || (crcOk     != crcOld)
        || (idOk      != idOld) )
    {
        lockedOld = lockedOk;
        alignedOld = alignedOk;
        crcOld = crcOk;
        idOld = idOk;

        string msg = "endcap " + boost::lexical_cast<string>(endcap()) + ", " + \
                     "sector " + boost::lexical_cast<string>(sector()) + ", " + \
                     id + " - " + \
                     "locked: " + boolToString(lockedOk) + ", " + \
                     "aligned: " + boolToString(alignedOk) + ", " + \
                     "crc: " + boolToString(crcOk) + ", " + \
                     "id: " + boolToString(idOk);
        LOG4CPLUS_TRACE(linkLogger, LOG4CPLUS_TEXT(msg));
    }
}

} // namespace

