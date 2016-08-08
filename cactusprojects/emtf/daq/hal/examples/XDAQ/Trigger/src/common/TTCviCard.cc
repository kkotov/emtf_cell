#include "halTriggerExample/TTCviCard.hh"

halTriggerExample::TTCviCard::TTCviCard(HAL::VMEAddressTable & addressTable,
		     HAL::VMEBusAdapterInterface & busAdapter,
                     uint32_t baseAddress ) :
  HAL::VMEDevice( addressTable, busAdapter, baseAddress ) 
{
}

// select any trigger source (0-7)
void halTriggerExample::TTCviCard::selectTriggerSource(uint32_t data){
  if(data<8){
    this->write("L1ATriggerSelect",data);
  } else {
    this->disableTrigger();
  }
}

// select external trigger source (0-3)
void halTriggerExample::TTCviCard::selectExternal(uint32_t data){
  if(data<4){
    this->write("L1ATriggerSelect",data);
  } else {
    this->disableTrigger();
  }
}

// select trigger by VME access
void halTriggerExample::TTCviCard::selectManual(){
  this->write("L1ATriggerSelect",4);
}

// select random trigger generator with rate in Hz
uint32_t halTriggerExample::TTCviCard::selectInternal(uint32_t rate){
  uint32_t newRate = setTriggerRate(rate);
  this->write("L1ATriggerSelect",5);
  return newRate;
}

// select calibration trigger mode
void halTriggerExample::TTCviCard::selectCalibration(){
  this->write("L1ATriggerSelect",6);
}

// disable trigger geneation
void halTriggerExample::TTCviCard::disableTrigger(){
  this->write("L1ATriggerSelect",7);
}

// send manual trigger
void halTriggerExample::TTCviCard::sendManual(){
  this->writePulse("L1ASend");
}

// send manual trigger
void halTriggerExample::TTCviCard::reset(){
  this->writePulse("SoftReset");
}

// set the rate of the internal random trigger
uint32_t halTriggerExample::TTCviCard::setTriggerRate( uint32_t data ) {
      uint32_t table[] = 
        {1,100, 1000, 5000, 10000, 25000, 50000, 100000};
      uint32_t code = 0;
      while ( code < 7 && data >= table[code+1] ) code++;
      this->write("RandomTriggerRate",code);
      return table[code];
}

// read event counter
uint32_t halTriggerExample::TTCviCard::getTriggerCount(){
  uint32_t MSB;
  this->read("CounterMSB", &MSB);
  uint32_t LSB;
  this->read("CounterLSB", &LSB);
  return (MSB << 16) + LSB;
}

// set event counter
void halTriggerExample::TTCviCard::setTriggerCount(uint32_t data){
  this->write("CounterMSB",data >> 16);
  this->write("CounterLSB",data & 0xFFFF);
}
