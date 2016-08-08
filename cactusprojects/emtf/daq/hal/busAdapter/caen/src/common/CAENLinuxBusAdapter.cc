#include "hal/CAENLinuxBusAdapter.hh"
#include "CAENVMElib.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string.h>


int32_t HAL::CAENLinuxBusAdapter::handleVX718UseCount_[16][8] = {{0,0,0,0,0,0,0,0},
								{0,0,0,0,0,0,0,0},
								{0,0,0,0,0,0,0,0},
								{0,0,0,0,0,0,0,0},
								{0,0,0,0,0,0,0,0},
								{0,0,0,0,0,0,0,0},
								{0,0,0,0,0,0,0,0},
								{0,0,0,0,0,0,0,0},
								{0,0,0,0,0,0,0,0},
								{0,0,0,0,0,0,0,0},
								{0,0,0,0,0,0,0,0},
								{0,0,0,0,0,0,0,0},
								{0,0,0,0,0,0,0,0},
								{0,0,0,0,0,0,0,0},
								{0,0,0,0,0,0,0,0},
								{0,0,0,0,0,0,0,0}};
int32_t HAL::CAENLinuxBusAdapter::handleVX718_[16][8] = {{0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0}};
sem_t*  HAL::CAENLinuxBusAdapter::handleSemaphore_ = 0;

/**
 * It has been decided not to use polymorphism and different derived 
 * classes for the usb and the optical version of the module since the
 * benefit would have been small. Having a parameter in the constructor
 * to let the user choose between the models makes life slightly easier 
 * for implementors which often swap usb module with optical modules in
 * the laboratory.
 */
