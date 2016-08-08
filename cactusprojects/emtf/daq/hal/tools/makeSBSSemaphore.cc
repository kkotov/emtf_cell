#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <string.h>
#include "SBS620x86LinuxBusAdapter.hh"
#include "btapi.h"

/************************************************************************
*
*
*     @short A small utility to  create and initialize a semaphore for
*            to protect the SBS driver against concurrent access.
*            
*            The  SBS  driver  must  be protected  against  concurrent 
*            accesses from  various processes or tasks using  the HAL.  
*            Since  the  SBS  libraray  does  not  provide  a  working 
*            mechanism  for this,  a semaphore  is optionally  used by 
*            the SBS BusAdapter. This routine  must be run in case the
*            semaphore does not exist yet.  It is not clean to let the
*            BusAdapter  itself create  the semaphore  because between 
*            creation  and  initialization  the  semaphore  cannot  be 
*            protected against concurrent access.
*
*       @see 
*    @author $Author: cschwick $
*   @version $Revision: 1.1 $
*      @date $Date: 2004/06/27 12:54:36 $
*
*
**//////////////////////////////////////////////////////////////////////
int main() {
  
  int error, semId, i;
  key_t lockKey;
  char * errorMsg;
  struct sembuf sops;
  
  /* get the key for the semaphoreset */
  lockKey = ftok(BT_DEVICE_FILE, BT_SEM_PROJID);
  if ( lockKey == -1 ) {
    errorMsg = strerror( errno );
    printf("Error during retrieval of key for semaphore: %s\n", errorMsg);
    exit(-1);
  }
  
  
  /* create the semaphore if it does not yet exist */
  semId = semget(lockKey, BT_MAX_UNITS, IPC_CREAT | IPC_EXCL | 0777 );
  if ( semId == -1 ) {
    if ( errno == EEXIST ) {
      printf( "Semaphore for SBS BusAdapter already exits\n");
      exit(-1);
    }
    errorMsg = strerror( errno );
    printf( "Error while getting/creating the semaphore Id: %s\n", errorMsg);
    exit(-1);
  }


  /* initialize the semaphores */
  for ( i = 0; i < BT_MAX_UNITS; i++ ) {
    sops.sem_num = i;
    sops.sem_op  = 1;
    sops.sem_flg = 0;

    error = semop( semId, &sops, 1);
    
    if ( error == -1 ) {
      errorMsg = strerror( errno );
      printf("Error during semop call: %s\n", errorMsg);
    }

  }

}
