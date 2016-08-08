#ifndef __ASCIIFileAddressTableContainer
#define __ASCIIFileAddressTableContainer

#include <tr1/unordered_map>
#include <string>

#include "hal/AddressTableContainerInterface.hh"
#include "hal/ModuleMapperInterface.hh"
#include "hal/VMEAddressTableXMLFileReader.hh"
#include "hal/PCIAddressTableXMLFileReader.hh"
#include "hal/VMEAddressTableASCIIReader.hh"
#include "hal/PCIAddressTableASCIIReader.hh"
#include "hal/NoSuchFileException.hh"
#include "hal/IllegalValueException.hh"
#include "hal/XMLProcessingException.hh"

namespace HAL {

/**
*
*
*     @short Retrieves AddressTables for a given SerialNumber.
*            
*            The  class reads  an  ASCII file  which  maps typeIds  to 
*            pathnames  of AddressTable files.  It then uses  the 
*            ModuleMapper in  order to retrieve  the AddressTables for 
*            a given  SerialNumber. AddressTables are  constructed the 
*            first time they are requested.  They are then stored in a
*            map  with the  typeId as  a  key. Therefore  they can  be 
*            shared  by   all  modules  with  the   same  typeId  (but 
*            different serialNumbers).
*            
*            Addresstables  can be either  in xml  format or  in ASCII 
*            format.  If the  file  extension of  the AddressTable  is 
*            ".xml"  or  ".XML" the  file  is  interpreted  as an  XML 
*            AddressTable. In all other  cases the file is interpreted
*            as an ASCII AddressTable. 
*
*            The format of the ASCII File is as follows:
*            
*            <pre>
*
*            ********************************************************
*            *  typeId       technology      addressTableFileName
*            ********************************************************
*            ECAL-DCC         vme            /home/data/dccAddressTable.xml
*            TTCvi            vme            /home/data/ttcviAddressTable.xml
*            gIIIFed          pci            /home/data/gIIIFedAddressTable.xml
*            ********************************************************
*            
*            </pre>
*            
*            The technology column might have the values 'vme' or 'pci'.
*            
*       @see 
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
**/

class ASCIIFileAddressTableContainer : public AddressTableContainerInterface {
public:

  /**
   * The constructor of the ASCIIFileAddressTableContainer.
   * @param moduleMapper is a ModuleMapperInterface in order to map the 
   *        serialNumber of a module to its typeId, and to retrieve the 
   *        baseaddress if the module is a standard VME module (not 
   *        VME64x plug and play)
   * @param addressTableMapFile is the path to the ASCII file described
   *        above.
   * @param addressTablePrefix allows the user to define a base-directory
   *        which will be prefixed to all address table file strings found
   *        in the map file.
   */
  ASCIIFileAddressTableContainer( ModuleMapperInterface& moduleMapper,
                                  std::string addressTableMapFile,
				  std::string addressTablePrefix = "")
    throw( NoSuchFileException,
	   IllegalValueException,
	   XMLProcessingException );
  
  virtual ~ASCIIFileAddressTableContainer();
  
  /**
   * Retrieves the AddressTable of a module with a given seralNumber.
   */
  VMEAddressTable& getVMETableFromSerialNumber( std::string serialNumber )
    throw( IllegalValueException );


private:
  ModuleMapperInterface& moduleMapper_;
  std::string mappingTable_;
  std::tr1::unordered_map< std::string, VMEAddressTable* > vmeAddressTableMap_;
  std::tr1::unordered_map< std::string, PCIAddressTable* > pciAddressTableMap_;
};

} /* namespace HAL */

#endif /* __ASCIIFileAddressTableContainer */
