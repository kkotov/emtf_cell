#include "hal/VME64xCrate.hh"
#include <sstream>
#include <iomanip>
#include <list>

HAL::VME64xCrate::VME64xCrate( HAL::VMEBusAdapterInterface& busAdapter,
			       HAL::AddressTableContainerInterface& addressTableContainer,
			       HAL::ModuleMapperInterface& moduleMapper) 
  throw( HAL::HardwareProblemException,
         HAL::IllegalValueException,
	 HAL::UnsupportedException)
  : mapped_(false),
    busAdapter_ ( busAdapter ),
    addressTableContainer_( addressTableContainer ),
    moduleMapper_( moduleMapper ),
    vmeConfigSpaceHandler_( busAdapter_ ),
    slotPtrVector_(MAX_NUMBER_OF_SLOTS) {
  
  // Set default values for all pointers (this is used later in the 
  // function which populates the slots with plug and play modules: The 
  // plug and play algorithm is only tried in cases for which the pointer
  // to the slot Object is NULL.
  for ( int i=0; i<MAX_NUMBER_OF_SLOTS; i++ ) {
    slotPtrVector_[i] = (HAL::VMESlot*)0;
  }
  
  configurePlugAndPlay();
}

HAL::VME64xCrate::VME64xCrate( HAL::VMEBusAdapterInterface& busAdapter,
			       HAL::AddressTableContainerInterface& addressTableContainer,
			       HAL::ModuleMapperInterface& moduleMapper, 
			       const HAL::StaticVMEConfiguration& staticConfiguration )
  throw( HAL::HardwareProblemException,
         HAL::IllegalValueException,
	 HAL::UnsupportedException )
  : mapped_(false),
    busAdapter_ ( busAdapter ),
    addressTableContainer_( addressTableContainer ),
    moduleMapper_( moduleMapper ),
    vmeConfigSpaceHandler_( busAdapter_ ),
    slotPtrVector_(MAX_NUMBER_OF_SLOTS) {
  
  // Set default values for all pointers (this is used later in the 
  // function which populates the slots with plug and play modules: The 
  // plug and play algorithm is only tried in cases for which no static
  // configuration has been given. This allows to inhibit the plug and 
  // play mechanism for not correctly working modules by giving a static
  // configuration for the module. The initialization in addition allows 
  // to check that there are not two items in the static configuration
  // for the same slot.
  for ( int i=0; i<MAX_NUMBER_OF_SLOTS; i++ ) {
    slotPtrVector_[i] = (HAL::VMESlot*)0;
  }
  
  // Evaluate the static Configuration and populate the slots
  // which contain static VME configuration items.
  populateWithStaticConfiguration(staticConfiguration);

  configurePlugAndPlay();
}

void HAL::VME64xCrate::configurePlugAndPlay() 
  throw( HAL::IllegalValueException,
	 HAL::UnsupportedException ) {
  // check Amnesia address first
  if ( vmeConfigSpaceHandler_.containsVME64xModule( 0x1e ) ) {
    std::stringstream text;
    text << "Module at Amnesia address 0x1e found. Correct the hardware error first"
         << std::ends;
    throw( HAL::HardwareProblemException( text.str() ) );
  }

  // Populate the slots which contain plug and play modules. Afterwards there is 
  // a (poosible empty) HAL::VMESlot object for every possible slot.
  populateWithPlugAndPlay();

  // Build the memory Map of the crate:
  mapCrate();

  // Finally enable the VME64xModules
  enablePlugAndPlayModules();

  // sort the list of occupied address space
  occupiedItemList_.sort( MappedItemSorter() );
}

HAL::VMEDevice* HAL::VME64xCrate::getVMEDevice( uint32_t slotId ) const 
  throw ( HAL::IllegalOperationException,
	  HAL::IllegalValueException,
	  HAL::UnsupportedException,
	  HAL::BusAdapterException,
	  HAL::NoSuchItemException ) {
  if (slotPtrVector_[slotId]->getContents() != SlotContents(EMPTY) ) {
    return slotPtrVector_[slotId]->getVMEDevice();
  } 
  return (HAL::VMEDevice*)0;
} 


