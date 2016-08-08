#ifndef _SBSCARD_H_
#define _SBSCARD_H_
#include "VMEDevice.hh"
#include "VMEAddressTable.hh"
#include "VMEAddressTableASCIIReader.hh"
#include "SBS620x86LinuxBusAdapter.hh"
#include "tk.h"

class SBSCard {

public:
  static int create(ClientData clientData, Tcl_Interp * interp,
              int argc, char* argv[]);

  static void deleteCard(ClientData clientData);

  static int command(ClientData clientData, Tcl_Interp * interp,
              int argc, char* argv[]);

private:
  SBSCard(uint32_t baseAddress,char* addressMap);

private:
  SBS620x86LinuxBusAdapter busAdapter;
  VMEAddressTableASCIIReader addressTableReader;
  VMEAddressTable addressTable;
  VMEDevice card;
  string name;
};

#endif
