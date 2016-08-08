#ifndef __VME64xMappedWindow
#define __VME64xMappedWindow

#include <list>

#include "hal/VMEConfigurationSpaceHandler.hh"
#include "hal/IllegalOperationException.hh"
#include "hal/IllegalValueException.hh"
#include "hal/UnsupportedException.hh"
#include "hal/BusAdapterException.hh"
#include "hal/NoSuchItemException.hh"
#include "hal/AMSortFunctor.hh"

namespace HAL {

/**
*
*
*     @short Representation of a mapped Address space window for a VME
*            module.
*            
*            The software implements in  the following way the concept
*            of the VME function and the Mapped Window: 
*            A function has one or more mapped windows.
*            All  mapped  windows of  a  function  refer  to the  same 
*            baseaddress  and  are distinguished  by  the  AM and  the 
*            access width.
*            In  the AddressTable  items are  specified by  the Mapped
*            window ID  (0..7) or  the Address Space.  The AM  and the
*            baseaddress   the   window   are   defined   during   the
*            configuration.  Therefore the Module Designer can force a
*            specific AM  to be used by  offering only that  AM in the
*            AMCAP for a given function.
*
*       @see 
*    @author Christoph Schwick
* $Revision: 1.2 $
*     $Date: 2009/01/14 18:10:06 $
*
*
**/

class VME64xMappedWindow {

public:
  VME64xMappedWindow( uint32_t slotId,
                      uint32_t functionId,
                      uint32_t mappedWindowId, 
                      VMEConfigurationSpaceHandler* vmeConfigurationSpaceHandler )
    throw( NoSuchItemException,
           IllegalOperationException,
           BusAdapterException,
           UnsupportedException,
           IllegalValueException );
  
  /**
   *
   * The constructor for standard VME modules.
   *
   * These modules have exaclty one mapped window. The mapping is defined
   * by some hardwired setting on the board and cannot be configured 
   * dynamically.
   */
  VME64xMappedWindow( uint32_t slotId_, 
		      uint32_t baseAddress,
		      uint32_t addressRank ); 

  virtual ~VME64xMappedWindow();
  
  uint32_t getDataAccessWidth() const;
  uint32_t getAddressRank() const;
  uint32_t getAM() const;
  uint32_t getMappedWindowId() const;
  uint32_t getFunctionId() const;
  uint32_t getSlotId() const;
  uint32_t getBaseaddress() const;
  const std::list< uint32_t >& getSortedAMCAPList() const;

  bool isConfigured() const;
  bool isImplemented() const;
  bool hasAnotherWindow() const;
  bool canA32() const;
  bool canA24() const;
  bool canA16() const;

  /**
   *
   * Set the ADER register with a baseAddress and a AM.
   *
   * In case this window is for a standard VME module without 
   * configuration space, an IllegalOperationException is thrown.
   */
  void setADER( uint32_t baseAddress,
                uint32_t AM )
    throw( NoSuchItemException, 
           IllegalOperationException,
           BusAdapterException );

private:
  /**
   * Read out the contents of a configured Mapped Window from the Hardware
   * and store the contents in the internal data. 
   */
  void configFromHardware() 
    throw( NoSuchItemException, 
	   IllegalOperationException,
           UnsupportedException,
	   BusAdapterException );
    
  void setAddressCapability( uint32_t AM );
  uint32_t slotId_;         // needed to access the configuration space
  uint32_t functionId_;
  uint32_t mappedWindowId_;  // between 0..7
  VMEConfigurationSpaceHandler* vmeConfigurationSpaceHandlerRef_; 

  std::list<uint32_t> AMCAP_;
  uint32_t AM_;
  uint32_t baseaddress_;

  uint32_t ADEM_;
  uint32_t addressRank_;

  uint32_t DAWPR_;
  uint32_t dataAccessWidth_;

  uint32_t ADER_;

  bool implemented_;
  bool configured_;
  
  bool canA16_;
  bool canA24_;
  bool canA32_;
};

} /* namespace HAL */

#endif /* __VME64xMappedWindow */
