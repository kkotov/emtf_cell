#ifndef __VME64xCrate
#define __VME64xCrate

#include <vector>
#include <functional>

#include "hal/VMEDevice.hh"
#include "hal/VMEBusAdapterInterface.hh"
#include "hal/AddressTableContainerInterface.hh"
#include "hal/StaticVMEConfiguration.hh"
#include "hal/ModuleMapperInterface.hh"
#include "hal/VMEConfigurationSpaceHandler.hh"
#include "hal/VMESlot.hh"
#include "hal/HardwareProblemException.hh"
#include "hal/UnsupportedException.hh"

namespace HAL {

/**
*
*
*     @short Browses  the  contents  of   a  crate  and  delivers  the 
*            VME-Modules contained in the crate.
*            
*            This class  should provide all services  which are needed 
*            to  configure  a VME64x  crate  with  the  plug and  play 
*            mechanism. The  result of the configuration is  a list of 
*            VMEModules which can be retrieved by the user. 
*
*       @see 
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class VME64xCrate {
public:

  VME64xCrate( VMEBusAdapterInterface& busAdapter,
               AddressTableContainerInterface& addressTables,
               ModuleMapperInterface& moduleMapper )
  throw( HardwareProblemException,
         IllegalValueException,
	 UnsupportedException );


  /**
   * A constructor for a crate which also contains standard (i.e. non
   * "plug-and-play") VME modules.
   *
   * For every standard VME module a StaticVMEConfiguration has to 
   * be provided. Plug and play modules are then mapped around these
   * standard VME modules which typically have the address-space 
   * defined by jumpers. 
   *
   * During debugging this constructor can also be used to avoid the 
   * automatic configuration of a plug-and-play VME64x module. 
   *
   */
  VME64xCrate( VMEBusAdapterInterface& busAdapter,
               AddressTableContainerInterface& addressTableContainer,
               ModuleMapperInterface& moduleMapper,
               const StaticVMEConfiguration& staticConfiguration ) 
  throw( HardwareProblemException,
         IllegalValueException,
	 UnsupportedException );

  /**
   * Retrieve VMEDevice for a slot.
   *
   * IMPORTANT: the caller of this function becomes owner of the VMEDevice
   * and is responsible for its deletion!
   *
   * @returns A pointer to the VMEDevice or zero if the slot is empty.
   *          It is the responsibility of the caller to check that the 
   *          returned pointer is not 0 before using it.
   */
  VMEDevice* getVMEDevice( uint32_t slotId ) const
    throw ( IllegalOperationException,
	    IllegalValueException,
	    UnsupportedException,
	    BusAdapterException,
	    NoSuchItemException );
 
  /**
   * Retrieve the VMESlot pointer for a given slot.
   *
   * The VME64xCrate remains owner of the VMESlot. The VMESlot is 
   * deleted when the VME64xCrate is destroyed. 
   *
   * @returns A pointer to the VMESlot. This pointer is always valid.
   *          The user must check the contents of the Slot before using 
   *          it with the function VMESlotPtr->getContents().
   *          
   */
  VMESlot* getVMESlot( uint32_t slotId ) const
    throw( IllegalValueException );

  /**
   * Print out the Mapping of the crate.
   */
  void printAddressMap( std::ostream& out ) const;



private:
  /**
   * A helper structure which stores information about an occupied region of the
   * address space.
   */
  struct MappedItem {
    uint32_t startAddress;
    uint32_t endAddress;
    uint32_t baseAddress;
    uint32_t AM;
    VME64xMappedWindow* windowPtr;
    bool configured;
  };

  /**
   * A functor to sort the list of occupied address regions. 
   * This is only used in order to obtain a nicely sorted printout of
   * the mapped address regions.
   */
  class MappedItemSorter {
  public:
    bool operator()( const struct MappedItem* x, const struct MappedItem* y ) const {
      return( x->baseAddress > y->baseAddress );
    }
  };

  /**
   * A functor to sort the list of mapped Windows. 
   * The windows are sorted according to their address rank.
   */
 class WindowSorter {
  public:
    bool operator() ( const VME64xMappedWindow* x, 
                      const VME64xMappedWindow* y) const {
      return ( x->getAddressRank() < y->getAddressRank() );
    }
  };

  /**
   * Helper function which contains functionality needed in all constructors.
   */
  void configurePlugAndPlay() throw( IllegalValueException,
				     UnsupportedException );
  /**
   * Evaluate a static VMEConfiguration given in the Constructor. 
   * This routine is necessary in order to allow the mapping of the crate in case
   * there are plug-and-play VME64x and VME modules coexisting in the crate. 
   */
  void populateWithStaticConfiguration( const StaticVMEConfiguration& staticConfiguration )
    throw( IllegalValueException );

  /**
   * Find all plug and play modules in the create and fill the corresponding data
   * structures.
   * After this routine has run there is a VMESlot object for each possilbe slot 
   * in the crate.
   */
  void populateWithPlugAndPlay() throw( IllegalValueException,
					UnsupportedException );

  /**
   * Build a list of all mapped windows of all modules in the crate. 
   *
   * The resulting list is sorted in ascending order with respect to the
   * size of the region occupied by the window.
   */
  void getAllWindows();

  /**
   * Map the address space for the whole crate.
   */
  void mapCrate();

  /**
   * Map a Window in the address space.
   *
   * The algorithm tries to map a window stating from the highest possible
   * address the module can use. It assumes lazy address decoding by the 
   * VME module. 
   *
   * @param maxAddress is the highest address the module can possibly
   *        address (for A32 0 is used, for A24 0x1000000, for A16 0x10000).
   * @param baseMin is the lowest possible baseAddress ( usually the lowest
   *        64kB are reserved for A16 and the lowest 16MB (without the A16 window)
   *        for A24).
   */
  void mapWindow( VME64xMappedWindow* windowPtr, 
                  uint32_t maxAddress, 
                  uint32_t baseMin );

  /**
   * Test if an address-region is already occupied.
   */
  bool isOccupied( uint32_t base, uint32_t space, bool bit33 );
  
  /**
   * Initilize the occupied address space with the static vme configuration.
   *
   * Find all configured Mapped Windows and build a list of mapped Items which
   * contain the occupied addressSpace. 
   */
  void makeOccupiedList();

  /**
   * Enable all mapped plug and play modules in the crate.
   */
  void enablePlugAndPlayModules() throw ( IllegalOperationException );

  bool mapped_;
  VMEBusAdapterInterface& busAdapter_;
  AddressTableContainerInterface& addressTableContainer_;
  ModuleMapperInterface& moduleMapper_;
  VMEConfigurationSpaceHandler vmeConfigSpaceHandler_;
  std::vector< VMESlot* > slotPtrVector_;
  std::list < struct MappedItem * > occupiedItemList_;
  std::list < VME64xMappedWindow * > windowList_;
  std::list < const VME64xMappedWindow * > notMappedList_;
};

} /* namespace HAL */

#endif /* __VME64xCrate */