HAL::CAENLinuxBusAdapter::CAENLinuxBusAdapter( CAENModel model, int unit, int chain, CAENPCCard pcCard )
    throw (BusAdapterException)
    : unit_(unit),
      chain_(chain),
      model_(model)
{

    int status;
    int32_t handle;

    // Initial check of parameters
    if ( unit > 15 || unit < 0 || chain > 7 || chain < 0 ) {
      std::stringstream errorMessage;
      errorMessage << "unit and chain parameters must be between 0 and 7 but they are " 
		   << unit << " and " << chain << "." << std::ends;
      throw (BusAdapterException( errorMessage.str(), __FILE__, __LINE__, __FUNCTION__ ) );
    }

    if ( handleSemaphore_ == 0 ) {
      handleSemaphore_ = new sem_t;
      sem_init( handleSemaphore_, 0, 0 );
    } else {
      sem_wait( handleSemaphore_ );
    }

    V1718FwRelease_[0] = char(0);
    V2718FwRelease_[0] = char(0);
    AX818FwRelease_[0] = char(0);
    A2719FwRelease_[0] = char(0);
    VMELibRelease_[0]  = char(0);
  
    if ( pcCard == A3818 ) {
      pcCard_ = cvA3818;
      pcCardStr_ = "A3818";
    } else if ( pcCard == A2818 ) {
      pcCard_ = cvA2818;
      pcCardStr_ = "A2818";
    } else {
      // a software bug
      throw (BusAdapterException( "A HAL software bug: illegal pc card identifier set", 
                                  __FILE__, __LINE__, __FUNCTION__ ) );
    }

    // collect all the firmware and software versions
    
    if ( model == V2718 ) {     // model V2718

      // firmware version of the PCI board 
      if( (status=CAENVME_Init(pcCard_, unit_, 0, &handle)) != cvSuccess ) {
           std::stringstream errorMessage;
           errorMessage << "Could not initialize CAENVME library for PC card (status: "
                        << status 
                        << ")"
                        << std::ends;
	   sem_post(handleSemaphore_);
	   throw (BusAdapterException( errorMessage.str(), __FILE__, __LINE__, __FUNCTION__ ) );
       }
       if ( ( status = CAENVME_BoardFWRelease( handle, AX818FwRelease_ )) != cvSuccess ) {
           std::stringstream errorMessage;
           errorMessage << "Could not determine firmware version of " << pcCardStr_ << " (status: "
                        << status 
                        << ")"
                        << std::ends;
	   sem_post(handleSemaphore_);
	   throw (BusAdapterException( errorMessage.str(), __FILE__, __LINE__, __FUNCTION__ ) );
      }
       
        CAENVME_End( handle );
    
        // // The following must not be done since it sets the optical link add-on board
	// // in service mode: If at the same time another process tries to access the
	// // VME bus via the same optical link, it will get an error since the packet 
	// // gets stuck in the optical link board which is in service mode.
	// 
	// // firmware version of the optical link add-on board
	// if( (status=CAENVME_Init(cvA2719, unit_, 0, &handle_)) != cvSuccess ) {
	//   std::stringstream errorMessage;
	//   errorMessage << "Could not initialize CAENVME library with cvA2719 (status: "
	// 	       << status 
	// 	       << ")"
	// 	       << std::ends;
	//   throw (BusAdapterException( errorMessage.str(), __FILE__, __LINE__, __FUNCTION__ ) );
	// }
	// if ( ( status = CAENVME_BoardFWRelease( handle_, A2719FwRelease_ )) != cvSuccess ) {
	//   std::stringstream errorMessage;
	//   errorMessage << "Could not determine firmware version of cvA2719 (status: "
	// 	       << status 
	// 	       << ")"
	// 	       << std::ends;
	//   throw (BusAdapterException( errorMessage.str(), __FILE__, __LINE__, __FUNCTION__ ) );
	// }
	// CAENVME_End( handle_ );
	strcpy( A2719FwRelease_, "not available" ) ;
	
        // firmware version of the VME Controller board (optical link version)
	// This process must only call Init once!!!
	if ( handleVX718UseCount_[unit_][chain_] == 0 ) {
	  if( (status=CAENVME_Init(cvV2718, unit_, chain_, &handleVX718_[unit_][chain_])) != cvSuccess ) {
	    std::stringstream errorMessage;
	    errorMessage << "Could not initialize CAENVME library with cvV2718 (status: "
			 << status 
			 << ")"
			 << std::ends;
	    sem_post(handleSemaphore_);
	    throw (BusAdapterException( errorMessage.str(), __FILE__, __LINE__, __FUNCTION__ ) );
	  }
	}

        if ( ( status = CAENVME_BoardFWRelease( handleVX718_[unit_][chain_], V2718FwRelease_ )) != cvSuccess ) {
	  std::stringstream errorMessage;
	  errorMessage << "Could not determine firmware version of V2718 (status: "
		       << status 
		       << ")"
		       << std::ends;
	  sem_post(handleSemaphore_);
	  throw (BusAdapterException( errorMessage.str(), __FILE__, __LINE__, __FUNCTION__ ) );
        }	
        
    } else {   // V1718
      
      
      // firmware of the VME Controller board (USB version)
      if ( handleVX718UseCount_[unit_][chain_] == 0 ) {
	if( (status=CAENVME_Init(cvV1718, unit_, 0, &handleVX718_[unit_][chain_])) != cvSuccess ) {
	  std::stringstream errorMessage;
	  errorMessage << "Could not initialize CAENVME library with cvV1718 (status: "
		       << status 
		       << ")"
		       << std::ends;
	  sem_post(handleSemaphore_);
	  throw (BusAdapterException( errorMessage.str(), __FILE__, __LINE__, __FUNCTION__ ) );
	}
      }
      if ( ( status = CAENVME_BoardFWRelease( handleVX718_[unit_][chain_], V1718FwRelease_ )) != cvSuccess ) {
	  std::stringstream errorMessage;
	  errorMessage << "Could not determine firmware version of V1718 (status: "
		       << status 
		       << ")"
		       << std::ends;
	  sem_post(handleSemaphore_);
	  throw (BusAdapterException( errorMessage.str(), __FILE__, __LINE__, __FUNCTION__ ) );	  
      }
    }
    CAENVME_SWRelease( VMELibRelease_ );
    handleVX718UseCount_[unit_][chain_]++;

    sem_post(handleSemaphore_);

}




HAL::CAENLinuxBusAdapter::~CAENLinuxBusAdapter() 
    throw( BusAdapterException ){

  sem_wait( handleSemaphore_ );
  handleVX718UseCount_[unit_][chain_]--;


  if ( handleVX718UseCount_[unit_][chain_] == 0 ) {
    // we are the last user: close down everything
    if (CAENVME_End( handleVX718_[unit_][chain_] ) != cvSuccess ) {
        std::string errorMessage( "Could not close the CAENVME library" );
	sem_post( handleSemaphore_ );
        throw (BusAdapterException( errorMessage, __FILE__, __LINE__, __FUNCTION__ ) );
    }
    handleVX718_[unit_][chain_] = 0;
    sem_post( handleSemaphore_ );
    sem_destroy( handleSemaphore_ );
    handleSemaphore_ = 0;
    delete handleSemaphore_;
 
 } else {

   // somebody else is still using the busAdapter 
   sem_post( handleSemaphore_ );

  }
}



