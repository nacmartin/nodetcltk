#include <nan.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tcl.h>
#include <tk.h>
#include <sstream>
#include <iostream>

std::string foo;


int Multi( ClientData Data, Tcl_Interp *interp, int argc, const char *argv[] ) {
    printf("%d\n", argc);
    long a = strtol(argv[0], (char **)NULL, 10);
    //long b = strtol(argv[1], (char **)NULL, 10);
    //std::string number;
    //std::stringstream strstream;
    //strstream << a * 4;
    //strstream >> number;
    //printf("Hola\n");
    //printf("%s\n", number.c_str());

    std::string feet;
    std::string meters;
    std::stringstream strstream;
    strstream << Tcl_GetVar(interp, "::feet", 0);
    strstream >> feet;

    meters = "set ::meters "+ feet;
    std::cout << meters;
    Tcl_Eval(interp, meters.c_str());

    return 0;
    //std::string number;
    //std::stringstream strstream;
    //strstream << a * b;
    //strstream >> number;
    //char *args[1]; 
    //args[0] = (char *) number.c_str();
    //return args;
}

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
    /* Read your startup code */
    printf("Hola\n");

    Tcl_CreateCommand(interp, "multi", Multi, (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL );
    Tcl_Eval(interp, foo.c_str());//"grid [ttk::button .b -text \"Hello World\"]; console hide ");
    return TCL_OK;
} // end InitProc

char *ppszArg[1]; 


void RunTclTk(const Nan::FunctionCallbackInfo<v8::Value>& info) {


    if (info.Length() > 1) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    if (!info[0]->IsString()) {
        Nan::ThrowTypeError("Wrong arguments");
        return;
    }

    v8::String::Utf8Value param1(info[0]->ToString());
    foo = std::string(*param1); 

    char *ppszArg[1]; 

    // allocate strings and set their contents
    ppszArg[0] = (char *) foo.c_str();
//    strcpy( ppszArg[0], "" );

//    Tcl_Interp *interp = Tcl_CreateInterp();
//    InitProc(interp);
    Tk_Main(1, ppszArg, InitProc);
    //info.GetReturnValue().Set(Nan::New("world").ToLocalChecked());
}

void Init(v8::Local<v8::Object> exports) {

    exports->Set(Nan::New("tk").ToLocalChecked(),
            Nan::New<v8::FunctionTemplate>(RunTclTk)->GetFunction());
}

NODE_MODULE(hello, Init)
