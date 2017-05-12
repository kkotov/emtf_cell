#include "emtf/ts/cell/EmtfCppfPort.hpp"
#include "emtf/ts/cell/EmtfCppfPortIds.hpp"
#include "emtf/ts/cell/Common.hpp"
#include <string>
#include <map>

namespace emtf {

using namespace std;

EmtfCppfInputPort::EmtfCppfInputPort(const string& aID, const uint32_t portId, EmtfProcessor &parent) :
    EmtfInputPortTemplate(aID, portId, parent)
{
}

EmtfCppfInputPort::~EmtfCppfInputPort()
{
}

void EmtfCppfInputPort::retrieveMetricValues()
{
    // the values of these three metrics are set to true, because there is no hardware implementation for them yet
    setMetricValue<bool>(mMetricIsLocked, true);
    setMetricValue<bool>(mMetricIsAligned, true);
    setMetricValue<uint32_t>(mMetricCRCErrors, 0);

    setMetricValue<string>(mLinkIdMismatch, compareLinkIds());
}

uint64_t EmtfCppfInputPort::readLinkRealId()
{
    string regName = "cppf_link_id_" + string(&id[5]);

    uint64_t result = 0;

    parentProcessor.read64(regName, result);

    return result;
}

uint64_t EmtfCppfInputPort::readLinkExpectedId()
{
    return EmtfCppfInputPortIds::getId(parentProcessor.endcap(), parentProcessor.sector(), id);
}

} // namespace

