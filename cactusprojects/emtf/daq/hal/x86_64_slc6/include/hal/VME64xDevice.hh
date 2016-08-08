#ifndef __VME64xDevice_hh_
#define __VME64xDevice_hh_

#include <string>

#include "hal/VMEDevice.hh"
#include "hal/VMESlot.hh"

namespace HAL {

class VME64xDevice : public VMEDevice
{
public :
  
  /**
   * The constructor of the VME64xDevice.
   *
   * This class extends the base class in order to handle the
   * configuration space access.
   *
   * @vmeSlot is giving the access to the configuration Space.
   */
  VME64xDevice( HAL::VMEAddressTable & vmeAddressTable,
		HAL::VMEBusAdapterInterface & vmeBusAdapter,
		std::vector<uint32_t>& baseaddresses,
		bool doSwapping,
		const HAL::VMESlot &vmeSLot )
    
    throw( HAL::NoSuchItemException, 
	   HAL::IllegalOperationException,
	   HAL::BusAdapterException,
	   HAL::IllegalValueException,
	   HAL::UnsupportedException );
  
  /**
   * Overwrite the baseclass method in order to handle configuration space access.
   *
   * The items therein are interpreted different in the sense that a 4 byte
   * wide items occupies a 4*(m_skip) byte wide address space. m_skip is
   * derived from bytes 0x13 and 0xff in the CROM (data access widths) and 
   * define if every byte (m_skip=0) every other byte (m_skip = 1) or only every
   * fourth byte (m_skip=3) is read out. 
   *
   * If a user has a User defined region in the configuration ROM, the byte 0x13
   * (Configuration ROM data access width) determines which bytes are read in this
   * region and which bytes are skipped. In case that byte 0x13 demands that not
   * every byte is read, the data widht is reinterpreted in the same way as 
   * mentioned above: 
   * 
   */
  void hardwareRead( const HAL::GeneralHardwareAddress& vmeAddress, 
		     uint32_t* result,
		     uint32_t offset ) const
    throw( HAL::BusAdapterException );
    
  /**
   * Overwrite the baseclass method in order to handle configuration space access.
   */
  void hardwareWrite( const HAL::GeneralHardwareAddress& vmeAddress, 
		      uint32_t data,
		      uint32_t offset ) const
    throw( HAL::BusAdapterException );

  /**
   * A method to read out standard configuration space items.
   *
   * This method is usefull since the standard VME64x configuration space items
   * only occupy every 4th byte in the configuration address space. This makes
   * the readout of the items cumbersome if the normal readout methods "read" or
   * "unmaskedRead" are used. 
   * This method uses a hardcoded addresstable for the VME64x configuration space
   * items. 
   *
   * In prinicple this method is not needed and standard configuration space
   * items could be defined in the modules Addresstable and could be read out
   * via the standard read method. But this procedure becomes cumbersome if 
   * if the CROM configuration parameter "Configuration ROM data access width"
   * (byte 0x13) defines that every or every second byte of the CROM is a valid
   * byte. In this case the user would have to assemble the mutli byte item 
   * by hand in order to eliminate the non valid bytes in between. (The VME64x
   * specification only uses every 4th byte in the CROM and CSR space).
   *
   * This method automatically does this task for the user. This routine
   * can only be used for the standard VME64x configuration space items which
   * are defined in the VMEConfigurationSpaceAddressReader.cc.
   *
   * User Configruation Space items should be read out with the normal read and
   * write methods. In these routines all bytes which are not skipped (according 
   * to the configuration items CROM Access width (0x13) and CRAM access width (0xFF)
   * are read out and are used in order to compose the bytes of the item defined
   * in the Addresstable. 
   *
   */
  void stdConfigRead( std::string item, 
		      uint32_t* resultPtr, 
		      uint32_t offset = 0 ) const
    throw( HAL::NoSuchItemException, 
	   HAL::IllegalOperationException,
	   HAL::BusAdapterException );

  void stdConfigWrite( std::string item, 
		       uint32_t data,
		       HalVerifyOption verifyFlag,
		       uint32_t offset = 0 ) const
    throw( HAL::NoSuchItemException, 
	   HAL::IllegalOperationException,
	   HAL::BusAdapterException );
  
  /**
   * Return the serial Number of the Module.
   *
   * This function retrieves the serial number of the module. The serial 
   * Number of the module is read during the initialization of the VME64x
   * crate and stored internally. This function returns this stored value.
   * It does not generate a hardware access to the VME module.
   */
  std::string getSerialNumber() const;

protected :

  /**
   * Internally used to read user defined Configuration Space items.
   */
  void userConfigRead( const HAL::GeneralHardwareAddress& vmeAddress, 
		       uint32_t *resultPtr,
		       uint32_t offset ) const
    throw( HAL::IllegalOperationException,
	   HAL::BusAdapterException );

  /**
   * Internally used to write user defined Configuration Space items.
   */
  void userConfigWrite(const HAL::GeneralHardwareAddress& vmeAddress, 
		       uint32_t data,
		       uint32_t offset) const
    throw( HAL::IllegalOperationException,
	   HAL::BusAdapterException );
  
  const HAL::VMESlot &vmeSlot_;
  uint32_t configSpaceOffset_;
  uint32_t CRAMAccessWidth_;
  uint32_t CROMAccessWidth_;
  uint32_t CSRAccessWidth_;

  uint32_t userCRStart_;
  uint32_t userCREnd_;
  uint32_t userCSRStart_;
  uint32_t userCSREnd_;
  uint32_t CRAMStart_;
  uint32_t CRAMEnd_;

private:
  uint32_t getAccessWidth( std::string item ) const
    throw( HAL::NoSuchItemException, 
	   HAL::IllegalOperationException,
	   HAL::BusAdapterException );

  uint32_t getAccessInterval(uint32_t address, uint32_t width ) const
    throw( HAL::IllegalOperationException );
};

} /* namespace HAL */

#endif /* __VME64xDevice_hh_ */

