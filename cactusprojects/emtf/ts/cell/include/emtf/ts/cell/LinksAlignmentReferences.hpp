#ifndef __INPUT_LINKS_ALIGNMENT_REFERENCES__
#define __INPUT_LINKS_ALIGNMENT_REFERENCES__

#include "emtf/ts/cell/EmtfCscPort.hpp"
#include <map>
#include <string>
#include <sstream>

namespace emtf {

class InputLinksAlignmentReferences
{
public:
    static uint32_t getReferenceValue(uint32_t endcap, uint32_t sector, std::string portId)
    {
        std::stringstream mapId;
        mapId << endcap << "_" << sector << "_" << portId;

        return referenceValues.at(mapId.str());
    }

private:
    InputLinksAlignmentReferences();
    InputLinksAlignmentReferences(const InputLinksAlignmentReferences &init);
    InputLinksAlignmentReferences & operator=(const InputLinksAlignmentReferences &init);
    ~InputLinksAlignmentReferences();

    static const std::map<std::string, uint32_t> referenceValues;
};

} // namespace

#endif /* __INPUT_LINKS_ALIGNMENT_REFERENCES__ */

