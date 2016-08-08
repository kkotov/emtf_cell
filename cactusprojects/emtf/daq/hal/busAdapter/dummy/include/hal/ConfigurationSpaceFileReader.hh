#ifndef __ConfigurationSpaceFileReader
#define __ConfigurationSpaceFileReader

#include <stdint.h>
#include <string>
#include <tr1/unordered_map>
#include "hal/NoSuchFileException.hh"
#include "hal/IllegalValueException.hh"

namespace HAL {

/**
*
*
*     @short Reads the description of a VME64x CR/CSR from an
*            ASCII file.
*            
*            This     class    is     used    together     with    the 
*            VME64xDummyBusAdapter.  It  reads  an  ASCII  file  which 
*            contains the  description of CR/CSR space.  The format of 
*            the file is best explained with an example:
*            <pre>
*            000000 :  6e000000                            # "checksum",             1,   "checksum of configuration ROM" 
*            000004 :  00000000 04000000 08000000          # "romLength",            3,   "length of the Configuration ROM"
*            000010 :  81000000                            # "CRWidth",              1,   "Configuration ROM (CR)  access width" 
*            000014 :  00000000                            # "CSRWidth",             1,   "Configuration Status Reeg (CSR) access width" 
*            000018 :  02000000                            # "specificationId",      1,   "CR/CSR specification ID" 
*            00001c :  43000000 52000000                   # "CR",                   2,   "to identify valid CR" 
*            000690 :  00000000 00000000 00000000 00000000 # "ADEM-F7",              4,   "Address Decoder Mask function 7: byte 3" 
*            
*            
*            
*            #
*            001000 :  64000000 61000000 71000000 76000000 6d000000 65000000 31000000 30000000 31000000 # serial number "daqvme001"
*            #
*            # The CSRAM space 
*            #
*            07ff6c : 00000000 00000000 00000000 00000000  # "ADER-F0",             4,"Address Decoder Compare Register function 0"
*            </pre>
*            
*            Lines starting with #  are comment lines. Comments may be
*            put at the end of a  line, two (preceded by a "#"). Every
*            other  line  starts with  a  32-bit  aligned hex  address 
*            which is the offset into  the CR/CSR space, followed by a
*            "  : "  and  the data  contents.  Contents is  in 32  bit 
*            words.  Since   in  the  CR  space   the  information  is 
*            contained  in the  bytes n+3,  only the  most significant 
*            byte  of the  data  words are  different  from 0.  (This, 
*            depending on the module architecture,  might be different 
*            for the  user defined  region of the  CR/CSR  space  (see 
*            specification for details).
*            An  address may  be followed  by an  arbitrary  number of 
*            data words. Lines sorted by increasing addresses.
*
*       @see 
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/06 09:54:25 $
*
*
**/

class ConfigurationSpaceFileReader {
public:
  /**
   * The Constructor of the ConfigurationSpaceFileReader parses
   * the input file and stores the contents of the CR/CSR space 
   * a newly created map on the heap. 
   */
   ConfigurationSpaceFileReader( std::string fileName ) 
     throw (NoSuchFileException,
            IllegalValueException);

  /**
   * The destructor deletes the internal map containing 
   * the CR/CSR definition only if the method getDataMapPtr()
   * has not been called on the object.
   */
  ~ConfigurationSpaceFileReader( );

  /**
   * Returns the pointer to a map with the contents of the 
   * CR/CSR space.
   * IMPORTANT: As soon as this function is called once, the 
   * map refered to by the returned pointer is not owned by 
   * this class anymore. This means the user must delete the 
   * map if it is not needed anymore. 
   * On the contrary if an object of this class is deleted
   * before this method has been called, the internal map is 
   * deleted by the destructor of this class.
   * @return A pointer to a map containing the CR/CSR definition. 
   *         The key of the map is an address offset in the 
   *         CR/CSR space, and the value is the contents. The
   *         address is 32 bit aligned.
   */
    std::tr1::unordered_map<uint32_t, uint32_t>* getDataMapPtr() ;
private:
  bool gaveDataAway_;
    std::tr1::unordered_map< uint32_t, uint32_t >* dataMapPtr_;
};


} /* namespace HAL */

#endif