void HAL::CAENLinuxBusAdapter::openDevice(const VMEAddressTable& vmeAddressTable,
                                     uint32_t vmeBaseAddress,
                                     DeviceIdentifier** deviceIdentifierPtr,
                                     uint32_t* baseAddressPtr,
                                     bool doSwapping ) 
    throw (BusAdapterException) {
  
    *deviceIdentifierPtr= new HAL::CAENDeviceIdentifier( handleVX718_[unit_][chain_],
							 doSwapping );

    *baseAddressPtr = vmeBaseAddress;
}



void HAL::CAENLinuxBusAdapter::openDevice(const VMEAddressTable& vmeAddressTable,
                                     std::vector<uint32_t> vmeBaseAddresses,
                                     DeviceIdentifier** deviceIdentifierPtr,
                                     std::vector<uint32_t>* baseAddressesPtr,
                                     bool doSwapping ) 
    throw (BusAdapterException) {
  
    *deviceIdentifierPtr= new HAL::CAENDeviceIdentifier( handleVX718_[unit_][chain_],
							 doSwapping );
    
    *baseAddressesPtr = vmeBaseAddresses;
}




void HAL::CAENLinuxBusAdapter::closeDevice( DeviceIdentifier* vmeDevice ) 
    throw() {
    delete(vmeDevice);
}



void HAL::CAENLinuxBusAdapter::read( DeviceIdentifier* vmeDevice, 
                                uint32_t address,
                                uint32_t addressModifier,
                                uint32_t dataWidth,
                                uint32_t *resultPtr )
    throw( BusAdapterException ) {

    // The CAEN Adapter now supports byte swapping
    if ( dynamic_cast<HAL::CAENDeviceIdentifier*>(vmeDevice)->doSwapping() &&
         dataWidth > 1 ) {
        dataWidth += 16;
    }

    if( CAENVME_ReadCycle( handleVX718_[unit_][chain_], address, 
                           (void*)resultPtr, 
                           (CVAddressModifier)addressModifier, 
                           (CVDataWidth)dataWidth ) != cvSuccess ) {
        std::string errorMessage( "Could not read from CAEN BusAdapter" );
        throw (BusAdapterException( errorMessage, __FILE__, __LINE__, __FUNCTION__ ) );    
    }
}

void HAL::CAENLinuxBusAdapter::write( DeviceIdentifier* vmeDevice, 
                                 uint32_t address, 
                                 uint32_t addressModifier,
                                 uint32_t dataWidth,
                                 uint32_t data)
    throw( BusAdapterException ) {

    // The CAEN Adapter now supports byte swapping
    if ( dynamic_cast<HAL::CAENDeviceIdentifier*>(vmeDevice)->doSwapping() &&
         dataWidth > 1 ) {
        dataWidth += 16;
    }

    if( CAENVME_WriteCycle( handleVX718_[unit_][chain_], address, 
                            (void*)(&data), 
                            (CVAddressModifier)addressModifier, 
                            (CVDataWidth)dataWidth ) != cvSuccess ) {
        std::string errorMessage( "Could not read from CAEN BusAdapter" );
        throw (BusAdapterException( errorMessage, __FILE__, __LINE__, __FUNCTION__ ) );    
    }
}

void HAL::CAENLinuxBusAdapter::resetBus( ) 
    throw(BusAdapterException) {

    if (CAENVME_SystemReset( handleVX718_[unit_][chain_] )  != cvSuccess ) {
        throw( BusAdapterException( "Cannot issue sysReset on VME crate", __FILE__, __LINE__, __FUNCTION__ ));
    }
}

