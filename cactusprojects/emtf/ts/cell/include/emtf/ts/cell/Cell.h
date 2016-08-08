#ifndef __EMTF_TS_CELL_H__
#define __EMTF_TS_CELL_H__

#include "swatchcell/framework/CellAbstract.h"

namespace emtf {

class Cell : public swatchcellframework::CellAbstract
{
public:
    XDAQ_INSTANTIATOR();

    Cell(xdaq::ApplicationStub * s);

    ~Cell();

    void init();
 
private:
    Cell( const Cell& );
};

} // namespace

#endif /* __EMTF_TS_CELL_H__ */

