#ifndef __StopWatch
#define __StopWatch

#include <stdint.h>
#include <vector>
#include <iostream>
#include <sys/time.h>

#include "hal/IllegalValueException.hh"


namespace HAL {

class StopWatch {
public:
  StopWatch( uint32_t laps );
  
  void start();
  void stop();
  void lap();
  void reset();
  uint32_t read();
  uint32_t readLap( uint32_t lapIndex, std::ostream& os = std::cout ) 
    throw( IllegalValueException );
private:
  void calibrate();
  uint32_t subtractTime( timeval& stopTime, timeval& startTime );
  double startStopOffset;
  double lapOffset;
  uint32_t lapCounter;
  uint32_t maxLaps;
  timeval startTime;
  timeval stopTime;
  std::vector< timeval > lapVector;
};

} /* namespace HAL */

#endif /* __StopWatch */
