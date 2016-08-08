#ifndef __HalHash
#define __HalHash

#include <string>

namespace HAL {

/**
*
*
*     @short Template class for a hash function
*            
*            This template and  its specialization has been introduced
*            in order  to have a  hash function for strings  (which is 
*            not  existing in  the STL  currently.) The  algorithm has 
*            been copied from the hash function of char* in the STL.
*
*       @see 
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/

template <class _Key> struct HalHash { };

template<>
struct HalHash< std::string > {
    size_t operator()(const std::string& __i) const {
      const char* __s = __i.c_str();
      uint32_t __h = 0; 
      for ( ; *__s; ++__s)
        __h = 5*__h + *__s;
      return size_t( __h );
    }
  };

} /* namespace HAL */

#endif /* __HalHash */
