#ifndef __EMTF_SYSTEM_HPP__
#define __EMTF_SYSTEM_HPP__

#include "swatch/system/System.hpp"
#include <vector>

namespace emtf {

class Mtf7System : public swatch::system::System {
public:
    Mtf7System(const swatch::core::AbstractStub& aStub);
    ~Mtf7System();
};

} // namespace

#endif /* __EMTF_SYSTEM_HPP__ */

