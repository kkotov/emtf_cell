#include "hal/ConfigROMDumper.hh"
#include <iostream>
#include <iomanip>

HAL::ConfigROMDumper::ConfigROMDumper() {
}

void HAL::ConfigROMDumper::dumpROM( const VMEConfigurationSpaceHandler& configHandler, 
				    const uint32_t slotId ) {
  std::vector<unsigned char> rom;
  uint32_t adr = 0x03;
  uint32_t result;

  for ( uint32_t offset = 0; adr <= 0x74f; offset += 4, adr+=4 ) {
    configHandler.configRead( "checksum", slotId, &result, offset );
    rom.push_back( (unsigned char)result );
  }

  displayItem( rom, 1, 0x000003, "checksum",           "checksum of configuration ROM");
  displayItem( rom, 3, 0x000007, "romLength",          "length of the Configuration ROM   ");
  displayItem( rom, 1, 0x000013, "CRWidth",            "Configuration ROM (CR)  access width");
  displayItem( rom, 1, 0x000017, "CSRWidth",           "Configuration Status Reg (CSR) access width");
  displayItem( rom, 1, 0x00001b, "specificationId",    "CR/CSR specification ID");
  displayItem( rom, 2, 0x00001f, "CR",                 "to identify valid CR");
  displayItem( rom, 3, 0x000027, "manufacturerId",     "Manufacturer Id"                              );
  displayItem( rom, 4, 0x000033, "boardId",            "Board Id (from manufacturer)"         );
  displayItem( rom, 4, 0x000043, "revisionId",         "Board-Revision Id (from manufacturer)");
  displayItem( rom, 3, 0x000053, "descriptionPtr",     "Pointer to Null terminated string "    );
  displayItem( rom, 1, 0x00007f, "programId",          "Program Id (if available)"                    );
  displayItem( rom, 3, 0x000083, "userCRStart",        "0 or Pointer to start of User CR "     );
  displayItem( rom, 3, 0x00008f, "userCREnd",          "0 or Pointer to end of User CR "       );
  displayItem( rom, 3, 0x00009b, "CRAMStart",          "0 or Pointer to start of CRAM "        );
  displayItem( rom, 3, 0x0000a7, "CRAMEnd",            "0 or Pointer to end of CRAM "          );
  displayItem( rom, 3, 0x0000b3, "userCSRStart",       "0 or Pointer to start of User CSR "    );
  displayItem( rom, 3, 0x0000bf, "userCSREnd",         "0 or Pointer to end of User CSR "      );
  displayItem( rom, 3, 0x0000cb, "serialNumberStart",  "0 or Pointer to start of Serialnumber ");
  displayItem( rom, 3, 0x0000d7, "serialNumberEnd",    "0 or Pointer to end of Serialnumber "  );

  displayItem( rom, 1, 0x0000e3, "slaveCharacts",      "characteristics of slave interface"  );
  displayItem( rom, 1, 0x0000e7, "userSlaveCharacts",  "user defined characteristics of slave interface");
  displayItem( rom, 1, 0x0000eb, "masterCharcts",      "characteristics of master interface"  );
  displayItem( rom, 1, 0x0000ef, "userMasterCharacts", "user defined characteristics of master interface"  );
  displayItem( rom, 1, 0x0000f3, "irqHandlerCap",      "interrupt handler capabilities"  );
  displayItem( rom, 1, 0x0000f7, "interrupterCap",     "interrupter capabilities"  );
  displayItem( rom, 1, 0x0000ff, "CRAMWidth",          "access width of CRAM"  );

  displayItem( rom, 1, 0x000103, "dataAccessWidth-F0", "Data access width for function 0"             );
  displayItem( rom, 1, 0x000107, "dataAccessWidth-F1", "Data access width for function 1"             );
  displayItem( rom, 1, 0x00010b, "dataAccessWidth-F2", "Data access width for function 2"             );
  displayItem( rom, 1, 0x00010f, "dataAccessWidth-F3", "Data access width for function 3"             );
  displayItem( rom, 1, 0x000113, "dataAccessWidth-F4", "Data access width for function 4"             );
  displayItem( rom, 1, 0x000117, "dataAccessWidth-F5", "Data access width for function 5"             );
  displayItem( rom, 1, 0x00011b, "dataAccessWidth-F6", "Data access width for function 6"             );
  displayItem( rom, 1, 0x00011f, "dataAccessWidth-F7", "Data access width for function 7"             );
  displayItem( rom, 8, 0x000123, "AMCAP-F0",         "AM-capability for function 0"         );
  displayItem( rom, 8, 0x000143, "AMCAP-F1",         "AM-capability for function 1"         );
  displayItem( rom, 8, 0x000163, "AMCAP-F2",         "AM-capability for function 2"         );
  displayItem( rom, 8, 0x000183, "AMCAP-F3",         "AM-capability for function 3"         );
  displayItem( rom, 8, 0x0001a3, "AMCAP-F4",         "AM-capability for function 4"         );
  displayItem( rom, 8, 0x0001c3, "AMCAP-F5",         "AM-capability for function 5"         );
  displayItem( rom, 8, 0x0001e3, "AMCAP-F6",         "AM-capability for function 6"         );
  displayItem( rom, 8, 0x000203, "AMCAP-F7",         "AM-capability for function 7"         );

  displayItem( rom,16, 0x000223, "XAMCAP-F0-low",   "XAM-capability for function 0 low"         );
  displayItem( rom,16, 0x000263, "XAMCAP-F0-high",   "XAM-capability for function 0 high"         );
  displayItem( rom,16, 0x0002a3, "XAMCAP-F1-low",   "XAM-capability for function 0 low"         );
  displayItem( rom,16, 0x0002e3, "XAMCAP-F1-high",   "XAM-capability for function 0 high"         );
  displayItem( rom,16, 0x000323, "XAMCAP-F2-low",   "XAM-capability for function 0 low"         );
  displayItem( rom,16, 0x000363, "XAMCAP-F2-high",   "XAM-capability for function 0 high"         );
  displayItem( rom,16, 0x0003a3, "XAMCAP-F3-low",   "XAM-capability for function 0 low"         );
  displayItem( rom,16, 0x0003e3, "XAMCAP-F3-high",   "XAM-capability for function 0 high"         );
  displayItem( rom,16, 0x000423, "XAMCAP-F4-low",   "XAM-capability for function 0 low"         );
  displayItem( rom,16, 0x000463, "XAMCAP-F4-high",   "XAM-capability for function 0 high"         );
  displayItem( rom,16, 0x0004a3, "XAMCAP-F5-low",   "XAM-capability for function 0 low"         );
  displayItem( rom,16, 0x0004e3, "XAMCAP-F5-high",   "XAM-capability for function 0 high"         );
  displayItem( rom,16, 0x000523, "XAMCAP-F6-low",   "XAM-capability for function 0 low"         );
  displayItem( rom,16, 0x000563, "XAMCAP-F6-high",   "XAM-capability for function 0 high"         );
  displayItem( rom,16, 0x0005a3, "XAMCAP-F7-low",   "XAM-capability for function 0 low"         );
  displayItem( rom,16, 0x0005e3, "XAMCAP-F7-high",   "XAM-capability for function 0 high"         );

  displayItem( rom, 4, 0x000623, "ADEM-F0",            "Address Decoder Mask function 0"      );
  displayItem( rom, 4, 0x000633, "ADEM-F1",            "Address Decoder Mask function 1"      );
  displayItem( rom, 4, 0x000643, "ADEM-F2",            "Address Decoder Mask function 2"      );
  displayItem( rom, 4, 0x000653, "ADEM-F3",            "Address Decoder Mask function 3"      );
  displayItem( rom, 4, 0x000663, "ADEM-F4",            "Address Decoder Mask function 4"      );
  displayItem( rom, 4, 0x000673, "ADEM-F5",            "Address Decoder Mask function 5"      );
  displayItem( rom, 4, 0x000683, "ADEM-F6",            "Address Decoder Mask function 6"      );
  displayItem( rom, 4, 0x000693, "ADEM-F7",            "Address Decoder Mask function 7"      );

  displayItem( rom, 1, 0x0006af, "MasterDataWidth",    "Master data access Width"      );
  displayItem( rom, 8, 0x0006b3, "MasterAMCAP",        "Master AM Capapbilities"      );
  displayItem( rom,16, 0x0006d3, "MasterXAMCAP-low",   "Master CAM Capapbilities"      );
  displayItem( rom,16, 0x000713, "MasterXAMCAP-high",  "Master CAM Capapbilities"      );
}

void HAL::ConfigROMDumper::displayItem( const std::vector<unsigned char>rom, 
					const uint32_t length,
					const uint32_t address,
					const std::string key,
					const std::string description ) {
  std::cout << std::hex << std::setw(8) << std::setfill('0') << address << " : " 
       << std::setw(18) << std::setfill(' ') << key << " : ";

  uint32_t ix = (address-3) / 4;
  for ( uint32_t ic = 0; ic < length; ic++ ) {
    std::cout << std::setw(2) << std::setfill('0') << std::hex << ((uint32_t) rom[ix])
	 << " ";
    ix ++;
  }

  uint32_t pad = (17-length) * 3;
  std::cout << std::setw(pad) << std::setfill(' ') << " : " << description << std::endl;   
}
