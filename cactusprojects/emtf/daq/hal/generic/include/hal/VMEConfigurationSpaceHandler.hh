#ifndef __VMEConfigurationSpaceHandler
#define __VMEConfigurationSpaceHandler

#include <string>
#include <vector>

#include "hal/VMEBusAdapterInterface.hh"
#include "hal/NoSuchItemException.hh"
#include "hal/IllegalOperationException.hh"
#include "hal/BusAdapterException.hh"
#include "hal/VMEConfigurationSpaceDevice.hh"
#include "hal/VMEAddressTable.hh"
#include "hal/VMEConfigurationSpaceAddressReader.hh"

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

class VMEConfigurationSpaceHandler {
public:
  VMEConfigurationSpaceHandler( VMEBusAdapterInterface& busAdapter );
  virtual ~VMEConfigurationSpaceHandler();

  void configWrite( std::string item,
		    uint32_t slotId,
		    uint32_t data,
		    HalVerifyOption verifyFlag = HAL_NO_VERIFY,
		    uint32_t offset = 0 ) const
    throw( NoSuchItemException, 
           IllegalOperationException,
           BusAdapterException );

  void configRead( std::string item,
		   uint32_t slotId,
		   uint32_t* resultPtr,
		   uint32_t offset = 0 ) const
   throw( NoSuchItemException, 
          IllegalOperationException,
          BusAdapterException );
  
  bool containsVME64xModule( uint32_t slotId ) const
          throw( IllegalValueException );

  bool functionIsImplemented( uint32_t slotId, 
                              uint32_t functionId ) const;
  
  std::string readSerialNumber( uint32_t slotId ) const
	throw( IllegalOperationException );

  void enableVME64xModule( uint32_t slotId ) const
    throw( IllegalOperationException );

  void disableVME64xModule( uint32_t slotId ) const
    throw( IllegalOperationException );

  bool enabled( uint32_t slotId ) const
    throw( IllegalOperationException );

  uint32_t getUserROMInterval( uint32_t slotId ) const;

  /**
   * Takes into account the UserROMInterval (which bytes are valid).
   */
  void readROMBlock( uint32_t slotId, 
                     uint32_t startAdr, 
                     uint32_t endAdr, 
                     std::vector<unsigned char>* data ) const;

  bool checksumOk( uint32_t slotId ) const
	throw( IllegalOperationException );

  VMEBusAdapterInterface& getBusAdapter() const;

private:
  void checkVME64xConfigSpace( uint32_t slot ) const
	throw( IllegalOperationException );

  inline uint32_t calculateOffset( uint32_t slotId ) const;

  /**
   * A helper function which calls configRead but does not throw 
   * exceptions. If in this routine an exception is encountered 
   * the program is stopped since then a hardware failure or an
   * internal software bug has been encountered. The routine 
   * simplifies the throw-clauses of calling methods.
   */
  void saveRead( std::string item,
		 uint32_t slot,
		 uint32_t* result, 
		 uint32_t offset = 0 ) const ;
  
  /**
   * A helper function which calls configWrite but does not throw 
   * exceptions. If in this routine an exception is encountered 
   * the program is stopped since then a hardware failure or an
   * internal software bug has been encountered. The routine 
   * simplifies the throw-clauses of calling methods.
   */
  void saveWrite( std::string item,
                  uint32_t slot,
                  uint32_t data, 
                  HalVerifyOption verifyFlag = HAL_NO_VERIFY,
                  uint32_t offset = 0 ) const ;

  VMEBusAdapterInterface& busAdapter_;
  VMEConfigurationSpaceDevice* vmeDevicePtr_;
  VMEAddressTable* configurationAddressTablePtr_; 
};

} /* namespace HAL */

#endif /* __VMEConfigurationSpaceHandler */

