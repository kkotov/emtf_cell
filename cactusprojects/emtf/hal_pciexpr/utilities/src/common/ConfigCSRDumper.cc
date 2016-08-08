#include "hal/ConfigCSRDumper.hh"

#include <iostream>
#include <iomanip>

HAL::ConfigCSRDumper::ConfigCSRDumper() {
}

void HAL::ConfigCSRDumper::dumpCSR( const VMEConfigurationSpaceHandler& configHandler, 
				    const uint32_t slotId ) {
  
  std::vector<unsigned char> csr;
  uint32_t adr = 0x7ff63;
  uint32_t result;

  for ( uint32_t offset = 0; adr <= 0x7ffff; offset += 4, adr+=4 ) {
    configHandler.configRead( "CSRStart", slotId, &result, offset );
    csr.push_back( (unsigned char)result );
  }

  displayItem( csr, 1, 0x07ffff, "bar",          "CR/CSR Base Address Register (BAR)");         
  displayItem( csr, 1, 0x07fffb, "bitSet",       "Bit Set Register");                           
  displayItem( csr, 1, 0x07fff7, "bitClear",     "Bit Clear Register");                         
  displayItem( csr, 1, 0x07fff3, "cramOwner",    "CRAM owner Register");                        
  displayItem( csr, 1, 0x07ffef, "userBitSet",   "User Bit Set Register");                      
  displayItem( csr, 1, 0x07ffeb, "userBitClear", "User Bit Clear Register");                    
		                 
  displayItem( csr, 4, 0x07ffd3, "ADER-F7",      "Address Decoder Compare Register function 7");
  displayItem( csr, 4, 0x07ffc3, "ADER-F6",      "Address Decoder Compare Register function 6");
  displayItem( csr, 4, 0x07ffb3, "ADER-F5",      "Address Decoder Compare Register function 5");
  displayItem( csr, 4, 0x07ffa3, "ADER-F4",      "Address Decoder Compare Register function 4");
  displayItem( csr, 4, 0x07ff93, "ADER-F3",      "Address Decoder Compare Register function 3");
  displayItem( csr, 4, 0x07ff83, "ADER-F2",      "Address Decoder Compare Register function 2");
  displayItem( csr, 4, 0x07ff73, "ADER-F1",      "Address Decoder Compare Register function 1");
  displayItem( csr, 4, 0x07ff63, "ADER-F0",      "Address Decoder Compare Register function 0");
}

void HAL::ConfigCSRDumper::displayItem( const std::vector<unsigned char>csr, 
					const uint32_t length,
					const uint32_t address,
					const std::string key,
					const std::string description ) {
  std::cout << std::hex << std::setw(8) << std::setfill('0') << address << " : " 
       << std::setw(18) << std::setfill(' ') << key << " : ";

  uint32_t ix = (address-0x7ff63) / 4;
  for ( uint32_t ic = 0; ic < length; ic++ ) {
    std::cout << std::setw(2) << std::setfill('0') << std::hex << ((uint32_t) csr[ix])
	 << " ";
    ix ++;
  }

  uint32_t pad = (5-length) * 3;
  std::cout << std::setw(pad) << std::setfill(' ') << " : " << description << std::endl;   
}
