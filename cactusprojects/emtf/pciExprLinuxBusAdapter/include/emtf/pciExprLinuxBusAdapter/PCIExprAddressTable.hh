#ifndef __PCIExprAddressTable
#define __PCIExprAddressTable

#include <string>
#include "hal/PCIAddressTable.hh"

namespace HAL {

class PCIExprAddressTable : public PCIAddressTable
{
public:
    PCIExprAddressTable(std::string name, AddressTableReader& tableReader) : PCIAddressTable(name, tableReader)
    {
    };

    void checkAddressLimits(std::string item, uint64_t offset) const
        throw(NoSuchItemException, AddressOutOfLimitsException)
    {
    };

    void checkAddressLimits(const GeneralHardwareAddress& hardwareAddress, uint64_t offset) const
        throw( HAL::AddressOutOfLimitsException )
    {
    };
};

} /* namespace HAL */

#endif /* __PCIExprAddressTable */

