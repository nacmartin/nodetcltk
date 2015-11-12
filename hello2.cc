#include <nan.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tcl.h>
#include <tk.h>
#include <sstream>
#include <iostream>

using namespace v8;


std::string foo;
Local<Function> cb;


int Multi( ClientData Data, Tcl_Interp *interp, int argc, const char *argv[] ) {
    printf("%d\n", argc);

    std::string feet;
    std::string meters;
    std::stringstream strstream;
    strstream << Tcl_GetVar(interp, "::feet", 0);
    strstream >> feet;


    const unsigned nargc = 1;
    Local<Value> nargv[nargc] = { Nan::New(feet).ToLocalChecked() };
    Nan::MakeCallback(Nan::GetCurrentContext()->Global(), cb, nargc, nargv);
//    Nan::CallAsFunction(Nan::GetCurrentContext()->Global(), cb, nargc, nargv);
    //Local<Value> res = Nan::CallAsFunction(Nan::GetCurrentContext()->Global(), cb, nargc, nargv);
    Handle<Value> js_result;
    js_result = cb->Call(Nan::GetCurrentContext()->Global(), nargc, nargv);
    String::Utf8Value bar(js_result->ToString());

    std::string met(*bar);

    meters = "set ::meters "+ met;
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


void RunTclTk(const Nan::FunctionCallbackInfo<Value>& info) {


    if (info.Length() > 2) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    if (!info[0]->IsString()) {
        Nan::ThrowTypeError("Wrong arguments");
        return;
    }

    if (!info[1]->IsFunction()) {
        Nan::ThrowTypeError("Wrong arguments");
        return;
    }

    String::Utf8Value param1(info[0]->ToString());
    String::Utf8Value param2(info[1]->ToString());
    foo = std::string(*param1); 
    std::cout << std::string(*param2); 

    cb = Local<Function>::Cast(info[1]);

    char *ppszArg[1]; 

    // allocate strings and set their contents
    ppszArg[0] = (char *) foo.c_str();
//    strcpy( ppszArg[0], "" );

//    Tcl_Interp *interp = Tcl_CreateInterp();
//    InitProc(interp);
    Tk_Main(1, ppszArg, InitProc);
    //info.GetReturnValue().Set(Nan::New("world").ToLocalChecked());
}

void Init(Local<Object> exports) {

    exports->Set(Nan::New("tk").ToLocalChecked(),
            Nan::New<FunctionTemplate>(RunTclTk)->GetFunction());
}

NODE_MODULE(hello, Init)