void HAL::CAENLinuxBusAdapter::readBlock( DeviceIdentifier *vmeDevice,
                                     uint32_t startAddress,
                                     uint32_t length,      // in bytes
                                     uint32_t addressModifier,
                                     uint32_t dataWidth,
                                     char *buffer,
                                     HalAddressIncrement addressBehaviour )
    throw( BusAdapterException,
	   UnsupportedException ){

    int actualTransferred, status;

    // The CAEN Adapter now supports byte swapping
    if ( dynamic_cast<HAL::CAENDeviceIdentifier*>(vmeDevice)->doSwapping() &&
         dataWidth > 1 ) {
        dataWidth += 16;
    }

    if ( addressBehaviour != HAL_DO_INCREMENT ) {

      if ( (status = CAENVME_FIFOBLTReadCycle(handleVX718_[unit_][chain_], startAddress, (unsigned char *)buffer, 
					      (int) length, (CVAddressModifier) addressModifier, 
					      (CVDataWidth) dataWidth, &actualTransferred))  != cvSuccess ) {

        throw( BusAdapterException( "Error during read block transfer (FIFO mode)", __FILE__, __LINE__, __FUNCTION__ ));

      }

    } else {

      if ( (status = CAENVME_BLTReadCycle(handleVX718_[unit_][chain_], startAddress, (unsigned char *)buffer, 
					  (int) length, (CVAddressModifier) addressModifier, 
					  (CVDataWidth) dataWidth, &actualTransferred))  != cvSuccess ) {
	
        throw( BusAdapterException( "Error during read block transfer", __FILE__, __LINE__, __FUNCTION__ ));
      }
    }
  
    if ( (uint32_t)actualTransferred != length ) {
        std::stringstream message;
        message << "Block transfer has only transferred " << std::dec << actualTransferred 
                << " bytes instead of the requested " << length
                << std::ends;
        throw( BusAdapterException( message.str(), __FILE__, __LINE__, __FUNCTION__ ));
    }
}

void HAL::CAENLinuxBusAdapter::writeBlock( DeviceIdentifier *vmeDevice,
                                      uint32_t startAddress,
                                      uint32_t length,      // in bytes
                                      uint32_t addressModifier,
                                      uint32_t dataWidth,
                                      char *buffer,
                                      HalAddressIncrement addressBehaviour )
    throw( BusAdapterException,
	   UnsupportedException ){

    int actualTransferred, status;

    // The CAEN Adapter now supports byte swapping
    if ( dynamic_cast<HAL::CAENDeviceIdentifier*>(vmeDevice)->doSwapping() &&
         dataWidth > 1 ) {
        dataWidth += 16;
    }

     if ( addressBehaviour != HAL_DO_INCREMENT ) {

       if ( (status = CAENVME_FIFOBLTWriteCycle(handleVX718_[unit_][chain_], startAddress, (unsigned char *)buffer, 
						(int) length, (CVAddressModifier) addressModifier, 
						(CVDataWidth) dataWidth, &actualTransferred))  != cvSuccess ) {
	 
	 throw( BusAdapterException( "Error during write block transfer (FIFO mode)", __FILE__, __LINE__, __FUNCTION__ ));
       }

     } else {

       if ( (status = CAENVME_BLTWriteCycle(handleVX718_[unit_][chain_], startAddress, (unsigned char *)buffer, 
					    (int) length, (CVAddressModifier) addressModifier, 
					    (CVDataWidth) dataWidth, &actualTransferred))  != cvSuccess ) {
    
	 throw( BusAdapterException( "Error during write block transfer", __FILE__, __LINE__, __FUNCTION__ ));
       }

     }
  
     if ( (uint32_t)actualTransferred != length ) {
       std::stringstream message;
       message << "Block transfer has only transferred " << std::dec << actualTransferred 
	       << " bytes instead of the requested " << length
	       << std::ends;
        throw( BusAdapterException( message.str(), __FILE__, __LINE__, __FUNCTION__ ));
     }
}

void HAL::CAENLinuxBusAdapter::readVersions( std::ostream& os ) const {
    if ( model_ == V2718 ) {
        os << "V2718 firmware : " << V2718FwRelease_ << "\n"
	   << pcCardStr_ << " firmware : " << AX818FwRelease_ << "\n"
	  //<< "A2719 firmware : " << A2719FwRelease_ << "\n"
           << "VMELibRelease  : " << VMELibRelease_  << "\n" 
           << std::ends;
    } else {
        os << "V1718 firmware : " << V1718FwRelease_ << "\n" << std::ends;
    }
}

