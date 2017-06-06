#ifndef __DOUBLE_MUON_TRG_HPP__
#define __DOUBLE_MUON_TRG_HPP__

#include "swatch/action/Command.hpp"

namespace emtf {

class SetDoubleMuonTrg : public swatch::action::Command
{
public:
    SetDoubleMuonTrg(const std::string& aId, swatch::action::ActionableObject& aActionable);

    virtual swatch::action::Command::State code(const swatch::core::XParameterSet& params);
};

} // namespace

#endif /* __DOUBLE_MUON_HPP__ */

