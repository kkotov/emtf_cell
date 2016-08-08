#ifndef __StaticVMEConfigurationReader
#define __StaticVMEConfigurationReader

#include <list>
#include "hal/StaticVMEItem.hh"

namespace HAL {

class StaticVMEConfigurationReader {
public:
  virtual ~StaticVMEConfigurationReader();

  /**
   * Method to retrieve Static VMEItems from some source.
   * Ahe call to next, *nextItem contains a pointer to the
   * next StaticVMEItem. If there are no more items to retrieve
   * the method returns false (and the value of *nextItem is
   * undefined).
   * After a user has retrieved an item via the next method
   * he is responsible for the destruction of the object 
   * retrieved.
   */  
  bool next( StaticVMEItem** nextItem );
protected:
  /**
   * The constructor is protected since only derived classes 
   * should be instantiated.
   */
  StaticVMEConfigurationReader();
  std::list< StaticVMEItem* > staticVMEItemList_;
private:
  bool firstAccess_;
  std::list< StaticVMEItem* >::iterator it;

};

} /* namespace HAL */

#endif /* __StaticVMEConfigurationReader */
