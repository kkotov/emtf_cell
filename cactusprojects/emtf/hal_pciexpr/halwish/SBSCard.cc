#include "SBSCard.h"

// create new card (factory method)
  int SBSCard::create(ClientData clientData, Tcl_Interp * interp,
              int argc, char* argv[]){
    if(argc!=4){
      interp->result = "wrong number of arguments";
      return TCL_ERROR; 
    }
    char* name = argv[1];
    int  baseAddress;
    if(Tcl_GetInt(interp,argv[2],&baseAddress)!=TCL_OK){
      interp->result = "second argument must be base adress";
      return TCL_ERROR; 
    }
    char* addressMap = argv[3];
    try {
            SBSCard* newCard = new SBSCard(baseAddress,addressMap);
            newCard->name=name;
            Tcl_CreateCommand(interp,name,SBSCard::command,(ClientData) newCard,
                              (Tcl_CmdDeleteProc *) SBSCard::deleteCard);
            return TCL_OK;
    }  catch ( HardwareAccessException & e ) {
            sprintf(interp->result, "HAL error: %s", e.what());
            return TCL_ERROR;
    }
  }

// delete card
  void SBSCard::deleteCard(ClientData clientData){
    delete (SBSCard*) clientData;
  }

// execute command on card
int SBSCard::command(ClientData clientData, Tcl_Interp * interp,
                  int argc, char* argv[]){
        int status = TCL_OK;
        if(argc==2){
                // one argument: read
                uint32_t value;
                try{
                        ((SBSCard*)clientData)->card.read(argv[1],&value);
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
                                ((SBSCard*)clientData)->card.write(argv[1],(uint32_t)value);
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
  SBSCard::SBSCard(uint32_t baseAddress,char* addressMap):  busAdapter(), 
   addressTableReader( addressMap ),
   addressTable( "TTCrx address table", addressTableReader ),  
   card(addressTable, busAdapter, baseAddress )
  {};




