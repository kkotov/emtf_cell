#ifndef __EMTF_PORT_FACTORY_HPP__
#define __EMTF_PORT_FACTORY_HPP__

#include "swatch/processor/Port.hpp"
#include "emtf/ts/cell/EmtfCscPort.hpp"
#include "emtf/ts/cell/EmtfCppfPort.hpp"
#include <stdexcept>

namespace emtf {

class EmtfInputPortFactory
{
public:
    static swatch::processor::InputPort * createPort(const std::string& aID, const uint32_t portId, emtf::EmtfProcessor &parent) throw(std::runtime_error)
    {
        if(0 == aID.find("me"))
        {
            return new EmtfCscInputPort(aID, portId, parent);
        }

        if(0 == aID.find("cppf_"))
        {
            return new EmtfCppfInputPort(aID, portId, parent);
        }

        throw std::runtime_error("Invalid link name: " + aID);
    }
};

} // namespace

#endif /* __EMTF_PORT_FACTORY_HPP__ */