void HAL::CAENLinuxBusAdapter::readDisplay(  std::ostream& os ) const
    throw( BusAdapterException ) {

    CVDisplay displayData;
    int status;
    if ( (status = CAENVME_ReadDisplay(handleVX718_[unit_][chain_], &displayData)) != cvSuccess ) {
        throw( BusAdapterException( "Could not read display.", __FILE__, __LINE__, __FUNCTION__ ));
    }

    os << "Address : " << std::hex << std::setw(8) << std::setfill('0') << displayData.cvAddress << std::endl;
    os << "Data    : " << std::hex << std::setw(8) << std::setfill('0') << displayData.cvData << std::endl;
    os << "AM      : " << std::hex << std::setw(2) << std::setfill('0') << (uint32_t)displayData.cvAM << std::endl;
    os << std::endl;	       
    os << "DS0     : " << std::setw(1) << (0x01 & displayData.cvDS0) << std::endl;	
    os << "DS1     : " << std::setw(1) << (0x01 & displayData.cvDS1) << std::endl;	
    os << "AS      : " << std::setw(1) << (0x01 & displayData.cvAS) << std::endl;	
    os << "IACK    : " << std::setw(1) << (0x01 & displayData.cvIACK) << std::endl;	
    os << "WRITE   : " << std::setw(1) << (0x01 & displayData.cvWRITE) << std::endl;
    os << "LWORD   : " << std::setw(1) << (0x01 & displayData.cvLWORD) << std::endl;
    os << "DTACK   : " << std::setw(1) << (0x01 & displayData.cvDTACK) << std::endl;
    os << "BERR    : " << std::setw(1) << (0x01 & displayData.cvBERR) << std::endl;	
    os << "SYSRES  : " << std::setw(1) << (0x01 & displayData.cvSYSRES) << std::endl;
    os << "BR      : " << std::setw(1) << (0x01 & displayData.cvBR) << std::endl;
    os << "BG      : " << std::setw(1) << (0x01 & displayData.cvBG) << std::endl;   
}


void HAL::CAENLinuxBusAdapter::enableIRQ( uint32_t mask ) const
  throw( BusAdapterException ) {  
  int status;
  status = CAENVME_IRQEnable(handleVX718_[unit_][chain_], mask);
  if ( status != cvSuccess ) {
    std::stringstream message;
    message << "Could not enable Interrupts: CAEN error " << std::dec << status 
	    << std::ends;
    throw( BusAdapterException( message.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
}

void HAL::CAENLinuxBusAdapter::disableIRQ( uint32_t mask ) const 
  throw( BusAdapterException ) {  
  int status;
  status = CAENVME_IRQDisable(handleVX718_[unit_][chain_], mask);
  if ( status != cvSuccess ) {
    std::stringstream message;
    message << "Could not disable Interrupts: CAEN error " << std::dec << status 
	    << std::ends;
    throw( BusAdapterException( message.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
}

uint32_t  HAL::CAENLinuxBusAdapter::checkIRQ( ) const
  throw( BusAdapterException ) {  
  int status;
  CAEN_BYTE retval;
  status = CAENVME_IRQCheck(handleVX718_[unit_][chain_], &retval);
  if ( status != cvSuccess ) {
    std::stringstream message;
    message << "Could not check for Interrupts: CAEN error " << std::dec << status 
	    << std::ends;
    throw( BusAdapterException( message.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
  return (uint32_t)retval;
}

uint32_t HAL::CAENLinuxBusAdapter::waitIRQ( uint32_t mask, uint32_t timeoutMs ) const 
  throw( BusAdapterException ) {  
  int status;
  status = CAENVME_IRQWait(handleVX718_[unit_][chain_], mask, timeoutMs);
  if ( status != cvSuccess ) {
  //  // This currently means that there was either a problem
  //  // or the timeout has fired. So we cannot assume that 
  //  // something went wrong. 
  //
  //  //std::stringstream message;
  //  //message << "Could not wait for Interrupts: CAEN error " << std::dec << status 
  //  //	    << std::ends;
  //  //throw( BusAdapterException( message.str(), __FILE__, __LINE__, __FUNCTION__ ));
    return 0;
  }
  uint32_t retmask = checkIRQ();
  return retmask;
}

void HAL::CAENLinuxBusAdapter::acknowledgeIRQ( uint32_t level, void* vector, uint32_t width ) const 
  throw( BusAdapterException ) {
  int status;
  status = CAENVME_IACKCycle(handleVX718_[unit_][chain_], (CVIRQLevels)level, vector, (CVDataWidth)width);
  if ( status != cvSuccess ) {
    std::stringstream message;
    message << "Could not acknowledge Interrupt: CAEN error " << std::dec << status 
	    << std::ends;
    throw( BusAdapterException( message.str(), __FILE__, __LINE__, __FUNCTION__ ));
  }
}
