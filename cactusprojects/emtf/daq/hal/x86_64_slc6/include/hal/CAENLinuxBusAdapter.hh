#ifndef __CAENLinuxBusAdapter
#define __CAENLinuxBusAdapter

#include <semaphore.h>
#include <vector>
#include <ostream>
#include "CAENVMEtypes.h"
#include "hal/IllegalValueException.hh"
#include "hal/BusAdapterException.hh"
#include "hal/UnsupportedException.hh"
#include "hal/VMEBusAdapterInterface.hh"
#include "hal/VMEAddressTable.hh"
#include "hal/DeviceIdentifier.hh"
#include "hal/CAENDeviceIdentifier.hh"

namespace HAL {

/**
*
*
*     @short BusAdapter for the PC-to-VME interfaces from CAEN.
*
*            This BusAdapter class handles two models of the CAEN Busadapter
*                         modules: The V2718  and the V1718. The V2718
*            is  based on  an  optical link  from  the PC  to the  VME 
*            controller.  The  V1718 is  based  on  a  USB link.  Both 
*            modules  are software  compatible except  for  some minor 
*            differences (e.g. Interrupt handling). 
*
*            The  correct  operation  of  this  BusAdapter  needs  the 
*            driver software of the CAEN interface to be installed. In
*            the  MakeVars file for  the BusAdapters  the path  to the 
*            relevant  include files  must  be specified  in order  to 
*            compile the adapter. 
*            
*            The  busAdapter does  direct driver  accesses  using the 
*            API of CAEN called "CAENVMELib".
*
*       @see BusAdapterInterface
*    @author Christoph Schwick
* $Revision: 1.2 $
*     $Date: 2008/08/27 18:33:29 $
*
*
**/

class CAENLinuxBusAdapter : public VMEBusAdapterInterface {
public:

  /**
   * A type to distinguish the supported CAEN BusAdapters.
   */
  enum CAENModel{ V1718, V2718 };
  enum CAENPCCard{ A2818, A3818 };

  /**
   * The constructor of the CAENLinuxBusAdapter.
   * The BusAdapter sets up the CAEN VME driver to work with the HAL.
   * Driver parameters are preset here. Be aware that no other application
   * on the system should change these parameters as long as the HAL is 
   * used. The bute swapping strategy is defined in the constructor. 
   *
   * @param CAENModel specifies the type of the CAEN module.
   *        Two modules are supported: V2718 (optical link) and V1718 (USB).
   * @param unit is a parameter which lets you distinguish between 
   *        various CAEN-busAdapters in the case that you have more
   *        than one interface connected to your PC via PCI. The interfaces
   *        are numbered from 0 to n.
   *        In case of a a3818 adapter this number is not completely intuitive
   *        to encode: The software is written for max. 4 a3818 cards ina PC:
   *        unit numbers 0,1,2,3 refer to the first optical link in the four
   *        cards. 4,5,6,7 refere to the second links, etc. 
   *        If you have one adapter card and want to use the first three links
   *        for you would have to specify 0 for the adapter serving the first link,
   *        4 for the adapter of the second link and 8 for the one dealing with 
   *        the third link.
   * @param chain indicates the interface in case you use a chain of controllers
   *        Numbers range from 0 to n and indicate the position of the 
   *        VMEController in the chain. This parameter has no meaning (and effect)
   *        if the USB version of the bridge is used.
   * @param pcCard is a parameter which you need to indicate with which PCI card
   *        you are operating the BusAdapter. CAEN presently has the A2818 pci
   *        card with one optical link and the A3818 PCIe card with four optical 
   *        links.
   * 
   */
  CAENLinuxBusAdapter( CAENModel model, int unit = 0, int chain = 0, CAENPCCard pcCard = A2818 ) 
    throw( BusAdapterException );

  virtual ~CAENLinuxBusAdapter() throw( BusAdapterException );

  /**
   * This function registers a standard VMEDevice with the BusAdapter.
   * @param VMEAddressTable is currently not needed by this specific
   *        busAdapter, but it is part of the interface in order to 
   *        allow a busAdapter to do memory mapping.
   *                 
   */
  void openDevice(const VMEAddressTable& VMEAddressTable,
                  uint32_t vmeBaseaddress,
                  DeviceIdentifier** deviceIdentifierPtr,
                  uint32_t* baseAddressPtr,
		  bool doSwapping)
    throw(BusAdapterException);

