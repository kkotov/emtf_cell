#ifndef __VMEDummyBusAdapter
#define __VMEDummyBusAdapter

#include <iostream>
#include "hal/VMEBusAdapterInterface.hh"
#include "hal/VMEDummyDeviceIdentifier.hh"


namespace HAL {

/**
 *
 *
 *     @short A dummy VMEBusAdapter in order to work without hardware.
 *            
 *            This  BusAdapter  merely  serves  to  let  you  test  you 
 *            software  without actually  having hardware  connected to 
 *            your  computer.  All actions  which  in  real life  would 
 *            result in  read or write  operations will be  reported on 
 *            the standard output.
 *
 *       @see BusAdapterInterface
 *    @author Christoph Schwick
 * $Revision: 1.1 $
 *     $Date: 2007/03/06 09:54:25 $
 *
 *
 **/
class VMEDummyBusAdapter : public VMEBusAdapterInterface {

public:

    /**
     * This enumerator is used in the constructor. It switches 
     * the versbose behaviour of the BusAdapter on or off. Note
     * that the enumerator is a member of the class PCIDummyBusAdapter.
     * So it must be used. This means that a Scope identifier is
     * needed when calling the constructor of the class. A valid 
     * example would be:
     * <pre>
     * PCIDummyBusAdapter myBa( PCIDummyBusAdapter::VERBOSE_ON,
     *                          PCIDummyBusAdapter::MEMORY_MAP_OFF );
     * </pre>
     * The enumerators have been put into the class in order not
     * to contaminate the global namesapce with objects merely used 
     * for debugging purposes.
     */
    enum VerboseMode { VERBOSE_OFF, VERBOSE_ON };

    /**
     * This enumerator is used in the constructor. It switches 
     * the memory-mapping behaviour of the BusAdapter on or off. 
     * If switched on every address-space  corresponding to a 
     * specific BAR or to the configuration space is mapped into
     * the local memory of the host. It is then used to perform
     * read and write operations into the mapped memory. This 
     * allows to play in a more realistic way with the device.
     * See VerboseMode comments for a note on how to use this 
     * enumerator.
     */
    enum MemoryMode  { MEMORY_MAP_OFF, MEMORY_MAP_ON };

    /**
     * The constructor takes two arguments:
     * @param verbose allows you to monitor all data transfers: If 
     *        set to true every action is reported on the standard
     *        output.
     * @param memoryMode allows you to create memory regions which
     *        fake the hardware device. For every openDevice call
     *        a memory region is created into which or from which 
     *        data is transfered. With this you can write and read
     *        back data from you dummy-device. The size of the 
     *        mapped region depend on the VMEAddressTable given
     *        to the openDevice call. 
     */
    VMEDummyBusAdapter( enum VerboseMode verbose    = VERBOSE_ON,
                        enum MemoryMode  memoryMode = MEMORY_MAP_ON,
                        std::ostream& os = std::cout );

    /**
     * There is nothing to do in the desctructor.
     */
    virtual ~VMEDummyBusAdapter();


    /**
     * If the VMEDummyBusAdapter was created with the memoryMode
     * option set to true, the call scans the VMEAddressTable in
     * order to find out how much memory has to be reserved for 
     * the dummy Hardware device. The pointer to this memory is 
     * then contained in the deviceIdentifier. 
     */
    void openDevice( const VMEAddressTable& VMEAddressTable,
                     uint32_t vmeBaseaddress,
                     DeviceIdentifier** deviceIdentifierPtr,
                     uint32_t* baseAddressPtr,
                     bool doSwapping = false )
        throw();
  
    void openDevice( const VMEAddressTable& vmeAddressTable,
                     std::vector<uint32_t>& vmeBaseaddresses,
                     DeviceIdentifier** deviceIdentifierPtr,
                     std::vector<uint32_t>* baseAddressPtr,
                     bool doSwapping = 0) 
        throw();
    /**
     * Here the memory regions which might have beeen reserved
     * in the openDevice call, are deleted.
     */
    void closeDevice( DeviceIdentifier* deviceIdentifier )
        throw();
  
    void write( DeviceIdentifier* deviceIdentifier,
                uint32_t address,
                uint32_t addressModifier,
                uint32_t dataWidth,
                uint32_t data)
        throw();
  
    void read( DeviceIdentifier* DeviceIdentifier,
               uint32_t address,
               uint32_t addressModifier,
               uint32_t dataWidth,
               uint32_t* result)
        throw();

    void writeBlock( DeviceIdentifier* deviceIdentifierPtr,
                     uint32_t startAddress,
                     uint32_t length,      // in bytes
                     uint32_t addressModifier,
                     uint32_t dataWidth,
                     char *buffer,
                     HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT )
        throw();
  
    void readBlock( DeviceIdentifier* deviceIdentifierPtr,
                    uint32_t startAddress,
                    uint32_t length,      // in bytes
                    uint32_t addressModifier,
                    uint32_t dataWidth,
                    char *buffer,
                    HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT )
        throw();

    void resetBus( )
        throw();
  
private:
    uint32_t doSwap( uint32_t data, uint32_t dataWidth ) const;
    std::ostream& os_;
    uint32_t deviceNumberCounter;
    enum VerboseMode verbose;
    enum MemoryMode  memoryMode;
};

} /* namespace HAL */

#endif /* __VMEDummyBusAdapter */
