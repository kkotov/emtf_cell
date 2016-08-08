#ifndef _NICARD_H_
#define _NICARD_H_
#include "VMEDevice.hh"
#include "VMEAddressTable.hh"
#include "VMEAddressTableASCIIReader.hh"
#include "MXI2x86LinuxBusAdapter.hh"
#include "tk.h"

class NICard {

public:
  static int create(ClientData clientData, Tcl_Interp * interp,
              int argc, char* argv[]);

  static void deleteCard(ClientData clientData);

  static int command(ClientData clientData, Tcl_Interp * interp,
              int argc, char* argv[]);

private:
  NICard(uint32_t baseAddress,char* addressMap);

private:
  MXI2x86LinuxBusAdapter busAdapter;
  VMEAddressTableASCIIReader addressTableReader;
  VMEAddressTable addressTable;
  VMEDevice card;
  string name;
};

#endif
