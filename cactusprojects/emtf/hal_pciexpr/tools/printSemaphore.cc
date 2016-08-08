#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <string.h>
#include "SBS620x86LinuxBusAdapter.hh"


/************************************************************************
*
*
*     @short A small utility to print the value of the Semaphore to 
*            protect the SBS BusAdapter agains concurrent access.
* 
*            Can be useful to insect the semphore values after a crash.
*            The values should all be one if no process has taken the 
*            semaphore. If this is not the case something went wrong. 
*            In this case the semaphore should be deleted with ipcrm 
*            (The id can be found with ipcs) and recreated with the
*            makeSBSSemaphore utility in this directory.
*            
*       @see 
*    @author $Author: cschwick $
*   @version $Revision: 1.1 $
*      @date $Date: 2004/06/27 12:54:36 $
*
*
**//////////////////////////////////////////////////////////////////////
int main() {
  
  int semId, i;
  key_t lockKey;
  char * errorMsg;
  
  /* get the key for the semaphoreset */
  lockKey = ftok(BT_DEVICE_FILE, BT_SEM_PROJID);
  if ( lockKey == -1 ) {
    errorMsg = strerror( errno );
    printf("Error during retrieval of key for semaphore: %s\n", errorMsg);
    exit(-1);
  }
  
  
  /* get the semaphore if it exists */
  semId = semget(lockKey, BT_MAX_UNITS, 0 );
  if ( semId == -1 ) {
    errorMsg = strerror( errno );
    printf( "Error while getting the semaphore Id: %s\n", errorMsg);
    exit(-1);
  }


  /* print semaphore values */

  int result;

  for ( i = 0; i < BT_MAX_UNITS; i++ ) {

    result = semctl(semId, i, GETVAL);

    printf( "number %3d  value %4d\n",i,result);

  }

}
