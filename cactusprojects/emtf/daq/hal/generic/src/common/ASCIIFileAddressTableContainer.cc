#include <sstream>
#include <fstream>
#include <iomanip>

#include "hal/ASCIIFileAddressTableContainer.hh"

HAL::ASCIIFileAddressTableContainer::ASCIIFileAddressTableContainer(HAL::ModuleMapperInterface& moduleMapper,
								    std::string addressTableMapFile,
								    std::string addressTablePrefix )
  throw( HAL::NoSuchFileException,
	 HAL::IllegalValueException,
	 HAL::XMLProcessingException) 
  : moduleMapper_(moduleMapper),
    mappingTable_(addressTableMapFile) {

  // some preparation to read the file:
  const int MaxLength=1024;
  char buffer[MaxLength];
  std::ifstream FileStream( addressTableMapFile.c_str() );
  if ( ! FileStream ) {
    std::string text = "Cannot open file : " + addressTableMapFile + "    (HAL::VMEAddressTableASCIIReader::HAL::VMEAddressTableASCIIReader)";
    throw (HAL::NoSuchFileException( text, __FILE__, __LINE__, __FUNCTION__ ));
  }


  // prepare the loop over all lines of the ascii file:
  std::string typeId, technology, addressTableFileName;

  // loop over all lines of the file:
  while ( ! FileStream.eof() ) {
    FileStream.getline(buffer, MaxLength);
    if ( FileStream.eof() ) break;
    //skip all white space at beginning of buffer:
    int ic = 0;
    while ( buffer[ic] == ' ' ||
            buffer[ic] == '\t' ) {
      ic++;
    }
    if (buffer[ic] == '*') continue;
    if (buffer[ic] == '\n') continue; // should never happen since \n is not extracted with getline
    if (buffer[ic] == 0x00) continue;
    
    std::istringstream Line(buffer);    
    Line >> typeId;
    Line >> std::ws >> technology;
    Line >> std::ws >> addressTableFileName;

    // check if the termination is ".xml" or ".XML"
    // set a flag to indicate if we deal with xml or ascii 
    // AddressTables. The decision is made looking at the
    // file-extension: .xml and .XML are treated as xml files.
    bool xmlFile = false;
    if ( ( addressTableFileName.rfind( ".xml" ) != std::string::npos ) || 
	 ( addressTableFileName.rfind( ".XML" ) != std::string::npos ) ) {
      xmlFile = true;
    }

    if ( addressTablePrefix != "" ) {
      addressTableFileName = addressTablePrefix + addressTableFileName;
    }

    HAL::AddressTableReader* readerPtr;

        if ( technology == "vme" ) {

      if ( xmlFile ) {
	readerPtr = new HAL::VMEAddressTableXMLFileReader( addressTableFileName );
      } else {
	readerPtr = new HAL::VMEAddressTableASCIIReader( addressTableFileName );
      }

      try {
	HAL::VMEAddressTable* adrTablePtr = new HAL::VMEAddressTable ( typeId, *readerPtr );
	vmeAddressTableMap_[typeId] = adrTablePtr;
      } catch( HAL::HardwareAccessException& e ) {
	delete (readerPtr);
	throw;
      }

      delete readerPtr;

    } else if ( technology == "pci" ) {
      
      if ( xmlFile ) {
	readerPtr = new HAL::PCIAddressTableXMLFileReader( addressTableFileName );
      } else {
	readerPtr = new HAL::PCIAddressTableASCIIReader( addressTableFileName );
      }

      try {
	HAL::PCIAddressTable* adrTablePtr = new HAL::PCIAddressTable ( typeId, *readerPtr );
	pciAddressTableMap_[typeId] = adrTablePtr;
      } catch ( HAL::HardwareAccessException& e ) {
	delete (readerPtr);
	throw;
      }
      delete readerPtr;

    } else {
      std::stringstream text;
      text << "Unknown technology : " << technology
	   << "\n    (HAL::ASCIIFileAddressTableContainer::ASCIIFileAddressTableContainer)" << std::ends;
      throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
    }
  }
  FileStream.close();
}

HAL::ASCIIFileAddressTableContainer::~ASCIIFileAddressTableContainer() {
  std::tr1::unordered_map< std::string, HAL::VMEAddressTable* >::iterator vmeIt;
  for ( vmeIt = vmeAddressTableMap_.begin(); vmeIt != vmeAddressTableMap_.end(); vmeIt++ ) {
    delete ((*vmeIt).second);
  }
  
  std::tr1::unordered_map< std::string, HAL::PCIAddressTable* >::iterator pciIt;
  for ( pciIt = pciAddressTableMap_.begin(); pciIt != pciAddressTableMap_.end(); pciIt++ ) {
    delete ((*pciIt).second);
  }

}

HAL::VMEAddressTable& 
HAL::ASCIIFileAddressTableContainer::getVMETableFromSerialNumber( std::string serialNumber )
  throw( HAL::IllegalValueException ) {
  std::string typeId = moduleMapper_.getTypeId( serialNumber );
  std::tr1::unordered_map< std::string, HAL::VMEAddressTable* >::iterator itTables;
  if ( (itTables = vmeAddressTableMap_.find( typeId )) == vmeAddressTableMap_.end() ) {
      std::stringstream text;
      text << "No vme address table for serial number " << serialNumber
	   << " mapped to typeId " << typeId 
	   << "\n    (HAL::ASCIIFileAddressTableContainer::ASCIIFileAddressTableContainer)" << std::ends;
      throw (HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
  return *((*itTables).second);
}
