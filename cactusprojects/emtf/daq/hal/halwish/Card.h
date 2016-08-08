#ifndef _CARD_H_
#define _CARD_H_
#include "VMEDevice.hh"
#include "VMEAddressTable.hh"
#include "VMEAddressTableASCIIReader.hh"
#if defined(BT1003)
#include "SBS620x86LinuxBusAdapter.hh"
#elif defined(VXILINUX)
#include "MXI2x86LinuxBusAdapter.hh"
#endif
#include "tk.h"

class Card {

public:
  static int create(ClientData clientData, Tcl_Interp * interp,
              int argc, char* argv[]);

  static void deleteCard(ClientData clientData);

  static int command(ClientData clientData, Tcl_Interp * interp,
              int argc, char* argv[]);

private:
  Card(uint32_t baseAddress,char* addressMap);

private:
#if defined(BT1003)
  SBS620x86LinuxBusAdapter busAdapter;
#elif defined(VXILINUX)
  MXI2x86LinuxBusAdapter busAdapter;
#endif
  VMEAddressTableASCIIReader addressTableReader;
  VMEAddressTable addressTable;
  VMEDevice card;
  string name;
};

#endif
