#include "hal/VMESlot.hh"
#include "hal/VME64xDevice.hh"
#include <sstream>
#include <iomanip>

HAL::VMESlot::VMESlot( HAL::StaticVMEItem* staticVME,
		       HAL::VMEBusAdapterInterface* busAdapterPtr,
		       HAL::AddressTableContainerInterface& addressTableContainer,
		       HAL::ModuleMapperInterface& moduleMapper )
  throw( HAL::IllegalValueException )
  : busAdapterPtr_(busAdapterPtr),
    addressTableContainer_(addressTableContainer),
    moduleMapper_(moduleMapper),
    windowVector_(NUMBER_OF_VME64XFUNCTIONS) {

  // initialization of variables
  configAdapterPtr_ = (HAL::VMEConfigurationSpaceHandler*)0;
  for ( int i=0; i<NUMBER_OF_VME64XFUNCTIONS; i++ ) {
    windowVector_.push_back( (HAL::VME64xMappedWindow*)0 );
  }
  slotId_ = staticVME->getSlotId();
  serialNumber_ = staticVME->getSerialNumber();
  contents_ = (HAL::SlotContents)VME;

  // get database info: typeId and baseaddress
  uint32_t baseAddress = moduleMapper.getBaseaddress( serialNumber_ );
  typeId_ = moduleMapper.getTypeId( serialNumber_ );

  // get HAL::AddressTable and related info
  HAL::VMEAddressTable& addressTable = addressTableContainer_.getVMETableFromSerialNumber( serialNumber_ );
  std::vector<uint32_t> minAddresses, maxAddresses;
  addressTable.getAddressBoundaries( minAddresses, maxAddresses );

  // for standard vme modules only the first entry in the vectors is relevant:
  // the minimal addressRank in VME64x is 256 bytes Therefore we start
  // with that addressRank here.
  uint32_t addressRank = 8;
  while ( (1u<<(addressRank)) <= maxAddresses[0] ) addressRank++;

  HAL::VME64xFunction* functionPtr = new HAL::VME64xFunction( slotId_, baseAddress, addressRank );
  functionPtrs_.push_back( functionPtr );

  // fill the windowVector of the slot

  std::list< HAL::VME64xMappedWindow* >::const_iterator it;
  it = functionPtr->getMappedWindowPtrList().begin();
  windowVector_[0] = *it;
  
}

HAL::VMESlot::VMESlot(  uint32_t slotId,
			HAL::VMEConfigurationSpaceHandler* configAdapterPtr,
			HAL::AddressTableContainerInterface& addressTableContainer,
			HAL::ModuleMapperInterface& moduleMapper,
			bool ignoreChecksumError )
  throw(HAL::IllegalValueException,
        HAL::UnsupportedException )
  : slotId_(slotId),
    configAdapterPtr_(configAdapterPtr),
    addressTableContainer_(addressTableContainer),
    moduleMapper_(moduleMapper_),
    windowVector_(NUMBER_OF_VME64XFUNCTIONS) {

  // initialization of variables
  busAdapterPtr_ = &(configAdapterPtr->getBusAdapter());
  for ( int i=0; i<NUMBER_OF_VME64XFUNCTIONS; i++ ) {
    windowVector_.push_back( (HAL::VME64xMappedWindow*)0 );
  }
  
  typeId_ = "";
  serialNumber_ = "";

  // If there is no VME64x module we assume the slot is empty.
  if ( ! configAdapterPtr->containsVME64xModule( slotId ) ) {
    contents_ = (HAL::SlotContents)EMPTY;
    return;
  } else {
    if ( (! ignoreChecksumError) && (! configAdapterPtr->checksumOk( slotId )) ) {
      std::stringstream text;
      text << "Found VME64x module in slot " << std::dec << slotId
           << " but checksum is corrupted" << std::ends;
	  throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
	}
    contents_ = (HAL::SlotContents)VME64x;
  }

  // get the serial Number of this module
  // it is needed later when the HAL::VMEDevice is created.
  // in case of old VME modules it is set from the configration
  // given in the constructor:
  serialNumber_ = configAdapterPtr_->readSerialNumber( slotId_ );
  typeId_ = moduleMapper.getTypeId( serialNumber_ );
  
  // get the functions of the module in this slot:
  HAL::VME64xFunction* functionPtr;
  uint32_t funcId = 0;
  bool atLeastOneValidFunction = false;

  // Go through all possible functions and see if they are implemented.
  // Implemented functions are put into the list fucntionPtrs_
  // and all mapped windows are put into the windowVector_.
  while ( funcId < NUMBER_OF_VME64XFUNCTIONS ) {
    if ( configAdapterPtr_->functionIsImplemented( slotId_, funcId )) {
      functionPtr =  new HAL::VME64xFunction( slotId_, 
					      funcId,
					      configAdapterPtr_ );
      functionPtrs_.push_back( functionPtr );
      atLeastOneValidFunction = true;
      funcId += functionPtr->getNumberOfMappedWindows();

      // fill the windowVector of the slot

      std::list< HAL::VME64xMappedWindow* >::const_iterator it;
      std::list< HAL::VME64xMappedWindow* > windowList = functionPtr->getMappedWindowPtrList();
      for( it = windowList.begin(); it != windowList.end(); it++ ) {
        windowVector_[(*it)->getMappedWindowId()] = *it;
      }
    } else {
      funcId++;
    }      
  }

  if ( ! atLeastOneValidFunction ) {
    std::stringstream text;
    text << "No valid function found in VME64x module of slot "
         << std::dec << slotId_ 
         << ".\n    (HAL::VMESlot::VMESlot)" << std::ends;
 
    // clean up what we have generated before throwing an exception.

    std::list< HAL::VME64xFunction* >::iterator it;
    for ( it=functionPtrs_.begin(); it != functionPtrs_.end(); it++ ) {
      delete *it;
    }
    functionPtrs_.clear();
    throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }

}

