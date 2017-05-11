#ifndef __EMTF_CSC_PORT_IDS_HPP__
#define __EMTF_CSC_PORT_IDS_HPP__

#include <map>
#include <string>
#include <sstream>

namespace emtf {

class InputCscPortIds
{
public:
    static InputCscPortIds & getInstance()
    {
        return instance;
    }

    uint64_t getId(const uint32_t endcap, const uint32_t sector, std::string id)
    {
        std::stringstream mapId;
        mapId << endcap << "_" << sector << "_" << id;

        return ids.at(mapId.str());
    }


private:
    // this class is a singleton so hide the constructor, the copy-constructor, the assignment operator and the destructor
    InputCscPortIds();
    InputCscPortIds(const InputCscPortIds &init);
    InputCscPortIds & operator=(const InputCscPortIds &init);
    ~InputCscPortIds();

    static InputCscPortIds instance;

    // the format of the key is ENDCAP_SECTOR_ID where ENDCAP is [1 or 2], SECTOR is [1, 2, 3, 4, 5 or 6] and ID is [cppf_1, cppf_2, cppf_3, cppf_4, cppf_5, cppf_6, cppf_7]
    const std::map<std::string, uint64_t> ids;
};

} // namespace

#endif /* __EMTF_CSC_PORT_IDS_HPP__ */

