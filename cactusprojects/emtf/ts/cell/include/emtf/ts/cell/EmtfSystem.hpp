#ifndef __EMTF_SYSTEM_HPP__
#define __EMTF_SYSTEM_HPP__

#include "swatch/system/System.hpp"
#include <vector>

namespace emtf {

class EmtfSystem : public swatch::system::System {
public:
    EmtfSystem(const swatch::core::AbstractStub& aStub);
    ~EmtfSystem();
};

} // namespace

#endif /* __EMTF_SYSTEM_HPP__ */