HAL::VMESlot::~VMESlot() {
  std::list< HAL::VME64xFunction* >::iterator it;
  for ( it = functionPtrs_.begin(); it != functionPtrs_.end(); it++) {
    delete *it;
  }
}

enum HAL::SlotContents HAL::VMESlot::getContents() const {
  return contents_;
}

std::string HAL::VMESlot::getTypeId() const {
  return typeId_;
}

std::string HAL::VMESlot::getSerialNumber() const {
  return serialNumber_;
}

uint32_t HAL::VMESlot::getSlotId() const {
  return slotId_;
}


HAL::VMEDevice* HAL::VMESlot::getVMEDevice() const 
  throw( HAL::IllegalOperationException,
         HAL::IllegalValueException,
	 HAL::UnsupportedException,
	 HAL::BusAdapterException,
	 HAL::NoSuchItemException ) {
  
  HAL::VMEDevice* module;

  if ( contents_ == EMPTY ) {
    std::stringstream text;
    text << "This slot (" << std::dec << slotId_ 
	 << "is empty and contains no module" 
	 << "\n    (HAL::VMESlot::getVMEDevice)" << std::ends;
    throw( HAL::IllegalOperationException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  
  HAL::VMEAddressTable& addressTable = 
    addressTableContainer_.getVMETableFromSerialNumber( serialNumber_ );

  // VME64x module:
  if ( contents_ == VME64x ) { 

    checkVME64xTable( addressTable );
    // try to build the module if not yet there:
    std::vector <uint32_t> baseaddresses(NUMBER_OF_VME64XFUNCTIONS);
    for ( int ic=0; ic<NUMBER_OF_VME64XFUNCTIONS; ic++ ) {
      if (windowVector_[ic]) {
	baseaddresses[ic] = windowVector_[ic]->getBaseaddress();
      } else {
	baseaddresses[ic] = 0;
      }
    }

    module = new HAL::VME64xDevice( addressTable,
				    *busAdapterPtr_,
				    baseaddresses,
				    false,
				    *this );

    // normal VME module:
  } else {
    uint32_t baseaddress = moduleMapper_.getBaseaddress( serialNumber_ );
    module = new HAL::VMEDevice( addressTable,
				 *busAdapterPtr_,
				 baseaddress );
  }
  
  return module;
}


// nothing should go wrong here since if the constructor survived
// there should be 8 valid VME64Function* in the functionPtrs_.
std::list< HAL::VME64xFunction* > HAL::VMESlot::getImplementedFunctions() const {
  std::list< HAL::VME64xFunction* > result;
  std::list< HAL::VME64xFunction* >::const_iterator it;
  for ( it = functionPtrs_.begin(); it != functionPtrs_.end(); it++ ) {
	if ( (*it)->isImplemented() ) {
	  result.push_back( *it );
	}
  }
  return result;
}

void HAL::VMESlot::checkVME64xTable( HAL::VMEAddressTable& table ) const 
  throw (HAL::IllegalValueException) {

  // First go through the items of the table and see if the AMs in the
  // table are really offered by the corresponding function. In case 
  // of standard VME modules there is one single function implemented of which
  // the configuration-settings are given to this class in the constructor.
  // This might have been done already during the mapping of the crate. 
  // But it depends on the chosen strategy so it is better to check it 
  // here once just before the HAL::VMEDevice is created. 
  uint32_t  mapId; 
  std::tr1::unordered_map<std::string, HAL::AddressTableItem*, HAL::HalHash<std::string> >::const_iterator it;
  for ( it=table.getItemListBegin(); it != table.getItemListEnd(); it++ ) {
	mapId = (*it).second->getGeneralHardwareAddress().getMapId();

	// is the function implemented and configured ?
	if ( ! ( windowVector_[mapId] && windowVector_[mapId]->isConfigured() ) ) {
	  std::stringstream text;
	  text << "HAL::AddressTableItem " << (*it).first
	       << " uses non-implemented or not-configured function " << std::dec << mapId
	       << ".\n    (HAL::VMESlot::checkVME64xTable)" << std::ends;
	  throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
	}

    // configure the HardwareAddress to work with the correct AM and dataWidth:

    uint32_t AM = windowVector_[mapId]->getAM();
    uint32_t width = windowVector_[mapId]->getDataAccessWidth();
    try {
      HAL::VME64xHardwareAddress& vme64xAddress = 
	dynamic_cast< HAL::VME64xHardwareAddress& >((*it).second->getGeneralHardwareAddress());

      // nothing needs to be done for configspace since the am is already set correctly in the 
      // constructor of the vme64xhardwareaddress and the width has also been set already.
      if ( vme64xAddress.isMemorySpace() ) {
 	vme64xAddress.setWindowConfiguration( AM, width );
      }
    } catch ( std::bad_cast ) {
      std::stringstream text;
      text << "The address table does not contain VME64x items!" 
	   << "\n    (HAL::VMESlot::checkVME64xTable)" << std::ends;
      throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
    }
  }


  // now that the data widths have been set for all items in the 
  // address table the boundaries for the various base addresses
  // have to be redetermined.
  table.determineAddressBoundaries();

  // Now check the limits of the address tables against what is 
  // demanded by the functions in the hardware. This test only has
  // to be done for VME64x modules. It must be done after the data 
  // widths have been set for the various address map items since the
  // width is needed in the calculation of address boundaries in the
  // call "table.getAddressBoundaries".
  if ( contents_ == VME64x ) {
    std::vector< uint32_t > minAddresses, maxAddresses;
    table.getAddressBoundaries( minAddresses, maxAddresses );

    // loop over all functions of this slot
    std::list< HAL::VME64xFunction* >::const_iterator itf;
    for ( itf = functionPtrs_.begin(); itf != functionPtrs_.end(); itf++ ) {

      // loop over all mapped windows of the function
      std::list< HAL::VME64xMappedWindow* > windowList = (*itf)->getMappedWindowPtrList();
      std::list< HAL::VME64xMappedWindow* >::const_iterator itw;
      for ( itw = windowList.begin(); itw != windowList.end(); itw++ ) {

        uint32_t windowDemand = 1 << (*itw)->getAddressRank();
        uint32_t windowId = (*itw)->getMappedWindowId();

        if ( windowDemand <= maxAddresses[windowId] ) {
        std::stringstream text;
        text << "mapped window number " << std::dec << windowId 
             << " demands less memory than required in HAL::AddressTable "
             << "(hex):\n" << std::hex << std::setw(8) << std::setfill('0') << windowDemand
             << " <= " <<  maxAddresses[windowId] 
             << "\n    (HAL::VMESlot::checkVME64xTable)" << std::ends;
        throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
        }
      }
    }
  }

}


void HAL::VMESlot::stdConfigRead( std::string item,
				  uint32_t* resultPtr,
				  uint32_t offset ) const
  throw( HAL::NoSuchItemException, 
	 HAL::IllegalOperationException,
	 HAL::BusAdapterException )
{
  if ( ! configAdapterPtr_ ) 
    {
      std::stringstream text;
      text << "No ConfigurationSpace Adapter available for slot " << std::dec << slotId_ 
             << " .\nProbbably this means that there is no VME64x modules plugged into this slot."
             << std::ends;
         throw( HAL::IllegalOperationException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));     
    }
  configAdapterPtr_->configRead( item, slotId_, resultPtr, offset );
}
  
void HAL::VMESlot::stdConfigWrite( std::string item,
				   uint32_t data,
				   HalVerifyOption verifyFlag,
				   uint32_t offset ) const
  throw( HAL::NoSuchItemException, 
	 HAL::IllegalOperationException,
	 HAL::BusAdapterException )
{
  if ( ! configAdapterPtr_ ) 
    {
      std::stringstream text;
      text << "No ConfigurationSpace Adapter available for slot " << std::dec << slotId_ 
             << " .\nProbbably this means that there is no VME64x modules plugged into this slot."
             << std::ends;
         throw( HAL::IllegalOperationException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));     
    }
  configAdapterPtr_->configWrite( item, slotId_, data, verifyFlag, offset );
}
