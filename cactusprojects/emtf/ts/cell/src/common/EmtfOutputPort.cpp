#include "emtf/ts/cell/EmtfOutputPort.hpp"
#include "swatch/core/MetricConditions.hpp"

namespace emtf {


using namespace std;
using namespace log4cplus;


EmtfOutputPort::EmtfOutputPort(const string& aID, const uint32_t portId, EmtfProcessor &parent) :
    OutputPort(aID),
    parentProcessor(parent)
{
}

EmtfOutputPort::~EmtfOutputPort()
{
}

void EmtfOutputPort::retrieveMetricValues()
{
    setMetricValue<bool>(mMetricIsOperating, readMetricIsOperating());
}

bool EmtfOutputPort::readMetricIsOperating()
{
    // TODO: implement the function when the firmware support for this metric is added

    return true;
}

} // namespace

