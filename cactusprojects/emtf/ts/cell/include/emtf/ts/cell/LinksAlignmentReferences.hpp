#ifndef __INPUT_LINKS_ALIGNMENT_REFERENCES__
#define __INPUT_LINKS_ALIGNMENT_REFERENCES__

#include "emtf/ts/cell/EmtfCscPort.hpp"
#include <map>

#define LINK_COUNT 49

namespace emtf {

class InputLinksAlignmentReferences
{
public:
    static uint32_t getReferenceValue(std::string processorName, uint32_t portId);

private:
    InputLinksAlignmentReferences();
    InputLinksAlignmentReferences(const InputLinksAlignmentReferences &init);
    InputLinksAlignmentReferences & operator=(const InputLinksAlignmentReferences &init);
    ~InputLinksAlignmentReferences();

    static std::map<std::string, uint32_t const *> InitializeMap();

private:
    static const uint32_t linkAlignmentReferencesPos1[LINK_COUNT];
    static const uint32_t linkAlignmentReferencesPos2[LINK_COUNT];
    static const uint32_t linkAlignmentReferencesPos3[LINK_COUNT];
    static const uint32_t linkAlignmentReferencesPos4[LINK_COUNT];
    static const uint32_t linkAlignmentReferencesPos5[LINK_COUNT];
    static const uint32_t linkAlignmentReferencesPos6[LINK_COUNT];
    static const uint32_t linkAlignmentReferencesNeg1[LINK_COUNT];
    static const uint32_t linkAlignmentReferencesNeg2[LINK_COUNT];
    static const uint32_t linkAlignmentReferencesNeg3[LINK_COUNT];
    static const uint32_t linkAlignmentReferencesNeg4[LINK_COUNT];
    static const uint32_t linkAlignmentReferencesNeg5[LINK_COUNT];
    static const uint32_t linkAlignmentReferencesNeg6[LINK_COUNT];

    static std::map<std::string, uint32_t const *> processorNameToArray;
};

} // namespace

#endif /* __INPUT_LINKS_ALIGNMENT_REFERENCES__ */

