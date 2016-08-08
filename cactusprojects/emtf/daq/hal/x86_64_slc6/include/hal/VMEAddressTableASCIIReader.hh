#ifndef __VMEAddressTableASCIIReader
#define __VMEAddressTableASCIIReader

#include <string>

#include "hal/NoSuchFileException.hh"
#include "hal/AddressTableReader.hh"
#include "hal/VMEHardwareAddress.hh"
#include "hal/VME64xHardwareAddress.hh"

namespace HAL {

/**
*
*
*     @short A class to read in an ASCII file with an 
*            VME AddressTable description.
*
*            The format of the file  must be strictly according to the
*            following two patterns   (lines  starting  with   a  "*"  are 
*            considered as comments). The first pattern describes the format
*            for standard VME Modules:
*
*            <pre>
*
*            *****************************************************************************************************************
*            *     key              AM  width  address      mask    read write description
*            *****************************************************************************************************************
*            DaqActive              39    2   00000000    00000001    1    1   This let data flow...
*            BxCount                39    2   00000000    00000004    1    1   sets the dummy data generator in contineous
*            *                                                                 count mode (100MHz)
*            TriggerSourceSelector  39    2   00000000    00000018    1    1   00 : selects software trigger
*            *                                                                 01 : selects external trigger
*            *                                                                 10 : selects internal trigger generator
*            *                                                                 11 : no trigger
*            DescriptorFifoCount    39    4   00000008    000001ff    1    0   number of words currently in the DescriptorFifo
*            Busy                   39    4   00000008    00010000    1    0   indicates that currently an event is processed
*            TriggerLevelReadback   39    4   00000008    00020000    1    0   reads back the level of the trigger line
*            *                                                                ( can be used to check the external trigger )
*            PagesAvailable         39    4   00000008    00040000    1    0   indicates if there is space for more events
*            DescriptorFifoQUnvalid 39    4   00000008    00080000    1    0   the next read of the DescriptorFifo does NOT give
*            PendingTriggerCounter  39    4   00000008    ff000000    1    0   number of pending triggers not yet treated
*            *                                                                 valid data (occurs after fifo has been empty)
*            DescriptorFifoCountQ   3a    4   00000010    0000ffff    1    0   read the Descriptorfifo and afterwards increment
*            *                                                                 readpointer
*            EventSize              3a    4   00000018    0000ffff    1    1   size of generated events in bytes
*            *                               
*            resetRui               3a    4   00000020    00000001    0    1   resets the logic of the RUI
*            flushData              3a    4   00000020    00000002    0    1   flushes all data from the RUI (including RAM)
*            Trigger                3a    4   00000020    00000004    0    1   triggers the generation of one event
*            clearDataGenerator     3a    4   00000020    00000008    0    1   clears the counter which generates dummy data
*            FreeRequest            3a    4   00000020    00000010    0    1   request to free one memory block
*            readData               3a    4   00000028    ffffffff    1    0   reads a data word and increment data counter (if not
*            *                                                                 in contineous mode anyway) used by DMA to SDRAM
*            TriggerCounterRate     3a    4   00000030    ffffffff    1    1   number of 10ns intervals between two triggers
*            **********************************************************************************************************
*
*            </pre>
*
*            The second pattern describes the format for VME64x Modules. The column map/width has a double meaning:
*            In case the item is a memory space item it is indicating the memory map in which the item is mapped. In
*            case of configuration items there are no multiple memory maps available. On the other hand the width of 
*            item must be specified since it is not known to the host. (Remeber: for items in the memory space the 
*            data width is chosen by the host during configuration of the module.)
*            <pre>
*
*            ********************************************************************************************************
*            *     key              space        map/width  address   mask    read write description
*            ********************************************************************************************************
*
*            PrivateConfigItem      configuration    1     00001100 000000ff    1    1   This is a 1 byte custom configuration item
*            AnotherOne             configuration    2     00001100 0000ffff    1    1   This is a 2 byte custom configuration item
*
*            DaqActive              memory           0     00000000 00000001    1    1   This let data flow...
*            BxCount                memory           0     00000000 00000004    1    1   sets the dummy data generator in contineous
*            *                                                                           count mode (100MHz)
*            TriggerSourceSelector  memory           0     00000000 00000018    1    1   00 : selects software trigger
*            *                                                                           01 : selects external trigger
*            *                                                                           10 : selects internal trigger generator
*            *                                                                           11 : no trigger
*            DescriptorFifoCount    memory           0     00000008 000001ff    1    0   number of words currently in the DescriptorFifo
*            Busy                   memory           0     00000008 00010000    1    0   indicates that currently an event is processed
*            TriggerLevelReadback   memory           0     00000008 00020000    1    0   reads back the level of the trigger line
*            *                                                                           ( can be used to check the external trigger )
*            PagesAvailable         memory           0     00000008 00040000    1    0   indicates if there is space for more events
*            DescriptorFifoQUnvalid memory           0     00000008 00080000    1    0   the next read of the DescriptorFifo does NOT give
*            PendingTriggerCounter  memory           0     00000008 ff000000    1    0   number of pending triggers not yet treated
*            *                                                                           valid data (occurs after fifo has been empty)
*            DescriptorFifoCountQ   memory           1     00000000 0000ffff    1    0   read the Descriptorfifo and afterwards increment
*            *                                                                           readpointer
*            EventSize              memory           1     00000008 0000ffff    1    1   size of generated events in bytes
*            *                                         	   
*            resetRui               memory           0     00000020 00000001    0    1   resets the logic of the RUI
*            flushData              memory           0     00000020 00000002    0    1   flushes all data from the RUI (including RAM)
*            Trigger                memory           0     00000020 00000004    0    1   triggers the generation of one event
*            clearDataGenerator     memory           0     00000020 00000008    0    1   clears the counter which generates dummy data
*            FreeRequest            memory           0     00000020 00000010    0    1   request to free one memory block
*            readData               memory           0     00000028 ffffffff    1    0   reads a data word and increment data counter (if not
*            *                                                                           in contineous mode anyway) used by DMA to SDRAM
*            TriggerCounterRate     memory           0     00000030 ffffffff    1    1   number of 10ns intervals between two triggers
*            *************************************************************************************************
*
*            </pre>
*            
*       @see AddressTable, AddressTableInterface            
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class VMEAddressTableASCIIReader : public AddressTableReader {

public:
  VMEAddressTableASCIIReader( std::string fileName ) 
    throw (NoSuchFileException) ;

private:
  AddressSpace convertAddressSpaceStr( std::string addressSpaceStr ) 
    throw( HAL::IllegalValueException );
};

} /* namespace HAL */

#endif /* __VMEAddressTableASCIIReader */
