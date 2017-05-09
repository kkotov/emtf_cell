#include "emtf/ts/cell/EmtfCppfPort.hpp"
#include "emtf/ts/cell/Common.hpp"
#include <string>

namespace emtf {

using namespace std;

EmtfInputCppfPort::EmtfInputCppfPort(const string& aID, const uint32_t portId, Mtf7Processor &parent) :
    EmtfInputPortTemplate(aID, portId, parent)
{
    cout << "My id is: " << id << ", the endcap is: " << endcap() << ", the sektor is: " << sector() << endl;
}

EmtfInputCppfPort::~EmtfInputCppfPort()
{
}

void EmtfInputCppfPort::retrieveMetricValues()
{
    // the values of these three metrics are set to true, because there is no hardware implementation for them yet
    setMetricValue<bool>(mMetricIsLocked, true);
    setMetricValue<bool>(mMetricIsAligned, true);
    setMetricValue<uint32_t>(mMetricCRCErrors, true);

    setMetricValue<string>(mLinkIdMismatch, compareLinkIds());
}

uint64_t EmtfInputCppfPort::readLinkRealId()
{
    return 0;
}

uint64_t EmtfInputCppfPort::readLinkExpectedId()
{
    return 0;
}

} // namespace

