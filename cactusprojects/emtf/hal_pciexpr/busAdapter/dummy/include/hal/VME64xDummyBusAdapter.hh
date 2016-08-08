#ifndef __VME64xDummyBusAdapter
#define __VME64xDummyBusAdapter

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <tr1/unordered_map>
#include "hal/NoSuchFileException.hh"
#include "hal/VMEBusAdapterInterface.hh"
#include "hal/VMEDummyDeviceIdentifier.hh"
#include "hal/ConfigurationSpaceFileReader.hh"

namespace HAL {

/**
*
*
*     @short A dummy VME64xBusAdapter in order to work without hardware.
*            
*            This  BusAdapter  merely  serves  to test  your 
*            software  without actually  having hardware  connected to 
*            your  computer.  All actions  which  in  real life  would 
*            result in  read or write  operations will be  reported on 
*            the standard output.
*            
*            The   VME64xDummyBusAdapter   is   different   from   the 
*            VMEDummyBusAdapter   in  that   it   also  emulates   the 
*            configuration space of a VME64x  crate. In order to so so
*            it needs  a memory map  of the configuration  space. This 
*            is build up in two  steps. A CR description file given to
*            the construtor  contains a simple ASCII  table where each 
*            line  contains a slot  number and  a filename.  This file 
*            then  contains  the  CR   space  of  the  module  in  the 
*            corresponding slot. 
*            
*            Internally the  CR and CSR  space are contained in  a stl 
*            map  with the  address as  a key.  This method  makes the 
*            implementation   independent   of   the   size   of   the 
*            configuration  space.  (Only  really used  addresses  are 
*            entered in  the map.) This  mechanism works independently 
*            of the setting for the MemoryMode.
*
*       @see BusAdapterInterface
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/06 09:54:25 $
*
*
**/

class VME64xDummyBusAdapter : public VMEBusAdapterInterface {

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
   * The enumerators have been put inside the class in order not
   * to contaminate the global namespace with objects merely used 
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
   * The constructor of the VME64xDummyBusAdapter.
   * The constructor needs a file which describes the layout of 
   * the CR/CSR space of the crate. The file format is as follows:
   * Lines beginning with # are comments. All other lines must 
   * contain lines of the format:
   * <pre>
   * slotId {path to CR/CSR description file}
   * </pre>
   * The format of the CR/CSR description file is documented in the
   * ConfigurationSpaceFileReader. Examples can be found in the 
   * directory hal/XDAQTools/classtesters/data.
   *
   * @see ConfigurationSpaceFileReader
   * @param configSpaceDefinitionFile is the full path to a file
   *        containing the association of slotId to CR/CSR memory map
   *        description file. 
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
  VME64xDummyBusAdapter( std::string configSpaceDefinitionFile,
                         enum VerboseMode verbose    = VERBOSE_ON,
                         enum MemoryMode  memoryMode = MEMORY_MAP_ON,
			 std::ostream& os = std::cout )
    throw (IllegalValueException,
           NoSuchFileException);
  /**
   * There is nothing to do in the desctructor.
   */
  virtual ~VME64xDummyBusAdapter();


  /**
   * If the VME64xDummyBusAdapter was created with the memoryMode
   * option set to true, the call scans the VMEAddressTable in
   * order to find out how much memory has to be reserved for 
   * the dummy Hardware device. The pointer to this memory is 
   * then contained in the deviceIdentifier. 
   */
  void openDevice( const VMEAddressTable& VMEAddressTable,
                   uint32_t vmeBaseaddress,
                   DeviceIdentifier** deviceIdentifierPtr,
                   uint32_t* baseAddressPtr,
                   bool doSwapping )
    throw();

  /**
   * The method to open a VME64x Device.
   * The method works as openDevice method for standard VME 
   * modules only that now for each mapped memory window 
   * the procedure is repeated.
   */  
  void openDevice( const VMEAddressTable& vmeAddressTable,
		   std::vector<uint32_t> vmeBaseaddresses,
		   DeviceIdentifier** deviceIdentifierPtr,
		   std::vector<uint32_t>* baseAddressPtr,
                   bool doSwapping ) 
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
    throw(BusAdapterException);
  
  void read( DeviceIdentifier* DeviceIdentifier,
             uint32_t address,
             uint32_t addressModifier,
             uint32_t dataWidth,
             uint32_t* result)
    throw(BusAdapterException);

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

  void checkConfigAccess(uint32_t address, uint32_t dataWidth, 
                       uint32_t slotId, uint32_t byte )
  throw (BusAdapterException);

  std::ostream& os_;
  uint32_t deviceNumberCounter;
  enum VerboseMode verbose;
  enum MemoryMode  memoryMode;
    std::vector< std::tr1::unordered_map<uint32_t, uint32_t>* > configSpaceVector_;
};

} /* namespace HAL */

#endif /* __VME64xDummyBusAdapter */
