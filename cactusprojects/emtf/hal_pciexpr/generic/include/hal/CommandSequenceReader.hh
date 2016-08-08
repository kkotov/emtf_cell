#ifndef __CommandSequenceReader
#define __CommandSequenceReader

#include <string>
#include <list>

namespace HAL {

/**
*
*
*     @short The Interface for a Reader of command sequences.
*
*            This  interface  is  used   in  the  constructor  of  the
*            CommandSequence  to  build up  the  sequence. This  class
*            should  not be  instantiated  directly. It  is the  child
*            classes  which   implement  the  different   methods  for
*            retrieving commands from different media (file, database,
*            memory,  ...) or  from  different representation  formats
*            (ASCII, XML, ...). The functionality to retrieve the data
*            from the reader is implemented in this class. 
*            
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 18:02:09 $
*
*
**/

class CommandSequenceReader {
public:
  /**
   * Delivers the next command in the sequence.
   * @param commandStringList is a reference to a list of strings
   *        which contains the complete command.
   * @return false if there is no more command.
   */
  bool next( std::list< std::string >& commandStringList );

  /**
   * Resets the reader to the start. The Sequence can now be
   * read another time.
   */
  virtual void rescan() = 0;
  virtual ~CommandSequenceReader();
protected:
  /**
   * The constructor is PROTECTED.
   * Nobody should ever instantiate this class directly.
   */
  CommandSequenceReader();
  std::list< std::list< std::string > > commandList;  
  bool firstAccess;
private:
  std::list< std::list< std::string > >::iterator it;
};

} /* namespace HAL */

#endif /* __CommandSequenceReader */