HAL::VMESlot* HAL::VME64xCrate::getVMESlot( uint32_t slotId ) const 
  throw (HAL::IllegalValueException) {
  if ( slotId > MAX_NUMBER_OF_SLOTS ) {
    std::stringstream text;
    text << "Illegal slot Id : " << std::dec << slotId
         << "  (must be less than " << MAX_NUMBER_OF_SLOTS << ")"
         << "\n    (HAL::VME64xCrate::getVMESlot)" << std::ends;
    throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  return slotPtrVector_[ slotId ];
}


void 
HAL::VME64xCrate::populateWithStaticConfiguration( const HAL::StaticVMEConfiguration& staticConfiguration ) 
  throw( HAL::IllegalValueException ) {
  HAL::VMESlot *newSlotPtr;
  std::list< HAL::StaticVMEItem* >::const_iterator it;
  for ( it = staticConfiguration.getListBegin();
        it != staticConfiguration.getListEnd();
        it++ ) {
    newSlotPtr = new HAL::VMESlot( *it,
				   &busAdapter_,
				   addressTableContainer_,
				   moduleMapper_ );
    // put the slot in the vector
    uint32_t slotId = (*it)->getSlotId();
    if ( slotPtrVector_[ slotId ] ) {
      std::stringstream text;
      text << "Static configuration contains more than one entry for slot number"
           << std::dec << slotId 
           << "\n    (HAL::VME64xCrate::populateWithStaticConfiguration)" << std::ends;
      throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
    }
    slotPtrVector_[ slotId ] = newSlotPtr;
  }
}


// only populate slots for which no static configuration has been given.
// This enables to give the configuration for not correctly designed modules 
// by hand. This method makes sure that there is a HAL::VMESlot object for each
// possible slot (it might be empty though)
void HAL::VME64xCrate::populateWithPlugAndPlay()
  throw ( HAL::IllegalValueException,
	  HAL::UnsupportedException ) {

  HAL::VMESlot *newSlotPtr;

  for ( int iSlot = 0; iSlot < MAX_NUMBER_OF_SLOTS; iSlot++ ) {
    if ( ! slotPtrVector_[ iSlot ] ) {
      newSlotPtr = new HAL::VMESlot( iSlot, 
                                &vmeConfigSpaceHandler_, 
                                addressTableContainer_,
                                moduleMapper_ );
      slotPtrVector_[iSlot] = newSlotPtr;
    }
  }
}

void HAL::VME64xCrate::getAllWindows() {
  std::vector< HAL::VMESlot* >::const_iterator it;
  for ( it = slotPtrVector_.begin(); it != slotPtrVector_.end(); it++ ) {
    std::list< HAL::VME64xFunction* > functions = (*it)->getImplementedFunctions();
    std::list< HAL::VME64xFunction* >::iterator funcIt;
    for( funcIt = functions.begin(); funcIt != functions.end(); funcIt++ ) {
      std::list<HAL::VME64xMappedWindow*> funcVectors = (*funcIt)->getMappedWindowPtrList();
      windowList_.insert( windowList_.end(), funcVectors.begin(), funcVectors.end() );
    }
  }
  WindowSorter sortFunctor;
  windowList_.sort(sortFunctor);
  makeOccupiedList();
}

// in principle one could map the A32 A24 A16 address spaces that they 
// overlap. But care should be taken since this could conflict with 
// standard vme modules which could respond to various address modifiers.
// So the most stupid but most save algorithm is to first detemine
// where the standard vme modules are and block the taken address space for
// all AMs. Then the VME64x modules can be mapped around them.
// 
void HAL::VME64xCrate::mapCrate() {
  // preparation: get a list of all windows mapped or to be mapped. 
  // sort this list according to the requested address-space.
  // then fill the occupiedItemList_ with those windows which belong
  // to standard VME modules. This is done in one function:
  getAllWindows( );
  // initialize constants:
  uint32_t A32Base = 0x00000000;
  uint32_t A24Base = 0x1000000;
  uint32_t A16Base = 0x10000;
  uint32_t A32Min = 0x1000000;
  uint32_t A24Min = 0x10000;
  uint32_t A16Min = 0x0;
  
  // loop over all windows starting with the ones which need most memory:
  // independently map the three address spaces
  // Static windoes are not mapped since they respond with false to all
  // the canAxx() requests.
  std::list< HAL::VME64xMappedWindow * >::reverse_iterator winIt;
  for ( winIt = windowList_.rbegin(); winIt != windowList_.rend(); winIt++ ) {
    if ( (*winIt)->canA32() ) {
      mapWindow( *winIt, A32Base, A32Min );
    } else if( (*winIt)->canA24() ) {
      mapWindow( *winIt, A24Base, A24Min );
    } else if( (*winIt)->canA16() ) {
      mapWindow( *winIt, A16Base, A16Min );
    }
  }
  mapped_ = true;
}

void HAL::VME64xCrate::mapWindow( HAL::VME64xMappedWindow* windowPtr, 
				  uint32_t base, 
				  uint32_t baseMin ) {
  uint32_t rank = windowPtr->getAddressRank();
  uint32_t space = 1 << rank;
  bool mapped = false;

  // simulate a 33rd bit in the address in order to get rid of problems in case of A32: 
  bool bit33 = true;
  while ( (!mapped) && ((base > space) || bit33)  && ((base - space) > baseMin) ) {
    base = base - space;
    if( ! isOccupied( base, space, bit33) ) {
      MappedItem *item = new MappedItem();
      item->baseAddress = base;
      item->startAddress = base;
      item->endAddress = base + space - 1;
      item->windowPtr = windowPtr;
      item->configured = true;
      occupiedItemList_.push_back( item );
      uint32_t am = windowPtr->getSortedAMCAPList().front();
      windowPtr->setADER( base, am);
      mapped = true;
    }
    bit33 = false;
  }
  if ( ! mapped ) {
    notMappedList_.push_back( windowPtr );
  }
}

/**
 * Figure out, if at a the address base there is enough "space" to map 
 * a window. Compare against all occupied windows in the crate.
 */
bool HAL::VME64xCrate::isOccupied( uint32_t base, uint32_t space, bool bit33 ) {
  std::list< struct MappedItem * >::const_iterator it ;
  // as soon as an item is found which overlaps with the proposed base address 
  // return true.
  for ( it=occupiedItemList_.begin(); it != occupiedItemList_.end(); it++ ) {
    //    cout << hex << base << " " << space << " " << bit33 << " " << (*it)->startAddress << " " << (*it)->endAddress << endl;

    // check for overlap: 1) check if the start address (=base)         is in the range between the start- and endaddress of item
    //                    2) check if the end   address (=base + space) is in the range between the start- and endaddress of item
    if ( ((base <= (*it)->endAddress ) && base >= (*it)->startAddress ) ||
         (((base + space - 1) <= (*it)->endAddress ) && (base + space - 1) >= (*it)->startAddress ) ) {
      //    if ( ((base <= (*it)->endAddress || bit33) && base >= (*it)->startAddress ) ||
      //         (((base + space - 1) <= (*it)->endAddress || bit33) && (base + space - 1) >= (*it)->startAddress ) ) {
      //cout << "true" << endl;
      return true;
    }
  }
  //cout << "false" << endl;
  return false;
}

// occupied windows are removed from the windowList_. 
// This is important in case that a crate is tried to be mapped
// more than once. (E.g. by different applications or processes)
void HAL::VME64xCrate::makeOccupiedList() {
  std::list< HAL::VME64xMappedWindow* > newList;
  std::list< HAL::VME64xMappedWindow* >::iterator it;
  struct MappedItem* item;
  for ( it = windowList_.begin(); it!= windowList_.end(); it++ ) {
    if ( (*it)->isConfigured() ) {
      item = new MappedItem();
      item->baseAddress = (*it)->getBaseaddress();
      item->startAddress = (*it)->getBaseaddress();
      item->endAddress = item->baseAddress + (1 << (*it)->getAddressRank()) - 1;
      item->windowPtr = (*it);
      item->configured = true;
      occupiedItemList_.push_back( item );
    } else {
      newList.push_back( *it );
    }
  }
  windowList_ = newList;
}

void HAL::VME64xCrate::enablePlugAndPlayModules() 
  throw (HAL::IllegalOperationException ) {
  if ( ! mapped_ ) {
    std::stringstream text;
    text << "The crate has not yet mapped the memory space and therefore the\n"
         << "modules cannot be enabled!\n    (HAL::VME64xCrate::enablePlugAndPlayModules)" 
         << std::ends;
    throw( HAL::IllegalOperationException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  std::vector< HAL::VMESlot* >::iterator it;
  for ( it=slotPtrVector_.begin(); it != slotPtrVector_.end(); it++ ) {
    if ((*it)->getContents() == (SlotContents)VME64x ) {
      vmeConfigSpaceHandler_.enableVME64xModule( (*it)->getSlotId() );
    }
  }
}

void HAL::VME64xCrate::printAddressMap( std::ostream& out ) const {
  if ( notMappedList_.size() > 0 ) {
    out << "\n\nWARNING: the following items could not be mapped\n";
    std::list< const HAL::VME64xMappedWindow* >::const_iterator itN;
    for( itN = notMappedList_.begin(); itN != notMappedList_.end(); itN++ ) {
      out << "    slot Id     : " << (*itN)->getSlotId()
          << "    function Id : " << (*itN)->getFunctionId()
          << "    map Id      : " << (*itN)->getMappedWindowId();
    }
  }

  out << "\n\nList of mapped items (ordered by address space size) :\n\n";
  std::list< struct MappedItem * >::const_iterator itM;
  for ( itM = occupiedItemList_.begin(); itM!= occupiedItemList_.end(); itM++ ) {
    uint32_t slotId     = (*itM)->windowPtr->getSlotId();
    uint32_t functionId = (*itM)->windowPtr->getFunctionId();
    uint32_t mapId      = (*itM)->windowPtr->getMappedWindowId();
    uint32_t AM         = (*itM)->windowPtr->getAM();
    std::string serialNumber = slotPtrVector_[ slotId ]->getSerialNumber();
    std::string typeId = slotPtrVector_[ slotId ]->getTypeId();

    // This is necessary since somehow setw gets confused if there are words containing 
    // digits or +- signs. (It seems at least...)
    std::string text;
    text = "         |             type : " + typeId + "      serial : " + serialNumber;
    uint32_t size = text.length();
    int diff = 78-size;
    for ( ; diff > 0; diff-- ) text += " ";
    text += "|";
    
    out << std::hex << std::setw(8) << std::setfill('0') << (*itM)->endAddress;
    out <<" ---------------------------------------------------------------------\n"
        <<"         |                                                                    |\n"
	<< text << "\n"
	<<"         |                                                                    |\n"
        <<"         |                      slot ID : " << std::setw(2) << slotId << "                                  |\n"
        <<"         |                     function : " << std::setw(2) << functionId << "                                  |\n"
        <<"         |                       map ID : " << std::setw(2) << mapId << "                                  |\n"
        <<"         |                           AM : " << std::setw(2) << std::setfill('0') << AM << "                                  |\n"
        <<"         |                                                                    |\n"
        << std::hex << std::setw(8) << std::setfill('0') << (*itM)->startAddress
        <<" ---------------------------------------------------------------------\n";
  }
  out << "\n\n" << std::endl;
}
