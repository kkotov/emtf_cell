#include "hal/VME64xMappedWindow.hh"
#include <sstream>
#include <iomanip>

HAL::VME64xMappedWindow::VME64xMappedWindow( uint32_t slotId,
					     uint32_t functionId,
					     uint32_t mappedWindowId, 
					     HAL::VMEConfigurationSpaceHandler* vmeConfigurationSpaceHandler )
  throw( HAL::NoSuchItemException, 
         HAL::IllegalOperationException,
         HAL::BusAdapterException,
         HAL::UnsupportedException,
         HAL::IllegalValueException )
  : slotId_( slotId ),
    functionId_( functionId ),
    mappedWindowId_( mappedWindowId ),
    vmeConfigurationSpaceHandlerRef_( vmeConfigurationSpaceHandler ) {
  
  // set the default values
  AM_ = 0;
  baseaddress_ = 0;
  ADER_ = 0;
  ADEM_ = 0;
  addressRank_ = 0;
  DAWPR_ = 0;
  canA32_ = false;
  canA24_ = false;
  canA16_ = false;
  dataAccessWidth_ = 0;
  configured_ = false;
  implemented_ = false;

  
  uint32_t amCapHigh, amCapLow;
  // retrieve raw function data from config space: The ADER is also read
  // in case one day we want to support fixed address functions (FAFs).
  std::stringstream ademItem;
  ademItem << "ADEM-F" << mappedWindowId;
  vmeConfigurationSpaceHandlerRef_->configRead(ademItem.str(), slotId, &ADEM_);
  std::stringstream widthItem;
  widthItem << "dataAccessWidth-F" << mappedWindowId;
  vmeConfigurationSpaceHandlerRef_->configRead(widthItem.str(), slotId, &DAWPR_);
  std::stringstream amcapLowItem;
  amcapLowItem << "AMCAP-F" << mappedWindowId << "-1";
  vmeConfigurationSpaceHandlerRef_->configRead(amcapLowItem.str(), slotId, &amCapHigh);
  std::stringstream amcapHighItem;
  amcapHighItem << "AMCAP-F" << mappedWindowId << "-0";
  vmeConfigurationSpaceHandlerRef_->configRead(amcapHighItem.str(), slotId, &amCapLow);

  // check if this function is implemented at all:
  if ( ADEM_ == 0x00 ) {
	return;
  }
  
  // decode the access width: 
  switch ( DAWPR_ ) {
  case 0x81:
  case 0x82:
	dataAccessWidth_ = 1;
	break;
  case 0x83:
	dataAccessWidth_ = 2;
	break;
  case 0x84:
	dataAccessWidth_ = 4;
	break;
  default:
	std::stringstream text;
	text << "DAWPR of " << std::hex << std::setw(2) << std::setfill('0') 
	     << DAWPR_ << " is not supported (slot " << std::dec << slotId_ << ")\n"
	     << "    (HAL::VME64xFunction::HAL::VME64xFunction)" << std::ends;
	throw( HAL::UnsupportedException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  
  // build the unsorted list of valid Address Modifiers
  for ( int ic=0; ic<32; ic++ ) {
	if ( amCapHigh & (1<<ic) ) {
      AMCAP_.push_back( ic+32 );
      setAddressCapability(ic+32);
    }
	if ( amCapLow & (1<<ic) ) {
      AMCAP_.push_back( ic );
      setAddressCapability(ic);
    }
  }
  
  // Sort the list. The list should be sorted according to the criteria 
  // of preference which one to use: During the mapping od Plug and Play
  // modules the software must determine which AM to use out of the ones
  // offered by the module. The sorting procedure will put the one which
  // is the most desirable as the first AM. It is a Functor which defines
  // the order of the AMs.
  
  //  HAL::AMSortFunctor amSortFunctor;
  AMCAP_.sort( HAL::AMSortFunctor() );
  // calculate rank of needed Address Space
  if ( ADEM_ & 8 ) {
	std::stringstream text;
	text << "Fixed Address Functions are not supported by this software (slot " 
	     << std::dec << slotId_ << ")\n"
	     << "    (HAL::VME64xFunction::HAL::VME64xFunction)" << std::ends;
	throw( HAL::UnsupportedException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
  if ( ADEM_ & 4 ) {
	std::stringstream text;
	text << "Dynamic Function sizing is not supported by this software (slot " 
	     << std::dec << slotId_ << ")\n"
	     << "   (HAL::VME64xFunction::HAL::VME64xFunction)" << std::ends;
	throw( HAL::UnsupportedException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
  if ( ADEM_ & 1 ) {
	std::stringstream text;
	text << "64(40) bit addresses are not supported by this software (slot " 
	     << std::dec << slotId_ << ")\n"
	     << "   (HAL::VME64xFunction::HAL::VME64xFunction)" << std::ends;
	throw( HAL::UnsupportedException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
  
  if ( (ADEM_ & (~0x0F)) == 0 ) {
	std::stringstream text;
	text << "No decoder-bits in ADEM set (slot " << std::dec << slotId_ 
	     << ")\n    HAL::VME64xFunction::HAL::VME64xFunction)" << std::ends;
	throw( HAL::IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }
  uint32_t memSize = (~(ADEM_ & (~0x0F))) + 1;
  addressRank_ = 8; // smallest possible rank according to spec.
  while ( ((1 << addressRank_) & memSize) == 0 ) {
	addressRank_++;
  }
  // do not forget:
  implemented_ = true;

  // if the module is enabled it means that it has been already configured
  // by somebody. Therefore the mapped windows must be read out of the ADER 
  // registers in order to avoid reconfiguration of this crate (It could be 
  // in use by another process.)
  if ( vmeConfigurationSpaceHandlerRef_->enabled( slotId_ ) ) {
	configFromHardware();
  } 
}

HAL::VME64xMappedWindow::VME64xMappedWindow( uint32_t slotId, 
					     uint32_t baseAddress,
					     uint32_t addressRank) 
  : slotId_(slotId),
    functionId_(0),
    mappedWindowId_(0),
    AMCAP_(0),
    baseaddress_(baseAddress),
    addressRank_(addressRank) {
  vmeConfigurationSpaceHandlerRef_ = (HAL::VMEConfigurationSpaceHandler*)0;
  AM_ = 0;
  ADEM_ = 0;
  DAWPR_ = 0;
  canA32_ = false;
  canA24_ = false;
  canA16_ = false;
  dataAccessWidth_ = 0;
  ADER_ = 0;
  implemented_ = true;
  configured_ = true;
}

HAL::VME64xMappedWindow::~VME64xMappedWindow() {
}

void HAL::VME64xMappedWindow::setAddressCapability( uint32_t AM ) {
  uint32_t code = AM & 0x38;
  if ( code == 0x38 ) {
    canA24_ = true;
  } else if( code == 0x28 ) {
    canA16_ = true;
  } else if( code == 0x08 ) {
    canA32_ = true;
  }
}

uint32_t HAL::VME64xMappedWindow::getDataAccessWidth() const {
  return dataAccessWidth_;
}

uint32_t HAL::VME64xMappedWindow::getAddressRank() const {
  return addressRank_;
}

uint32_t HAL::VME64xMappedWindow::getMappedWindowId() const {
  return mappedWindowId_;
}

uint32_t HAL::VME64xMappedWindow::getFunctionId() const {
  return functionId_;
}

uint32_t HAL::VME64xMappedWindow::getSlotId() const {
  return slotId_;
}

uint32_t HAL::VME64xMappedWindow::getAM() const {
  return AM_;
}

bool HAL::VME64xMappedWindow::canA32() const {
  return canA32_;
}

bool HAL::VME64xMappedWindow::canA24() const {
  return canA24_;
}

bool HAL::VME64xMappedWindow::canA16() const {
  return canA16_;
}

uint32_t HAL::VME64xMappedWindow::getBaseaddress() const {
  return baseaddress_;
}

bool HAL::VME64xMappedWindow::isConfigured() const {
  return configured_;
}

bool HAL::VME64xMappedWindow::isImplemented() const {
  return implemented_;
}

bool HAL::VME64xMappedWindow::hasAnotherWindow() const {
  return ((ADEM_ & 0x02) == 0x02);
}

const std::list< uint32_t>& HAL::VME64xMappedWindow::getSortedAMCAPList() const {
  return AMCAP_;
}

void HAL::VME64xMappedWindow::configFromHardware() 
  throw( HAL::NoSuchItemException, 
         HAL::IllegalOperationException,
         HAL::UnsupportedException,
	 HAL::BusAdapterException ) {
  
  // read from the hardware
  std::stringstream item;
  item << "ADER-F" << mappedWindowId_;
  vmeConfigurationSpaceHandlerRef_->configRead( item.str(), slotId_, &ADER_ );

  // if ADER is 0 it means that this window has not been configured. 
  // In this case the software should try to reconfigure the window. 
  // Therefore we return without setting the configured flag.
  if ( ADER_ == 0 ) return;

  // check if the XAM codes are in used (they are not supported in this software)
  if ( ADER_ & 0x00000001 ) {
	std::stringstream text;
	text << "XAM bit found set in ADER but XAM codes are not supported by this software (slot " 
	     << std::dec << slotId_ << ")\n"
	     << "   (HAL::VME64xFunction::configFromHardware)" << std::ends;
	throw( HAL::UnsupportedException( text.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
  AM_ = (ADER_ & 0xfc ) >> 2;
  baseaddress_ = ADER_ & 0xffffff00; 
  configured_ = true;
}

void HAL::VME64xMappedWindow::setADER( uint32_t baseAddress,
                                  uint32_t AM )
  throw( HAL::NoSuchItemException, 
         HAL::IllegalOperationException,
         HAL::BusAdapterException ) {
  
  if ( ! vmeConfigurationSpaceHandlerRef_ ) {
    std::stringstream text;
    text << "Cannot set ADER for Window without Configuration Space!"
         << "(slotId " << std::dec << slotId_ << ")" 
         << "\n    (HAL::VME64xMappedWindow::setADER)\n" << std::ends; 
    throw( HAL::IllegalOperationException( text.str(), __FILE__, __LINE__, __FUNCTION__ ) );
  }

  ADER_ = baseAddress + (AM << 2);

  // write it into the hardware
  std::stringstream item;
  item << "ADER-F" << mappedWindowId_;
  vmeConfigurationSpaceHandlerRef_->configWrite( item.str(), slotId_, ADER_ );
  AM_ = AM;
  baseaddress_ = baseAddress;
  configured_ = true;
}
