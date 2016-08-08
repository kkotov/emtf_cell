#ifndef __TestMaster
#define __TestMaster

#include <list>
#include <tr1/unordered_map>
#include <iostream>
#include "hal/TesterInterface.hh"
#include "hal/HardwareAccessException.hh"

namespace HAL {

/**
*
*
*     @short A class to perform automated testing of classes.
*            
*            The TestMaster  can be used to  perform automatic testing 
*            of software  classes. The  testcode for a  software class 
*            must be  implemented in a  class which inherits  from the 
*            TesterInterface. TesterInterfaces  can be registered with
*            teh TestMaster, all registered  classes can be tested and
*            the result of  the test can be reported.  In the HAL this 
*            is used to do tests on newly written classes.
*
*       @see 
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 17:44:00 $
*
*
**/

class TestMaster {
public:
  TestMaster( std::ostream& os = std::cout );

  /**
   * Register a class with testcode with the TestMaster.
   */
  void registerTester( TesterInterface& newTester );

  /**
   * Excecute a test with all registered objects.
   *
   * This method calls the execute() method of all registered
   * TesterInterfaces.
   */
  bool doTest() throw (HardwareAccessException);

  /**
   * Print the name of the test and if the test was successfull or not.
   */
  void printShortReport() ;

  /**
   * Print a long description of the test and if is was successfull.
   */
  void printLongReport() ;

private:
  std::ostream& os_;
  std::list< TesterInterface* > testerList_;
  std::tr1::unordered_map< TesterInterface*, bool > resultMap_;
  bool testDone_;
};

} /* namespace HAL */

#endif /* __TestMaster */
