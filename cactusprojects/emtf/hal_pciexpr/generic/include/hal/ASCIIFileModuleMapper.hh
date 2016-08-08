#ifndef __ASCIIFileModuleMapper
#define __ASCIIFileModuleMapper

#include <tr1/unordered_map>
#include <string>

#include "hal/ModuleMapperInterface.hh"
#include "hal/NoSuchFileException.hh"

namespace HAL {

/**
*
*
*     @short Reads an ASCII file to map serialNumbers to typeIds and
*            baseAddresses.
*            
*            The format of the ASCII File is as follows:
*            
*            <pre>
*
*            ********************************************************
*            *  serialNumber       typeId      baseAddress(hex)
*            ********************************************************
*            daqECAL_DCC-r1-001    ECAL-DCC    00000000
*            TTCve-0192            TTCvi       00900800
*            ********************************************************
*            
*            </pre>
*            
*            The  example contains  two modules.  The  baseaddress for 
*            VME64x modules  is ignored (but must be  present in order 
*            to make this class work.)
*            
*            No  thorough testing  of the  input file  format  is done 
*            since it is expected that  in the final system the module
*            mapper    will   use    a   database    (look    at   the 
*            DatabaseModuleMapper).
*
*       @see DatabaseModuleMapper, ModuleMapperInterface
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/

class ASCIIFileModuleMapper : public ModuleMapperInterface {

public:

  /**
   * The constructor needs the fileName as argument.
   */
  ASCIIFileModuleMapper( std::string mapFile )
    throw( NoSuchFileException );

  virtual ~ASCIIFileModuleMapper() { };

  /**
   * This function maps the serialNumber to the TypeID. If the mapping
   * is unsuccessfull an exception is thrown.
   */
  std::string getTypeId( std::string serialNumber ) 
    throw( IllegalValueException );

  /**
   * Retrieve the baseAddress for a standard VME module.
   */
  uint32_t getBaseaddress( std::string serialNumber ) 
    throw( IllegalValueException );

private:
  std::string mapFile_;
  std::tr1::unordered_map<std::string,std::string> typeIdMap_;
  std::tr1::unordered_map<std::string,uint32_t> baseaddressMap_;
};

} /* namespace HAL */

#endif /* __DatabaseModuleMapper */
