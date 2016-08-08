#ifndef __TTCviCard
#define __TTCviCard

#include "hal/VMEDevice.hh"

namespace halTriggerExample
{

class TTCviCard : public HAL::VMEDevice {

public :
  TTCviCard(HAL::VMEAddressTable & addressTable,
            HAL::VMEBusAdapterInterface & busAdapter,
            uint32_t baseAddress);

  // select any trigger source (0-7)
  void selectTriggerSource(uint32_t data);
  // select external trigger source (0-3)
  void selectExternal(uint32_t data);
  // select trigger by VME access
  void selectManual();
  // select random trigger generator with rate in Hz
  // returns new rate in Hz (may be different)
  uint32_t selectInternal(uint32_t rate);
  // disable trigger geneation
  void disableTrigger();
  // select calibration trigger mode
  void selectCalibration();

  // software reset of TTCvi card
  void reset();
  // send manual trigger by VME access
  void sendManual();

  // (pre) set internal trigger rate
  uint32_t setTriggerRate( uint32_t data ) ;

  // read event counter
  uint32_t getTriggerCount();
  // set event counter
  void setTriggerCount(uint32_t data);

private:

};

} /* halTriggerExample */

#endif /* __TTCviCard */





