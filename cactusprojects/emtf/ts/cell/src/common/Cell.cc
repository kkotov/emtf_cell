#include "emtf/ts/cell/Cell.h"
#include "swatch/action/ThreadPool.hpp"
#include "ts/framework/CellPanelFactory.h"
#include "swatchcell/framework/RedirectPanel.h"

namespace emtf {

XDAQ_INSTANTIATOR_IMPL(Cell)

Cell::Cell(xdaq::ApplicationStub * s) : swatchcellframework::CellAbstract(s)
{
}

Cell::~Cell()
{
}

void Cell::init()
{
    addGenericSwatchComponents();

    tsframework::CellPanelFactory* lPanelFactory = getContext()->getPanelFactory();
	lPanelFactory->add<swatchcellframework::RedirectPanel>("Home");
}

} // namespace

