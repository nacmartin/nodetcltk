#include <nan.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tcl.h>
#include <tk.h>

int InitProc( Tcl_Interp *interp )
{
    /* Basic init */
    if (Tcl_Init(interp) == TCL_ERROR)
        return TCL_ERROR;
    if (Tk_Init(interp) == TCL_ERROR)
        return TCL_ERROR;
    Tcl_StaticPackage(interp, "Tk", Tk_Init, Tk_SafeInit);
    /* Make your commands here */
    //Tcl_CreateCommand(interp, "set_parser",           SET_Parser, NULL, NULL);
    //Tcl_CreateCommand(interp, "set_c2tcl_data",       SET_C2TclData, NULL, NULL);
    //Tcl_CreateCommand(interp, "set_random_number",    SET_RandomNumber, NULL, NULL);
    //Tcl_CreateCommand(interp, "set_scale_graph_data", SET_ScaleGraphData, NULL, NULL);
    //Tcl_CreateCommand(interp, "set_free_and_reset_module_data", 
    //        SET_FreeAndResetModuleData, NULL, NULL);
    //Tcl_CreateCommand(interp, "set_free_and_reset_global_data", 
    //        SET_FreeAndResetGlobalData, NULL, NULL);
    //Tcl_CreateCommand(interp, "set_reset_sim_control_parameter", 
    //        SET_ResetSimControlParameter, NULL, NULL);
    ///* Can't remember if you need this */
    //Tcl_SetVar(interp, "tcl_rcFileName", "~/.simonrc", TCL_GLOBAL_ONLY);
    /* Now your remaining initialization - assume no error for this! */
    /* Read your startup code */
    return Tcl_Eval(interp, "source ./simon.tk");
} // end InitProc

int main(int argc, char **argv) {
    //char *ppszArg[2]; 

    // allocate strings and set their contents
    //ppszArg[0] = (char *)malloc( sizeof( char ) * 12 );
    //ppszArg[1] = (char *)malloc( sizeof( char ) * 12 );
    //strcpy( ppszArg[0], "Hello World" );
    //strcpy( ppszArg[1], "./hello.tcl" ); 

    //printf("hola\n");
    // the following call does not return
    //Tk_Main( 2, ppszArg, InitProc );

    //Tk_MainLoop();
    Tk_Main(argc, argv, InitProc);
    return(0);
}
