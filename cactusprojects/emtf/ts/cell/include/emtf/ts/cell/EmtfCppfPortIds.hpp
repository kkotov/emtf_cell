#ifndef __EMTF_CPPF_PORT_IDS_HPP__
#define __EMTF_CPPF_PORT_IDS_HPP__

#include <map>
#include <string>
#include <sstream>

namespace emtf {

class EmtfCppfInputPortIds
{
public:
    static uint64_t getId(const uint32_t endcap, const uint32_t sector, std::string id)
    {
        std::stringstream mapId;
        mapId << endcap << "_" << sector << "_" << id;

        return ids.at(mapId.str());
    }

private:
    // the class in not a Singleton but because only the static 'getId' function is supposed to be used hide the
    // constructor, the copy-constructor, the assignment operator and the destructor
    EmtfCppfInputPortIds();
    EmtfCppfInputPortIds(const EmtfCppfInputPortIds &init);
    EmtfCppfInputPortIds & operator=(const EmtfCppfInputPortIds &init);
    ~EmtfCppfInputPortIds();

    static const std::map<std::string, uint64_t> ids;
};

} // namespace

#endif /* __EMTF_CPPF_PORT_IDS_HPP__ */

