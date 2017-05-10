#include "emtf/ts/cell/EmtfCppfPort.hpp"
#include "emtf/ts/cell/Common.hpp"
#include <string>
#include <map>

namespace emtf {

using namespace std;


namespace InputCppfPort {
    static const map<string, uint64_t> ids = {
        // the format of the key is ENDCAP_SECTOR_ID where ENDCAP is [1 or 2], SECTOR is [1, 2, 3, 4, 5 or 6] and ID is [cppf_1, cppf_2, cppf_3, cppf_4, cppf_5, cppf_6, cppf_7]
        {"1_1_cppf_1", 0x49}, {"1_1_cppf_2", 0x4A}, {"1_1_cppf_3", 0x4B}, {"1_1_cppf_4", 0x4C}, {"1_1_cppf_5", 0x4D}, {"1_1_cppf_6", 0x4E}, {"1_1_cppf_7", 0x4F},
        {"1_2_cppf_1", 0x51}, {"1_2_cppf_2", 0x52}, {"1_2_cppf_3", 0x53}, {"1_2_cppf_4", 0x54}, {"1_2_cppf_5", 0x55}, {"1_2_cppf_6", 0x56}, {"1_2_cppf_7", 0x57},
        {"1_3_cppf_1", 0x59}, {"1_3_cppf_2", 0x5A}, {"1_3_cppf_3", 0x5B}, {"1_3_cppf_4", 0x5C}, {"1_3_cppf_5", 0x5D}, {"1_3_cppf_6", 0x5E}, {"1_3_cppf_7", 0x5F},
        {"1_4_cppf_1", 0x61}, {"1_4_cppf_2", 0x62}, {"1_4_cppf_3", 0x63}, {"1_4_cppf_4", 0x64}, {"1_4_cppf_5", 0x65}, {"1_4_cppf_6", 0x66}, {"1_4_cppf_7", 0x67},
        {"1_5_cppf_1", 0x69}, {"1_5_cppf_2", 0x6A}, {"1_5_cppf_3", 0x6B}, {"1_5_cppf_4", 0x6C}, {"1_5_cppf_5", 0x6D}, {"1_5_cppf_6", 0x6E}, {"1_5_cppf_7", 0x6F},
        {"1_6_cppf_1", 0x71}, {"1_6_cppf_2", 0x72}, {"1_6_cppf_3", 0x73}, {"1_6_cppf_4", 0x74}, {"1_6_cppf_5", 0x75}, {"1_6_cppf_6", 0x76}, {"1_6_cppf_7", 0x77},
        {"2_1_cppf_1", 0x89}, {"2_1_cppf_2", 0x8A}, {"2_1_cppf_3", 0x8B}, {"2_1_cppf_4", 0x8C}, {"2_1_cppf_5", 0x8D}, {"2_1_cppf_6", 0x8E}, {"2_1_cppf_7", 0x8F},
        {"2_2_cppf_1", 0x91}, {"2_2_cppf_2", 0x92}, {"2_2_cppf_3", 0x93}, {"2_2_cppf_4", 0x94}, {"2_2_cppf_5", 0x95}, {"2_2_cppf_6", 0x96}, {"2_2_cppf_7", 0x97},
        {"2_3_cppf_1", 0x99}, {"2_3_cppf_2", 0x9A}, {"2_3_cppf_3", 0x9B}, {"2_3_cppf_4", 0x9C}, {"2_3_cppf_5", 0x9D}, {"2_3_cppf_6", 0x9E}, {"2_3_cppf_7", 0x9F},
        {"2_4_cppf_1", 0xA1}, {"2_4_cppf_2", 0xA2}, {"2_4_cppf_3", 0xA3}, {"2_4_cppf_4", 0xA4}, {"2_4_cppf_5", 0xA5}, {"2_4_cppf_6", 0xA6}, {"2_4_cppf_7", 0xA7},
        {"2_5_cppf_1", 0xA9}, {"2_5_cppf_2", 0xAA}, {"2_5_cppf_3", 0xAB}, {"2_5_cppf_4", 0xAC}, {"2_5_cppf_5", 0xAD}, {"2_5_cppf_6", 0xAE}, {"2_5_cppf_7", 0xAF},
        {"2_6_cppf_1", 0xB1}, {"2_6_cppf_2", 0xB2}, {"2_6_cppf_3", 0xB3}, {"2_6_cppf_4", 0xB4}, {"2_6_cppf_5", 0xB5}, {"2_6_cppf_6", 0xB6}, {"2_6_cppf_7", 0xB7}
    };
};


EmtfInputCppfPort::EmtfInputCppfPort(const string& aID, const uint32_t portId, Mtf7Processor &parent) :
    EmtfInputPortTemplate(aID, portId, parent)
{
    // cout << "My id is: " << id << ", the endcap is: " << endcap() << ", the sektor is: " << sector() << endl;

    //stringstream myId;
    //myId << endcap() << "_" << sector() << "_" << aID;

    //cout << myId << " - " << InputCppfPort::ids.at(myId.str()) << endl;

    // cout << readLinkRealId() << " - " << readLinkExpectedId() << endl;

    // assert(readLinkRealId() == readLinkExpectedId());
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
    string regName = "cppf_link_id_" + string(&id[5]);

    uint64_t result = 0;

    parentProcessor.read64(regName, result);

    return result;
}

uint64_t EmtfInputCppfPort::readLinkExpectedId()
{
    stringstream myId;
    myId << endcap() << "_" << sector() << "_" << id;

    return InputCppfPort::ids.at(myId.str());
}


} // namespace

