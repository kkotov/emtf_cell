#ifndef __MTF7_SYSTEM_HPP__
#define __MTF7_SYSTEM_HPP__

#include "swatch/system/System.hpp"


namespace emtf {

class Mtf7System : public swatch::system::System {
public:
    Mtf7System(const swatch::core::AbstractStub& aStub);
    ~Mtf7System();
};

} // namespace

#endif /* __MTF7_SYSTEM_HPP__ */

