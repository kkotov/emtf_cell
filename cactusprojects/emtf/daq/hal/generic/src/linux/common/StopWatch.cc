#include "hal/linux/StopWatch.hh"
#include <sstream>
#include <iomanip>

HAL::StopWatch::StopWatch( uint32_t laps ) {
        lapVector.reserve(laps);
        maxLaps = laps;
        calibrate();
}

void HAL::StopWatch::start() {
        gettimeofday(&startTime,NULL);
}

void HAL::StopWatch::stop() {
        gettimeofday(&stopTime,NULL);
}

void HAL::StopWatch::lap() {
        gettimeofday( &stopTime,NULL );
        lapVector.push_back( stopTime );
}

uint32_t HAL::StopWatch::read() {
        return subtractTime( stopTime, startTime );
}

uint32_t HAL::StopWatch::readLap( uint32_t lapIndex, std::ostream& os )
  throw (IllegalValueException) {
        if ( lapIndex > (lapVector.size() - 1) ) {
		  std::stringstream text;
		  text << "ERROR ===> only " << std::dec << lapVector.size() << "laps taken." << std::ends;
		  throw( IllegalValueException( text.str(), __FILE__, __LINE__, __FUNCTION__ )); 
        } else if ( lapIndex > maxLaps ) {
                os << "ERROR ===> maximal lap index is " << std::dec << maxLaps << std::endl;
                os << "Measurements might be inprecise due to extra memory allocation" << std::endl;
        }
        return  subtractTime( lapVector[lapIndex], startTime );
}

void HAL::StopWatch::reset() {
        lapCounter = 0;
        lapVector.clear();
}

void HAL::StopWatch::calibrate() {

}

uint32_t HAL::StopWatch::subtractTime(struct timeval& t, struct timeval& sub) {
        signed long sec, usec, rsec, rusec;
        sec = t.tv_sec - sub.tv_sec;
        usec = t.tv_usec - sub.tv_usec;
        if (usec < 0) {
                sec--;
                usec += 1000000;
  }
        if (sec < 0) {
                rsec = 0;
                rusec = 0;
        }
        else {
                rsec = (uint32_t) sec;
                rusec = (uint32_t) usec;
        }
        return (rsec * 1000000 + rusec);
}
