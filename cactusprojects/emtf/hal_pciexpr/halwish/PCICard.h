#ifndef _PCICARD_H_
#define _PCICARD_H_
#include "PCIDevice.hh"
#include "PCIAddressTable.hh"
#include "PCIAddressTableASCIIReader.hh"
#include "PCIi386BusAdapter.hh"
#include "tk.h"

class PCICard {

public:
  static int create(ClientData clientData, Tcl_Interp * interp,
              int argc, char* argv[]);

  static void deletePCICard(ClientData clientData);

  static int command(ClientData clientData, Tcl_Interp * interp,
              int argc, char* argv[]);

private:
  PCICard(uint32_t vendorid, uint32_t deviceid, uint32_t index
          ,char* addressMap, bool swapFlag);

private:
  PCIi386BusAdapter busAdapter;
  PCIAddressTableASCIIReader addressTableReader;
  PCIAddressTable addressTable;
  PCIDevice pcicard;
  string name;
};

#endif
