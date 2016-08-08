#ifndef __VMESlot
#define __VMESlot

#include <list>
#include <vector>
#include <string>

#include "hal/VMEConfigurationSpaceHandler.hh"
#include "hal/AddressTableContainerInterface.hh"
#include "hal/ModuleMapperInterface.hh"
#include "hal/VME64xFunction.hh"
#include "hal/VMEDevice.hh"
#include "hal/VME64xHardwareAddress.hh"
#include "VMEConstants.h"
#include "hal/StaticVMEItem.hh"

namespace HAL {

enum SlotContents{ EMPTY, VME, VME64x };

/**
*
*
*     @short 
*            
*            Concept:  Everything  which  is  related  to  the  VME64x 
*            configuration  space  (NOT   the  user  defined  one)  is 
*            handled by the  slot. (This is the main  "service" of the 
*            slot.) This allows to leave  the module in the slot to be
*            a  normal VMEDevice  from the  generic part  of the  HAL.  
*            User  defined items in  the CR/CSR  space can be accessed 
*            via  the  VMEDevice  by  specifiying  the  items  in  the 
*            address table. 
*
*       @see 
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:10 $
*
*
**/

class VMESlot {

public:

  /**
   * Constructs the VMEModule if the slot is not empty.
   * 
   * The first step of the Constructor is to check if there is 
   * a VME64x Module plugged into the slot or not. If so, it's
   * serial number is read which is used to retrieve the AddressTable
   * from the AddressTableContainer. The construction of the
   * VMEDevice must be deferred until the 
   * mapping of the baseaddress has been established by the 
   * VMECrate. Therefore the VMESlot must contain all functionality
   * to read out the relevant configuration space data which 
   * is necessary to establish the baseaddress mapping of the
   * crate.
   *
   */
  VMESlot( uint32_t slotNumber,
           VMEConfigurationSpaceHandler* configAdapterPtr,
           AddressTableContainerInterface& addressTableContainer,
           ModuleMapperInterface & moduleMapper,
	   bool ignoreChecksumError = false)
    throw( IllegalValueException,
           UnsupportedException );

  /**
   * A Constructor for a VMESlot with a "standard" ( i.e. not 
   * plug and play ) module plugged into it.
   */
  VMESlot( StaticVMEItem* staticVME,
           VMEBusAdapterInterface* busAdapterPtr,
           AddressTableContainerInterface& addressTableContainer,
           ModuleMapperInterface & moduleMapper )
    throw( IllegalValueException );


  virtual ~VMESlot();

  enum SlotContents getContents() const;

  std::string getTypeId() const;

  /**
   * Retrieve serial number of the slot.
   * If the slot is empty retyurn "";
   */
  std::string getSerialNumber() const;
 
 /**
   * Returns only those functions which are really implemented.
   *
   * This functions leaves the slot "const" and the functions. Only
   * the slot itself should be allowed to change the functions.
   */
  std::list< VME64xFunction* > getImplementedFunctions() const;

  /**
   * Return or create and return the VMEModule in this slot.
   * 
   * The caller of the method becomes owner of the VMEDevice and
   * is responsitble for its deletion. 
   *
   */
  VMEDevice* getVMEDevice() const
    throw( IllegalOperationException,
           IllegalValueException,
	   UnsupportedException,
	   BusAdapterException,
	   NoSuchItemException );
  /**
   * Retrieve the Id number of the slot.
   */
  uint32_t getSlotId() const;

  /**
   * Read a standard Configuration Item.
   *
   * This function allows to read a Standard Configuration Item from the 
   * Configuration space. Standard Configuration Items are those which are
   * defined in the VME64x specification. The string to identify these items
   * are defined in the class VMEConfigurationSpaceAddressReader.
   *
   * THIS METHOD MUST NOT BE USED TO READ USER DEFINED CONFIGURATION SPACE ITEMS!
   *
   * @see VMEConfigurationSpaceReader, VME64xDevice
   *
   */
  void stdConfigRead( std::string item,
		      uint32_t* resultPtr,
		      uint32_t offset = 0 ) const
    throw( NoSuchItemException, 
	   IllegalOperationException,
	   BusAdapterException );
  

  /**
   * Write a standard Configuration Item.
   *
   * This function allows to write a Standard Configuration Item in the 
   * Configuration space. Standard Configuration Items are those which are
   * defined in the VME64x specification. The string to identify these items
   * are defined in the class VMEConfigurationSpaceAddressReader.
   * 
   * Of course this method only works for the writable items in the CSR space.
   *
   * THIS METHOD MUST NOT BE USED TO WRITE USER DEFINED CONFIGURATION SPACE ITEMS!
   *
   * @see VMEConfigurationSpaceReader, VME64xDevice
   *
   */
  void stdConfigWrite( std::string item,
		       uint32_t data,
		       HalVerifyOption verifyFlag = HAL_NO_VERIFY,
		       uint32_t offset = 0 ) const
    throw( NoSuchItemException, 
	   IllegalOperationException,
	   BusAdapterException );
  

private:
  /**
   * Check addresstable for consistency against contents of configuration
   * space. In addition this function configures the HardwareAddresses of
   * VME64x modules with the address modifier and the data width which will
   * be used to access the module.
   */
  void checkVME64xTable( VMEAddressTable& table ) const
    throw( IllegalValueException );

  uint32_t slotId_;
  VMEConfigurationSpaceHandler* configAdapterPtr_;
  VMEBusAdapterInterface* busAdapterPtr_;
  AddressTableContainerInterface& addressTableContainer_;
  ModuleMapperInterface& moduleMapper_;
  enum SlotContents contents_;

  /**
   * The list of implemented functions of the module in this slot.
   * If there is a standard VME module in the slot (non-VME64x) then
   * there is only the first function implemented.
   */
  std::list< VME64xFunction* > functionPtrs_; 

  std::string serialNumber_;
  std::string typeId_;

  /**
   * There is one entry in this vector for each POSSIBLE window.
   * This means the vector always has the size of 6.
   */
  std::vector< const VME64xMappedWindow* > windowVector_;


};

} /* namespace HAL */

#endif /* __VMESlot */

