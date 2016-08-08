#ifndef __PersistentCommandSequencer
#define __PersistentCommandSequencer

#include <string>
#include <tr1/unordered_map>

#include "hal/NoSuchFileException.hh"
#include "hal/CommandSequencer.hh"
#include "hal/CommandSequence.hh"
#include "hal/CommandSequenceASCIIReader.hh"

namespace HAL {

/**
*
*
*     @short Adds persistency to the CommandSequencer.
*            
*            This class is a CommandSequencer which saves its internal
*            status  in a  file.  When the  program  is restarted  the
*            status  is  reloaded from  the  file  so that  previously
*            registered  sequences  are  immediately  available,  also
*            after a restart of the program.  This class is convenient
*            for small test programs.
*            
*       @see 
*    @author Christoph Schwick
* $Revision: 1.1 $
*     $Date: 2007/03/05 17:44:00 $
*
*
**/

class PersistentCommandSequencer : public  CommandSequencer {
public:

  /**
   * Constructor of the class.
   *
   * @param     configurationFile is the fileName of the file where the 
   *            status of the CommandSequencer is saved. Each line in 
   *            the file contains the name of a sequence file. Also the
   *            last entry must be followed by a carriage return. 
   */
  PersistentCommandSequencer( std::string configurationFile,
                              const AddressTableInterface& addressTable )
    throw( IllegalOperationException );

  ~PersistentCommandSequencer();

  void registerSequence( std::string sequenceName );

  void deleteSequence( std::string sequenceName )
    throw( IllegalValueException );
  
  void deleteSequences();

private:

  void loadSequences( );
  void saveSequences();
  
  std::string configurationFile;
  const AddressTableInterface& addressTable;
  std::tr1::unordered_map< std::string, CommandSequenceReader* > readerMap;
};

} /* namespace HAL */

#endif /* __PersistentCommandSequencer */
