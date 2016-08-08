#ifndef __PCIAddressTableASCIIReader
#define __PCIAddressTableASCIIReader

#include <string>

#include "hal/NoSuchFileException.hh"
#include "hal/IllegalValueException.hh"
#include "hal/AddressTableReader.hh"
#include "hal/PCIHardwareAddress.hh"
#include "hal/AddressSpace.hh"

namespace HAL {

/**
*
*
*     @short A class to read in an ASCII file with an 
*            AddressTable description.
*
*            The format of the file  must be strictly according to the
*            following  pattern   (lines  starting  with   a  "*"  are 
*            considered as comments) :
*
*            <pre>
*
*            *****************************************************************************************************
*            *    RUI G3	  
*            *    Vendor ID: ECD6 
*            *    Device ID:FED0  
*            *****************************************************************************************************
*            *  key           accessmode  bar  address     mask      read write   description
*            *****************************************************************************************************
*            bar0           configuration      00000010    FFFFFFFF    1    1     standard config-space item
*            bar1           configuration      00000014    FFFFFFFF    1    1     standard config-space item
*            TriggerAddress    memory      0   00000000    FFFFFFFF    1    1     Address to use for l1 and l2 triggers
*            DMAAddress        memory      0   00000004    FFFFFFFF    1    1     destination address of the DMA
*            EndOfEventAddress memory      0   00000008    FFFFFFFF    1    1     address to indicate end of event
*            loadParameters    memory      0   0000000C    FFFFFFFF    1    1     bit[15..0] WC in Words; bit[31..16]seed
*            run               memory      1   00000010    00000001    0    1     '1' = run; '0' = stop
*            resetDevice       memory      1   00000014    00000000    0    1     resets the G3; used with "writePulse"
*            readStatus        memory      1   00000018    FFFFFFFF    1    0     read the status word
*            FifoFullFlag      memory      1   00000018    00000800    1    0     one bit of the status word
*            ControlRegister   memory      1   0000001C    0000FFFF    1    1     set the running conditions (detailed below)
*            someThreshold     memory      1   0000001C    000000FF    1    1     one byte of the control register
*            rate              memory      1   0000001C    00000F00    1    1     another item of the control register
*            idNumber          memory      1   000000!C    0000F000    1    1     sets some constant value
*            *****************************************************************************************************
*            * TriggerAddress   must be MMU BAR0   + 0x00010000  with the current RUM				  
*            * EndOfEventAddres must be DMAAddress + 0x00008000  with the current RUM				  
*            *****************************************************************************************************
*            
*            </pre> 
*            
*            Important note: configuration Space items  must not have a baseaddress 
*            identifier.
*            
*       @see 
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class PCIAddressTableASCIIReader : public AddressTableReader {
public :
  PCIAddressTableASCIIReader( std::string fileName ) throw (HAL::NoSuchFileException, 
							    HAL::IllegalValueException);
};

} /* namespace HAL */

#endif /* __PCIAddressTableASCIIReader */
