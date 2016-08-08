#ifndef __TesterInterface
#define __TesterInterface

#include <string>

namespace HAL {

/**
*
*
*     @short Interface for automatic testing of classes.
*            
*            A class  inheriting from this interface can be registered
*            with the TestMaster class. The TestMaster itself can then
*            perform  automated tests  of the  registered  objects via
*            this interface.
*
*       @see 
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 17:44:00 $
*
*
**/

class TesterInterface {
public:
  /**
   * Execute the test to be done by this class.
   * @return true if the test is successfull.
   */
  virtual bool execute() = 0;
  
  /**
   * Return the name of the test.
   */
  virtual std::string getName() const = 0;

  /**
   * Return a description of the Tester.
   */
  virtual std::string getDescription() const {
    return std::string("No description available for this tester");
  }
};

} /* namespace HAL */

#endif /* __TesterInterface */