  /**
   * This function registers a VME64x module with the BusAdapter.
   * @param VMEAddressTable is currently not needed by this specific
   *        busAdapter, but it is part of the interface in order to 
   *        allow a busAdapter to do memory mapping.
   *                 
   */
  void openDevice(const VMEAddressTable& VMEAddressTable,
                  std::vector<uint32_t> vmeBaseaddresses,
                  DeviceIdentifier** deviceIdentifierPtr,
                  std::vector<uint32_t>* baseAddressesPtr,
		  bool doSwapping)
    throw(BusAdapterException);

  /**
   * Here the deviceIdentifier is destroyed.
   */
  void closeDevice( DeviceIdentifier* deviceIdentifier ) 
    throw();

  void write( DeviceIdentifier* vmeDevice,
              uint32_t address,
              uint32_t addressModifier,
              uint32_t dataWidth,
              uint32_t data)
    throw( BusAdapterException );
  
  void read( DeviceIdentifier* vmeDevice,
             uint32_t address,
             uint32_t addressModifier,
             uint32_t dataWidth,
             uint32_t* result)
    throw( BusAdapterException );

  void writeBlock( DeviceIdentifier* deviceIdentifierPtr,
                   uint32_t startAddress,
                   uint32_t length,      // in bytes
                   uint32_t addressModifier,
                   uint32_t dataWidth,
                   char *buffer,
                   HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT )
    throw( BusAdapterException, UnsupportedException );
  
  void readBlock( DeviceIdentifier* deviceIdentifierPtr,
                  uint32_t startAddress,
                  uint32_t length,      // in bytes
                  uint32_t addressModifier,
                  uint32_t dataWidth,
                  char *buffer,
                  HalAddressIncrement addressBehaviour = HAL_DO_INCREMENT )
    throw( BusAdapterException, UnsupportedException );

  /**
   * Generate a Reset on the VME backplane.
   */
  void resetBus( )
    throw( BusAdapterException );
  
  /**
   * Read the display of the CAEN display an put in in ASCII format 
   * int the output stream os.
   */
  void readDisplay( std::ostream& os ) const
    throw( BusAdapterException );

  /**
   * Print out the versions read in the Constructor in 
   * the output stream. 
   */
  void readVersions( std::ostream& os ) const;

  /**
   * Enable interrupts for those IRQ lines which are
   * indicated by the mask. 
   */
  void enableIRQ( uint32_t mask ) const
    throw( BusAdapterException );

  /**
   * Disable interrupts for those IRQ lines which are
   * indicated by the mask. 
   */
  void disableIRQ( uint32_t mask ) const 
    throw( BusAdapterException );
  
  /**
   * Chack if an Interrupt has been asserted. 
   * The return value contains a bit mask of the 
   * correspondign interrupt lines.
   */
  uint32_t checkIRQ( ) const
    throw( BusAdapterException );

  /**
   * Wait for an interrupts to occur.
   *
   * If the timeoutMs is set to 0 this function waits until an interrupt 
   * occurs. If it is set to value n>0 it will return after n milliseconds
   * if no interrupt occurs. The return value contains a bitmask of the
   * interrupt lines asserted.
   */
  uint32_t waitIRQ( uint32_t mask, uint32_t timeoutMs ) const 
    throw( BusAdapterException );

  /**
   * Generates an Interrupt acknowledge cycle.
   */
  void acknowledgeIRQ( uint32_t level, void* vector, uint32_t width ) const 
    throw( BusAdapterException );

private:

  static int32_t handleVX718_[16][8];
  static int32_t handleVX718UseCount_[16][8];
  static sem_t * handleSemaphore_;
  int unit_;
  int chain_;
  char V1718FwRelease_[256];
  char V2718FwRelease_[256];
  char AX818FwRelease_[256];
  char A2719FwRelease_[256];
  char VMELibRelease_[256] ;
  CAENModel model_;
  CVBoardTypes pcCard_;
  std::string pcCardStr_;

};

} /* namespace HAL */

#endif /* __CAENLinuxBusAdapter */
