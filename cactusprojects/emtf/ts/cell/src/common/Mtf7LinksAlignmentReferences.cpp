#include "emtf/ts/cell/Mtf7LinksAlignmentReferences.hpp"

using namespace std;

namespace emtf {

const uint32_t InputLinksAlignmentReferences::linkAlignmentReferencesPos1[LINK_COUNT] = \
    {15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 14, 15, 15, 16, 15, 15, 15, 15, 15, 15, 15, 14, 15, 15, 15, 15, 15, 15, 15, 14, 15, 14, 14, 14, 14, 15, 14, 14, 15, 14, 14, 14, 13, 15, 14, 15, 14};

const uint32_t InputLinksAlignmentReferences::linkAlignmentReferencesPos2[LINK_COUNT] = \
    {15, 15, 15, 15, 15, 15, 15, 15, 16, 16, 16, 16, 15, 16, 16, 17, 15, 15, 15, 15, 15, 15, 15, 14, 15, 15, 15, 15, 15, 15, 15, 14, 3, 3, 3, 3, 3, 3, 3, 2, 15, 14, 14, 15, 14, 15, 14, 14, 14};

const uint32_t InputLinksAlignmentReferences::linkAlignmentReferencesPos3[LINK_COUNT] = \
    {15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 14, 15, 15, 16, 15, 15, 14, 14, 15, 15, 14, 13, 15, 15, 14, 15, 14, 15, 14, 14, 9, 9, 9, 9, 9, 9, 9, 8, 16, 15, 15, 15, 14, 15, 14, 3, 2};

const uint32_t InputLinksAlignmentReferences::linkAlignmentReferencesPos4[LINK_COUNT] = \
    {15, 15, 14, 14, 15, 15, 15, 15, 15, 15, 15, 15, 14, 15, 15, 16, 14, 14, 14, 14, 14, 14, 14, 13, 15, 15, 15, 15, 15, 15, 15, 14, 15, 15, 14, 14, 14, 15, 14, 14, 15, 14, 14, 14, 14, 15, 14, 9, 8};

const uint32_t InputLinksAlignmentReferences::linkAlignmentReferencesPos5[LINK_COUNT] = \
    {42, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 40, 41, 41, 42, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 14, 15, 15, 14, 14, 14, 14, 14, 14, 14, 14, 14, 13, 15, 14, 14, 14, 13, 15, 14, 15, 14};

const uint32_t InputLinksAlignmentReferences::linkAlignmentReferencesPos6[LINK_COUNT] = \
    {15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 14, 15, 15, 16, 15, 14, 14, 14, 14, 14, 14, 13, 15, 15, 15, 15, 15, 15, 15, 14, 15, 15, 15, 15, 15, 15, 15, 14, 41, 40, 40, 15, 15, 15, 14, 14, 13};

const uint32_t InputLinksAlignmentReferences::linkAlignmentReferencesNeg1[LINK_COUNT] = \
    {15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 14, 15, 14, 16, 15, 15, 14, 15, 15, 15, 15, 14, 15, 15, 14, 15, 14, 15, 15, 14, 15, 15, 15, 15, 15, 15, 15, 14, 14, 13, 13, 14, 14, 14, 13, 14, 13};

const uint32_t InputLinksAlignmentReferences::linkAlignmentReferencesNeg2[LINK_COUNT] = \
    {15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 14, 14, 14, 15, 14, 16, 8, 8, 8, 8, 8, 8, 8, 7, 9, 9, 9, 9, 9, 9, 9, 8, 9, 9, 9, 9, 9, 9, 9, 8, 14, 14, 14, 14, 14, 14, 14, 15, 14};

const uint32_t InputLinksAlignmentReferences::linkAlignmentReferencesNeg3[LINK_COUNT] = \
    {15, 15, 15, 15, 15, 15, 15, 15, 15, 14, 14, 14, 14, 14, 14, 15, 9, 9, 9, 9, 9, 9, 9, 8, 8, 8, 8, 8, 9, 8, 8, 8, 3, 2, 2, 2, 2, 2, 2, 1, 15, 14, 14, 8, 8, 9, 8, 9, 8};

const uint32_t InputLinksAlignmentReferences::linkAlignmentReferencesNeg4[LINK_COUNT] = \
    {14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 13, 14, 14, 15, 15, 14, 14, 14, 14, 14, 14, 14, 15, 14, 14, 14, 15, 14, 14, 14, 3, 3, 3, 3, 3, 3, 3, 2, 14, 13, 13, 9, 8, 8, 7, 2, 2};


const uint32_t InputLinksAlignmentReferences::linkAlignmentReferencesNeg5[LINK_COUNT] = \
    {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 40, 42, 41, 43, 15, 15, 15, 15, 15, 15, 15, 14, 15, 15, 14, 14, 15, 15, 15, 14, 15, 15, 15, 15, 15, 15, 15, 14, 14, 13, 13, 14, 14, 15, 14, 3, 2};

const uint32_t InputLinksAlignmentReferences::linkAlignmentReferencesNeg6[LINK_COUNT] = \
    {15, 15, 14, 15, 15, 15, 15, 15, 15, 14, 14, 14, 14, 15, 14, 16, 15, 15, 15, 14, 15, 15, 15, 14, 15, 14, 14, 14, 14, 14, 14, 13, 14, 14, 14, 14, 14, 14, 14, 13, 41, 40, 40, 15, 14, 15, 14, 15, 14};


map<string, uint32_t const *> InputLinksAlignmentReferences::processorNameToArray = InputLinksAlignmentReferences::InitializeMap();



map<string, uint32_t const *> InputLinksAlignmentReferences::InitializeMap()
{
    map<string, uint32_t const *> map;

    map["EMTF+1"] = linkAlignmentReferencesPos1;
    map["EMTF+2"] = linkAlignmentReferencesPos2;
    map["EMTF+3"] = linkAlignmentReferencesPos3;
    map["EMTF+4"] = linkAlignmentReferencesPos4;
    map["EMTF+5"] = linkAlignmentReferencesPos5;
    map["EMTF+6"] = linkAlignmentReferencesPos6;

    map["EMTF-1"] = linkAlignmentReferencesNeg1;
    map["EMTF-2"] = linkAlignmentReferencesNeg2;
    map["EMTF-3"] = linkAlignmentReferencesNeg3;
    map["EMTF-4"] = linkAlignmentReferencesNeg4;
    map["EMTF-5"] = linkAlignmentReferencesNeg5;
    map["EMTF-6"] = linkAlignmentReferencesNeg6;

    return map;
}

uint32_t InputLinksAlignmentReferences::getReferenceValue(std::string processorName, uint32_t portId)
{
    return processorNameToArray[processorName][portId];
}

} // namespace

