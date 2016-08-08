#include "PCICard.h"

// create new pcicard (factory method)
  int PCICard::create(ClientData clientData, Tcl_Interp * interp,
              int argc, char* argv[]){
    if(argc!=6 && argc != 7){
      interp->result = "wrong number of arguments";
      return TCL_ERROR; 
    }
    char* name = argv[1];
    int  vendorid, deviceid, index, swapInt;
    bool swapFlag;
    if(Tcl_GetInt(interp,argv[2],&vendorid)!=TCL_OK){
      interp->result = "second argument must be vendor ID";
      return TCL_ERROR; 
    }
    if(Tcl_GetInt(interp,argv[3],&deviceid)!=TCL_OK){
      interp->result = "third argument must be device ID";
      return TCL_ERROR; 
    }
    if(Tcl_GetInt(interp,argv[4],&index)!=TCL_OK){
      interp->result = "fourth argument must be index of pcicard";
      return TCL_ERROR; 
    }
    char* addressMap = argv[5];

    if (argc == 7 ) {
            if(Tcl_GetBoolean(interp,argv[6],&swapInt)!=TCL_OK){
                    interp->result = "sixth argument must be swapflag";
                    return TCL_ERROR; 
            }
            
    } else {
            swapInt = 0;
    }
    swapFlag = (bool) swapInt;
    try {
            PCICard* newPCICard = new PCICard(vendorid, deviceid, index, addressMap, swapFlag);
            newPCICard->name=name;
            Tcl_CreateCommand(interp,name,PCICard::command,(ClientData) newPCICard,
                              (Tcl_CmdDeleteProc *) PCICard::deletePCICard);
            return TCL_OK;
    }  catch ( HardwareAccessException & e ) {
            sprintf(interp->result, "HAL error: %s", e.what());
            return TCL_ERROR;
    }
  }

// delete pcicard
  void PCICard::deletePCICard(ClientData clientData){
    delete (PCICard*) clientData;
  }

// execute command on pcicard
int PCICard::command(ClientData clientData, Tcl_Interp * interp,
                  int argc, char* argv[]){
        int status = TCL_OK;
        if(argc==2){
                // one argument: read
                uint32_t value;
                try{
                        ((PCICard*)clientData)->pcicard.read(argv[1],&value);
                        sprintf(interp->result,"%lu",value);
                        status = TCL_OK;
                } catch ( HardwareAccessException & e ) {
                        sprintf(interp->result, "HAL error: %s", e.what());
                        status = TCL_ERROR;
                }
        } else if (argc==3) {
                // two arguments: write
                int value;
                if(Tcl_GetInt(interp,argv[2],&value)==TCL_OK){
                        try{
                                ((PCICard*)clientData)->pcicard.write(argv[1],(uint32_t)value);
                                interp->result = "";
                                status = TCL_OK;
                        } catch ( HardwareAccessException & e ) {
                                sprintf(interp->result, "HAL error: %s", e.what());
                                status = TCL_ERROR;
                        }
                } else {
                        interp->result = "wrong second argument";
                        status = TCL_ERROR;
                }
        } else {
                interp->result = "wrong number of arguments";
                status = TCL_ERROR;
        }
        return status;
};

// constructor (private)
PCICard::PCICard(uint32_t vendorid, uint32_t deviceid, uint32_t index,
                 char* addressMap, bool swapFlag):  busAdapter(), 
        addressTableReader( addressMap ),
        addressTable( "TTCrx address table", addressTableReader ),  
        pcicard(addressTable, busAdapter, vendorid, deviceid, index, swapFlag ){
};




