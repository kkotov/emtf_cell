#include "hal/VMEConfigurationSpaceAddressReader.hh"

/**
 * The implementation of this class has been chosen to increase
 * readability of the code.
 */
HAL::VMEConfigurationSpaceAddressReader::VMEConfigurationSpaceAddressReader() {
  /****************************************************************************************************/
  /*          key                   width address   r  w  description                                 */
  /****************************************************************************************************/
  createItem( "romStart",             1,   0x000000, 1, 0, "technical helper item: pointers refer to this startaddress");
  createItem( "configSpaceEnd",       1,   0xffffff, 1, 1, "technical helper item:in order to access all slots with offsets");
  createItem( "checksum",             1,   0x000003, 1, 0, "checksum of configuration ROM"); 
				          
  createItem( "romLength",            3,   0x000007, 1, 0, "length of the Configuration ROM");
  createItem( "CRWidth",              1,   0x000013, 1, 0, "Configuration ROM (CR)  access width"); 
  createItem( "CSRWidth",             1,   0x000017, 1, 0, "Configuration Status Reg (CSR) access width"); 
  createItem( "specificationId",      1,   0x00001b, 1, 0, "CR/CSR specification ID"); 
  createItem( "CR",                   2,   0x00001f, 1, 0, "to identify valid CR");              
				          
  createItem( "manufacturerId",       3,   0x000027, 1, 0, "Manufacturer Id"); 
  createItem( "boardId",              4,   0x000033, 1, 0, "Board Id (from manufacturer)"); 
  createItem( "revisionId",           4,   0x000043, 1, 0, "Board-Revision Id (from manufacturer)"); 
  createItem( "descriptionPtr",       3,   0x000053, 1, 0, "Pointer to Null terminated string"); 
  createItem( "programId",            1,   0x00007f, 1, 0, "Program Id (if available)"); 
  createItem( "userCRStart",          3,   0x000083, 1, 0, "0 or Pointer to start of User CR"); 
  createItem( "userCREnd",            3,   0x00008f, 1, 0, "0 or Pointer to end of User CR"); 
  createItem( "CRAMStart",            3,   0x00009b, 1, 0, "0 or Pointer to start of CRAM"); 
  createItem( "CRAMEnd",              3,   0x0000a7, 1, 0, "0 or Pointer to end of CRAM"); 
  createItem( "userCSRStart",         3,   0x0000b3, 1, 0, "0 or Pointer to start of User CSR"); 
  createItem( "userCSREnd",           3,   0x0000bf, 1, 0, "0 or Pointer to end of User CSR"); 
  createItem( "serialNumberStart",    3,   0x0000cb, 1, 0, "0 or Pointer to start of Serialnumber"); 
  createItem( "serialNumberEnd",      3,   0x0000d7, 1, 0, "0 or Pointer to end of Serialnumber"); 


  createItem( "slaveCharacts",        1,   0x0000e3, 1, 0, "characteristics of slave interface"  );
  createItem( "userSlaveCharacts",    1,   0x0000e7, 1, 0, "user defined characteristics of slave interface");
  createItem( "masterCharcts",        1,   0x0000eb, 1, 0, "characteristics of master interface"  );
  createItem( "userMasterCharacts",   1,   0x0000ef, 1, 0, "user defined characteristics of master interface"  );
  createItem( "irqHandlerCap",        1,   0x0000f3, 1, 0, "interrupt handler capabilities"  );
  createItem( "interrupterCap",       1,   0x0000f7, 1, 0, "interrupter capabilities"  );
  createItem( "CRAMWidth",            1,   0x0000FF, 1, 0, "Configuration RAM (CRAM) access width"); 

  createItem( "dataAccessWidth-F0",   1,   0x000103, 1, 0, "Data access width for function 0"); 
  createItem( "dataAccessWidth-F1",   1,   0x000107, 1, 0, "Data access width for function 1"); 
  createItem( "dataAccessWidth-F2",   1,   0x00010b, 1, 0, "Data access width for function 2"); 
  createItem( "dataAccessWidth-F3",   1,   0x00010f, 1, 0, "Data access width for function 3"); 
  createItem( "dataAccessWidth-F4",   1,   0x000113, 1, 0, "Data access width for function 4"); 
  createItem( "dataAccessWidth-F5",   1,   0x000117, 1, 0, "Data access width for function 5"); 
  createItem( "dataAccessWidth-F6",   1,   0x00011b, 1, 0, "Data access width for function 6"); 
  createItem( "dataAccessWidth-F7",   1,   0x00011f, 1, 0, "Data access width for function 7"); 
				       
  createItem( "AMCAP-F0-1",           4,   0x000123, 1, 0, "AM-capability for function 0: byte 7"); 
  createItem( "AMCAP-F0-0",           4,   0x000133, 1, 0, "AM-capability for function 0: byte 3"); 
  createItem( "AMCAP-F1-1",           4,   0x000143, 1, 0, "AM-capability for function 1: byte 7"); 
  createItem( "AMCAP-F1-0",           4,   0x000153, 1, 0, "AM-capability for function 1: byte 3"); 
  createItem( "AMCAP-F2-1",           4,   0x000163, 1, 0, "AM-capability for function 2: byte 7"); 
  createItem( "AMCAP-F2-0",           4,   0x000173, 1, 0, "AM-capability for function 2: byte 3"); 
  createItem( "AMCAP-F3-1",           4,   0x000183, 1, 0, "AM-capability for function 3: byte 7"); 
  createItem( "AMCAP-F3-0",           4,   0x000293, 1, 0, "AM-capability for function 3: byte 3"); 
  createItem( "AMCAP-F4-1",           4,   0x0001a3, 1, 0, "AM-capability for function 4: byte 7"); 
  createItem( "AMCAP-F4-0",           4,   0x0001b3, 1, 0, "AM-capability for function 4: byte 3"); 
  createItem( "AMCAP-F5-1",           4,   0x0001c3, 1, 0, "AM-capability for function 5: byte 7"); 
  createItem( "AMCAP-F5-0",           4,   0x0001d3, 1, 0, "AM-capability for function 5: byte 3"); 
  createItem( "AMCAP-F6-1",           4,   0x0001e3, 1, 0, "AM-capability for function 6: byte 7"); 
  createItem( "AMCAP-F6-0",           4,   0x0001f3, 1, 0, "AM-capability for function 6: byte 3"); 
  createItem( "AMCAP-F7-1",           4,   0x000203, 1, 0, "AM-capability for function 7: byte 7"); 
  createItem( "AMCAP-F7-0",           4,   0x000213, 1, 0, "AM-capability for function 7: byte 3"); 
				       
  createItem( "XAMCAP-F0-7",          4,   0x000223, 1, 0, "XAM-capability for function 0: word 7"); 
  createItem( "XAMCAP-F1-7",          4,   0x0002a3, 1, 0, "XAM-capability for function 1: word 7"); 
  createItem( "XAMCAP-F2-7",          4,   0x000323, 1, 0, "XAM-capability for function 2: word 7"); 
  createItem( "XAMCAP-F3-7",          4,   0x0003a3, 1, 0, "XAM-capability for function 3: word 7"); 
  createItem( "XAMCAP-F4-7",          4,   0x000423, 1, 0, "XAM-capability for function 4: word 7"); 
  createItem( "XAMCAP-F5-7",          4,   0x0004a3, 1, 0, "XAM-capability for function 5: word 7"); 
  createItem( "XAMCAP-F6-7",          4,   0x000523, 1, 0, "XAM-capability for function 6: word 7"); 
  createItem( "XAMCAP-F7-7",          4,   0x0005a3, 1, 0, "XAM-capability for function 7: word 7"); 
				       
  createItem( "XAMCAP-F0-6",          4,   0x000233, 1, 0, "XAM-capability for function 0: word 6"); 
  createItem( "XAMCAP-F1-6",          4,   0x0002b3, 1, 0, "XAM-capability for function 1: word 6"); 
  createItem( "XAMCAP-F2-6",          4,   0x000333, 1, 0, "XAM-capability for function 2: word 6"); 
  createItem( "XAMCAP-F3-6",          4,   0x0003b3, 1, 0, "XAM-capability for function 3: word 6"); 
  createItem( "XAMCAP-F4-6",          4,   0x000433, 1, 0, "XAM-capability for function 4: word 6"); 
  createItem( "XAMCAP-F5-6",          4,   0x0004b3, 1, 0, "XAM-capability for function 5: word 6"); 
  createItem( "XAMCAP-F6-6",          4,   0x000533, 1, 0, "XAM-capability for function 6: word 6"); 
  createItem( "XAMCAP-F7-6",          4,   0x0005b3, 1, 0, "XAM-capability for function 7: word 6"); 
				       							   
  createItem( "XAMCAP-F0-5",          4,   0x000243, 1, 0, "XAM-capability for function 0: word 5"); 
  createItem( "XAMCAP-F1-5",          4,   0x0002c3, 1, 0, "XAM-capability for function 1: word 5"); 
  createItem( "XAMCAP-F2-5",          4,   0x000343, 1, 0, "XAM-capability for function 2: word 5"); 
  createItem( "XAMCAP-F3-5",          4,   0x0003c3, 1, 0, "XAM-capability for function 3: word 5"); 
  createItem( "XAMCAP-F4-5",          4,   0x000443, 1, 0, "XAM-capability for function 4: word 5"); 
  createItem( "XAMCAP-F5-5",          4,   0x0004c3, 1, 0, "XAM-capability for function 5: word 5"); 
  createItem( "XAMCAP-F6-5",          4,   0x000543, 1, 0, "XAM-capability for function 6: word 5"); 
  createItem( "XAMCAP-F7-5",          4,   0x0005c3, 1, 0, "XAM-capability for function 7: word 5"); 
				       							   
  createItem( "XAMCAP-F0-4",          4,   0x000253, 1, 0, "XAM-capability for function 0: word 4"); 
  createItem( "XAMCAP-F1-4",          4,   0x0002d3, 1, 0, "XAM-capability for function 1: word 4"); 
  createItem( "XAMCAP-F2-4",          4,   0x000353, 1, 0, "XAM-capability for function 2: word 4"); 
  createItem( "XAMCAP-F3-4",          4,   0x0003d3, 1, 0, "XAM-capability for function 3: word 4"); 
  createItem( "XAMCAP-F4-4",          4,   0x000453, 1, 0, "XAM-capability for function 4: word 4"); 
  createItem( "XAMCAP-F5-4",          4,   0x0004d3, 1, 0, "XAM-capability for function 5: word 4"); 
  createItem( "XAMCAP-F6-4",          4,   0x000553, 1, 0, "XAM-capability for function 6: word 4"); 
  createItem( "XAMCAP-F7-4",          4,   0x0005d3, 1, 0, "XAM-capability for function 7: word 4"); 
				       							   
  createItem( "XAMCAP-F0-3",          4,   0x000263, 1, 0, "XAM-capability for function 0: word 3"); 
  createItem( "XAMCAP-F1-3",          4,   0x0002e3, 1, 0, "XAM-capability for function 1: word 3"); 
  createItem( "XAMCAP-F2-3",          4,   0x000363, 1, 0, "XAM-capability for function 2: word 3"); 
  createItem( "XAMCAP-F3-3",          4,   0x0003e3, 1, 0, "XAM-capability for function 3: word 3"); 
  createItem( "XAMCAP-F4-3",          4,   0x000463, 1, 0, "XAM-capability for function 4: word 3"); 
  createItem( "XAMCAP-F5-3",          4,   0x0004e3, 1, 0, "XAM-capability for function 5: word 3"); 
  createItem( "XAMCAP-F6-3",          4,   0x000563, 1, 0, "XAM-capability for function 6: word 3"); 
  createItem( "XAMCAP-F7-3",          4,   0x0005e3, 1, 0, "XAM-capability for function 7: word 3"); 
			  	       							   
  createItem( "XAMCAP-F0-2",          4,   0x000273, 1, 0, "XAM-capability for function 0: word 2"); 
  createItem( "XAMCAP-F1-2",          4,   0x0002f3, 1, 0, "XAM-capability for function 1: word 2"); 
  createItem( "XAMCAP-F2-2",          4,   0x000373, 1, 0, "XAM-capability for function 2: word 2"); 
  createItem( "XAMCAP-F3-2",          4,   0x0003f3, 1, 0, "XAM-capability for function 3: word 2"); 
  createItem( "XAMCAP-F4-2",          4,   0x000473, 1, 0, "XAM-capability for function 4: word 2"); 
  createItem( "XAMCAP-F5-2",          4,   0x0004f3, 1, 0, "XAM-capability for function 5: word 2"); 
  createItem( "XAMCAP-F6-2",          4,   0x000573, 1, 0, "XAM-capability for function 6: word 2"); 
  createItem( "XAMCAP-F7-2",          4,   0x0005f3, 1, 0, "XAM-capability for function 7: word 2"); 
			  	       							  
  createItem( "XAMCAP-F0-1",          4,   0x000283, 1, 0, "XAM-capability for function 0: word 1"); 
  createItem( "XAMCAP-F1-1",          4,   0x000303, 1, 0, "XAM-capability for function 1: word 1"); 
  createItem( "XAMCAP-F2-1",          4,   0x000383, 1, 0, "XAM-capability for function 2: word 1"); 
  createItem( "XAMCAP-F3-1",          4,   0x000403, 1, 0, "XAM-capability for function 3: word 1"); 
  createItem( "XAMCAP-F4-1",          4,   0x000483, 1, 0, "XAM-capability for function 4: word 1"); 
  createItem( "XAMCAP-F5-1",          4,   0x000503, 1, 0, "XAM-capability for function 5: word 1"); 
  createItem( "XAMCAP-F6-1",          4,   0x000583, 1, 0, "XAM-capability for function 6: word 1"); 
  createItem( "XAMCAP-F7-1",          4,   0x000603, 1, 0, "XAM-capability for function 7: word 1"); 
			  	       							   
  createItem( "XAMCAP-F0-0",          4,   0x000293, 1, 0, "XAM-capability for function 0: word 0"); 
  createItem( "XAMCAP-F1-0",          4,   0x000313, 1, 0, "XAM-capability for function 1: word 0"); 
  createItem( "XAMCAP-F2-0",          4,   0x000393, 1, 0, "XAM-capability for function 2: word 0"); 
  createItem( "XAMCAP-F3-0",          4,   0x000413, 1, 0, "XAM-capability for function 3: word 0"); 
  createItem( "XAMCAP-F4-0",          4,   0x000493, 1, 0, "XAM-capability for function 4: word 0"); 
  createItem( "XAMCAP-F5-0",          4,   0x000513, 1, 0, "XAM-capability for function 5: word 0"); 
  createItem( "XAMCAP-F6-0",          4,   0x000593, 1, 0, "XAM-capability for function 6: word 0"); 
  createItem( "XAMCAP-F7-0",          4,   0x000613, 1, 0, "XAM-capability for function 7: word 0"); 
				       
  createItem( "ADEM-F0",              4,   0x000623, 1, 0, "Address Decoder Mask function 0"); 
  createItem( "ADEM-F1",              4,   0x000633, 1, 0, "Address Decoder Mask function 1"); 
  createItem( "ADEM-F2",              4,   0x000643, 1, 0, "Address Decoder Mask function 2"); 
  createItem( "ADEM-F3",              4,   0x000653, 1, 0, "Address Decoder Mask function 3"); 
  createItem( "ADEM-F4",              4,   0x000663, 1, 0, "Address Decoder Mask function 4"); 
  createItem( "ADEM-F5",              4,   0x000673, 1, 0, "Address Decoder Mask function 5"); 
  createItem( "ADEM-F6",              4,   0x000683, 1, 0, "Address Decoder Mask function 6"); 
  createItem( "ADEM-F7",              4,   0x000693, 1, 0, "Address Decoder Mask function 7"); 

  /**************************************** CSR items *************************************************/

  createItem("bar",                   1,   0x07ffff, 1, 1, "CR/CSR Base Address Register (BAR)");
  createItem("bitSet",                1,   0x07fffb, 1, 1, "Bit Set Register");
  createItem("bitClear",              1,   0x07fff7, 1, 1, "Bit Clear Register");
  createItem("cramOwner",             1,   0x07fff3, 1, 1, "CRAM owner Register");
  createItem("userBitSet",            1,   0x07ffef, 1, 1, "User Bit Set Register");
  createItem("userBitClear",          1,   0x07ffeb, 1, 1, "User Bit Clear Register");
				         	   
  createItem("ADER-F7",               4,   0x07ffd3, 1, 1, "Address Decoder Compare Register function 7");
  createItem("ADER-F6",               4,   0x07ffc3, 1, 1, "Address Decoder Compare Register function 6");
  createItem("ADER-F5",               4,   0x07ffb3, 1, 1, "Address Decoder Compare Register function 5");
  createItem("ADER-F4",               4,   0x07ffa3, 1, 1, "Address Decoder Compare Register function 4");
  createItem("ADER-F3",               4,   0x07ff93, 1, 1, "Address Decoder Compare Register function 3");
  createItem("ADER-F2",               4,   0x07ff83, 1, 1, "Address Decoder Compare Register function 2");
  createItem("ADER-F1",               4,   0x07ff73, 1, 1, "Address Decoder Compare Register function 1");
  createItem("ADER-F0",               4,   0x07ff63, 1, 1, "Address Decoder Compare Register function 0");
  createItem("CSRStart",              1,   0x07ff63, 1, 1, "Marks start of CSR space");


 /****************************************************************************************************/
}

void HAL::VMEConfigurationSpaceAddressReader::createItem( std::string key, 
							  uint32_t width,
							  uint32_t address, 
							  uint32_t isReadable, 
							  uint32_t isWritable, 
							  std::string description) {

  HAL::VMEHardwareAddress* addressPointer = new HAL::VMEHardwareAddress(address,0x2f,width);
  HAL::AddressTableItem* itemPointer = new HAL::AddressTableItem(key, description, *addressPointer, 
								 0xffffffff, isWritable, isReadable);  
  itemPointerList.push_back( itemPointer );
}
