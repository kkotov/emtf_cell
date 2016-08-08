#ifndef __CommandSequenceASCIIReader
#define __CommandSequenceASCIIReader

#include <string>

#include "hal/CommandSequenceReader.hh"
#include "hal/NoSuchFileException.hh"

namespace HAL {

/**
*
*
*     @short Reads a command sequence from a plain ASCII file. 
*            
*            The  format of  the file  can be  best explained  with an 
*            example: 
*            <pre>
*            # comments are allowed if the line starts with a "#"
*            write memStart 0x1111
*            write mem0 18
*            write mem1 0x11
*            # empty lines also are allowed
*            
*            define $offset 0
*            define $count 0x0
*            label loop
*            write memStart $count $offset
*            add $count 1
*            add $offset 4
*            goto loop $offset <= 0x10
*            print Last value written was %dec $count at offset %hex $offset
*            </pre>
*
*       @see 
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/

class CommandSequenceASCIIReader : public CommandSequenceReader {
public:
  CommandSequenceASCIIReader( std::string fileName ) 
    throw (NoSuchFileException) ;
  ~CommandSequenceASCIIReader( ) ;
  void rescan()
    throw (NoSuchFileException) ;
private:
  std::string fileName;
  void scanInputFile();
};

} /* namespace HAL */

#endif /* __CommandSequenceASCIIReader */
