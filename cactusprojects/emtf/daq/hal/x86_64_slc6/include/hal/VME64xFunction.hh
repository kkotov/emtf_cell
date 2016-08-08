#ifndef __VME64xFunction
#define __VME64xFunction

#include <list>
#include "hal/VMEConfigurationSpaceHandler.hh"
#include "hal/VME64xMappedWindow.hh"
#include "hal/UnsupportedException.hh"
#include "hal/IllegalValueException.hh"

// A function is identified by an address table; if one function 
// has more than one function decoder it means that the same 
// functionality (= address table) is mapped into different windows.
// If a module has different functions it means that in the windows
// corresponding to the functions different addresse tables are valid.

namespace HAL {

class VME64xFunction {
  
public:
  
  VME64xFunction( uint32_t slotId,
                  uint32_t functionId,
                  VMEConfigurationSpaceHandler* vmeAccess )
    throw( UnsupportedException, 
           IllegalValueException );
  
  VME64xFunction( uint32_t slotId,
                  uint32_t baseAddress,
                  uint32_t addressRank );

  virtual ~VME64xFunction();

  uint32_t getNumberOfMappedWindows() const;
  bool isImplemented() const;
  
  uint32_t getAddressRank() const;

  /**
   * Be aware that the pointers in the list can be used to change the 
   * contents of the mapped windows. This is needed during configuration
   * in order to configure the windows with their baseaddress and their 
   * AM.
   */
  std::list< VME64xMappedWindow* > getMappedWindowPtrList() const;
  
  
private:
  uint32_t slotId_;
  uint32_t functionId_; // between 0 and 7
  VMEConfigurationSpaceHandler* vmeConfigurationSpaceHandlerRef_; // needed to set the ADER_  
  std::list< VME64xMappedWindow* > mappedWindowPtrList_;  // owns the mapped windows
};

} /* namespace HAL */

#endif /* __VME64xFunction */

