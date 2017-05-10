#ifndef __EMTF_PORT_FACTORY_HPP__
#define __EMTF_PORT_FACTORY_HPP__

#include "swatch/processor/Port.hpp"
#include "emtf/ts/cell/EmtfCppfPort.hpp"

namespace emtf {

class EmtfInputPortFactory
{
public:
    static swatch::processor::InputPort * createPort(const std::string& aID, const uint32_t portId, emtf::Mtf7Processor &parent)
    {
        if(0 == aID.find("me"))
        {
            return NULL;
        }
        else if(0 == aID.find("cppf_"))
        {
            return new EmtfInputCppfPort(aID, portId, parent);
        }
        else
        {
            // throw
            return NULL;
        }
    }
};

} // namespace

#endif /* __EMTF_PORT_FACTORY_HPP__ */

