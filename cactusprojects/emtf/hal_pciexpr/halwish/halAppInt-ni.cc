#include "tk.h"
#include "NICard.h"

int main(int argc, char ** argv)
{
    Tk_Main(argc, argv, Tcl_AppInit);
    return 0;                   /* Needed only to prevent compiler warning. */
}

int
Tcl_AppInit(Tcl_Interp * interp)
{
    if (Tcl_Init(interp) == TCL_ERROR) {
        return TCL_ERROR;
    }
    if (Tk_Init(interp) == TCL_ERROR) {
        return TCL_ERROR;
    }
    Tcl_StaticPackage(interp, "Tk", Tk_Init, Tk_SafeInit);

    /*
     * Call the init procedures for included packages.  Each call should
     * look like this:
     *
     * if (Mod_Init(interp) == TCL_ERROR) {
     *     return TCL_ERROR;
     * }
     *
     * where "Mod" is the name of the module.
     */

    /*
     * Call Tcl_CreateCommand for application-specific commands, if
     * they weren't already created by the init procedures called above.
     */

    Tcl_CreateCommand(interp,"nivmecard",NICard::create,(ClientData) 0,
                      (Tcl_CmdDeleteProc *) 0);
    
    /*
     * Specify a user-specific startup file to invoke if the application
     * is run interactively.  Typically the startup file is "~/.apprc"
     * where "app" is the name of the application.  If this line is deleted
     * then no user-specific startup file will be run under any conditions.
     */

    Tcl_SetVar(interp, "tcl_rcFileName", "~/.wishrc", TCL_GLOBAL_ONLY);
    return TCL_OK;
}
